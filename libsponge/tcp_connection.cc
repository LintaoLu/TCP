#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const { return _sender.stream_in().remaining_capacity(); }

size_t TCPConnection::bytes_in_flight() const { return _sender.bytes_in_flight(); }

size_t TCPConnection::unassembled_bytes() const { return _receiver.unassembled_bytes(); }

size_t TCPConnection::time_since_last_segment_received() const { return last_segment_received_time; }

void TCPConnection::segment_received(const TCPSegment &seg) {
    last_segment_received_time = 0;

    const TCPHeader &header = seg.header();
    const bool empty_seg = seg.length_in_sequence_space() == 0;
    const bool good_ack = seg.header().ack && (_sender.next_seqno() == header.ackno);
    if (header.rst) {
        if (state() == TCPState::State::SYN_SENT && !good_ack) return;
        terminate_connection();
        return;
    }
    _receiver.segment_received(seg);
    // Passive close. If both byte stream end, no need to linger.
    if (_receiver.stream_out().input_ended() && !_sender.stream_in().input_ended()) {
         _linger_after_streams_finish = false;
    }
    if (header.ack) {
        // I follow fsm_ack_rst.cc's requirements. If invalid ack send during listen, throw error.
        if (state() == TCPState::State::LISTEN && !header.syn) {
            terminate_connection(true);
            return;
        }
        // If sender just send a syn, it's only accepts segment with both ack and syn.
        // This is according to the definition of 3-way handshake!
        if (state() == TCPState::State::SYN_SENT && !header.syn) {
            // Should return error if ack is invalid.
            if (!good_ack) {
                TCPSegment err_seg{};
                err_seg.header().seqno = header.ackno; // Maybe let peer know this ack is accepted.
                _sender.segments_out().emplace(err_seg);
                terminate_connection(true);
            }
            return;
        }
        _sender.ack_received(seg.header().ackno, seg.header().win);
    }

    bool future_ack(_sender.get_unwrapped_no(header.ackno) > _sender.get_unwrapped_no(_sender.next_seqno()));
    // 1. Data segment should get reply.
    // 2. Receiver should response to segment who has wrong sequence number.
    // 3. Future ack should get reply.
    if (!empty_seg || (_receiver.ackno().has_value() && _receiver.ackno() != header.seqno) || future_ack) {
        // If receiver is in listen, segment received must be SYN.
        if (state() == TCPState::State::LISTEN && !header.syn) return;
        _sender.fill_window();
        if (_sender.segments_out().empty()) _sender.send_empty_segment();
        add_ack_and_send_out();
    }
}

bool TCPConnection::active() const {
    if (_sender.stream_in().error() || _receiver.stream_out().error()) return false;
    if (_sender.stream_in().eof() && _receiver.stream_out().eof() && bytes_in_flight() == 0) {
        if (_linger_after_streams_finish && last_segment_received_time < 10 * _cfg.rt_timeout) {
            return true;
        }
        return false;
    }
    return true;
}

size_t TCPConnection::write(const string &data) {
    size_t bytes_written(min(remaining_outbound_capacity(), data.size()));
    _sender.stream_in().write(data);
    add_ack_and_send_out();
    return bytes_written;
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) {
    // Throw error if too many repeated transmission.
    if (_sender.consecutive_retransmissions() > TCPConfig::MAX_RETX_ATTEMPTS) {
        terminate_connection(true);
        return;
    }
    last_segment_received_time += ms_since_last_tick;
    _sender.tick(ms_since_last_tick);
    if (state() == TCPState::State::ESTABLISHED) _sender.fill_window();
    add_ack_and_send_out();
}

void TCPConnection::end_input_stream() {
    _sender.stream_in().end_input();
    _sender.fill_window();
    add_ack_and_send_out();
}

void TCPConnection::connect() {
    _sender.fill_window();
    add_ack_and_send_out();
}

void TCPConnection::add_ack_and_send_out() {
    while (!_sender.segments_out().empty()) {
        TCPSegment &seg(_sender.segments_out().front());
        if (_receiver.ackno().has_value()) {
            seg.header().ack = true;
            seg.header().ackno = _receiver.ackno().value();
            seg.header().win = _receiver.window_size();
        }
        _segments_out.emplace(seg);
        _sender.segments_out().pop();
    }
}

void TCPConnection::terminate_connection(bool send) {
    _sender.stream_in().set_error();
    _receiver.stream_out().set_error();
    _linger_after_streams_finish = false;
    if (!send) return;
    if (_sender.segments_out().empty()) _sender.send_empty_segment();
    TCPSegment &seg = _sender.segments_out().front();
    seg.header().rst = true;
    _segments_out.emplace(seg);
    _sender.segments_out().pop();
}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";
            // Your code here: need to send a RST segment to the peer
            terminate_connection(true);
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}
#include "tcp_sender.hh"

#include "tcp_config.hh"
#include <iostream>
#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity)
    , current_retransmission_timeout(_initial_retransmission_timeout)
    , closed(false) {}

uint64_t TCPSender::bytes_in_flight() const { return byte_flight; }

void TCPSender::fill_window() {
    if (closed || current_window == 0) return;
    if (_next_seqno == 0 || _stream.eof()) {
        TCPSegment segment{};
        segment.header().seqno = wrap(_next_seqno, _isn);
        if (_next_seqno == 0) {
            segment.header().syn = true;
            byte_flight++;
            current_window--;
        }
        if (_stream.eof()) {
            segment.header().fin = true;
            closed = true;
            byte_flight++;
            current_window--;
        }
        segment.payload() = string("");
        _segments_out.emplace(segment);
        outstanding_buffer.emplace(segment);
        _next_seqno++;
        return;
    }
    // end_index is the furthest index we can reach, we must consider 2 factors.
    // Then we can safely pop characters from byte stream and it also within widows size.
    size_t end_index(min(_next_seqno + current_window, _next_seqno + _stream.buffer_size()));
    while (_next_seqno < end_index) {
        TCPSegment segment{};
        // To calculate right index of the current segment, must consider 2 factors.
        size_t right(min(_next_seqno + TCPConfig::MAX_PAYLOAD_SIZE, end_index));
        segment.header().seqno = wrap(_next_seqno, _isn);
        segment.payload() = string(_stream.read(right - _next_seqno));
        if (_stream.eof()) segment.header().fin = true;
        byte_flight += segment.payload().size();
        current_window -= segment.payload().size();
        segments_out().emplace(segment);
        outstanding_buffer.emplace(segment);
        _next_seqno = right;
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
//! \returns `false` if the ackno appears invalid (acknowledges something the TCPSender hasn't sent yet)
bool TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    uint64_t ack_seqno(unwrap(ackno, _isn, _checkpoint));
    if (ack_seqno > _next_seqno) return false;
    // Reset RTO to initial value.
    current_retransmission_timeout = _initial_retransmission_timeout;
    alarm_time = 0;
    // Reset consecutive transmission to 0.
    consecutive_transmission = 0;
    // Clean outstanding buffer.
    while (!outstanding_buffer.empty()) {
        TCPSegment &segment(outstanding_buffer.front());
        // Segment data = [left, right).
        size_t left(unwrap(segment.header().seqno, _isn, _checkpoint));
        size_t right(left + segment.length_in_sequence_space());
        if (right > ack_seqno) break;
        byte_flight = byte_flight - (right - left);
        // Note windows size is 16 bits, so it always smaller than 1ll << 32.
        if (right >= _checkpoint + (1ll << 32)) {
            _checkpoint += (1ll << 32);
        }
        outstanding_buffer.pop();
    }
    // Update the current window.
    current_window = window_size;
    return true;
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    alarm_time += ms_since_last_tick;
    if (alarm_time < current_retransmission_timeout) return;
    if (!outstanding_buffer.empty()) {
        segments_out().emplace(outstanding_buffer.front());
        consecutive_transmission++;
        current_retransmission_timeout *= 2;
    }
    alarm_time = 0;
}

unsigned int TCPSender::consecutive_retransmissions() const { return consecutive_transmission; }

void TCPSender::send_empty_segment() {
    TCPSegment segment{};
    segment.header().seqno = wrap(_next_seqno, _isn);
    segment.payload() = string("");
    segments_out().emplace(segment);
}

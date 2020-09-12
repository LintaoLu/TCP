#include "tcp_receiver.hh"

#include <iostream>

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

bool TCPReceiver::segment_received(const TCPSegment &seg) {
    if (seg.header().syn) {
        if (SYN) return false;
        ISN = seg.header().seqno;
        SYN = true;
    }
    if (!SYN) return false;
    // update checkpoint
    if (_reassembler.get_offset() == 1ll << 32) checkpoint += 1ll << 32;
    // window range [start, end) is represent by stream index
    size_t start(_reassembler.get_offset()), end(start + window_size());
    // calculate absolute indices for the segment
    size_t seg_start(unwrap(seg.header().seqno, ISN, checkpoint));
    size_t seg_end(unwrap(seg.header().seqno + seg.length_in_sequence_space(), ISN, checkpoint));
    // calculate stream indices
    if (seg_start != 0) {
        seg_start--;
        seg_end--;
    }
    if (seg.header().fin) seg_end--;
    if (seg.header().fin && seg_end == 0 && !FIN) {
        _reassembler.push_substring(seg.payload().copy(), seg_start, seg.header().fin);
        return FIN = true;
    }
    //! I don't know why TCP has such design. I'll check it later.
    //! If lower out of bound and higher out of bond, return false.
    //! Otherwise (only one side is out of bound), it's acceptable....
    if (seg_start < start && seg_end > end) return false;
    if (seg_end <= start || seg_start >= end) return false;
    _reassembler.push_substring(seg.payload().copy(), seg_start, seg.header().fin);
    return true;
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (!SYN) return nullopt;
    // Must include SYN or FIN.
    if (_reassembler.stream_out().input_ended()) {
        return wrap(_reassembler.get_offset() + 2, ISN);
    }
    return wrap(_reassembler.get_offset() + 1, ISN);
}

size_t TCPReceiver::window_size() const {
    // Window size is reassembler's size.
    return _capacity - _reassembler.stream_out().buffer_size();
}

#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    // I assume uint64_t is big enough, so no overflow.
    // 1ll << 32 : 2 to the power of 32
    uint32_t num((n + isn.raw_value()) % (1ll << 32));
    return WrappingInt32{num};
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t max_len(1ll << 32);
    uint64_t offset((n - isn + max_len) % max_len);
    uint32_t coefficient(checkpoint / max_len);
    uint64_t res1(coefficient * max_len + offset);
    uint64_t res2((coefficient + 1) * max_len + offset);
    if (checkpoint % max_len == 0) {
        res2 = (coefficient == 0 ? 0 : coefficient - 1) * max_len + offset;
    }
    // abs() is not working since uint64_t a - uint64_t b is always a positive number
    // even a is smaller than b
    return res1 - checkpoint > max(res2, checkpoint) - min(res2, checkpoint) ? res2 : res1;
}

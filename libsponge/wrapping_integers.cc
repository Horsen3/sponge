#include "wrapping_integers.hh"
#include <cstdint>

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number


//将absolute seqno 转换成 seqno
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    DUMMY_CODE(n, isn);


    auto t = static_cast<uint32_t>(n) + isn.raw_value();
    return WrappingInt32{t};


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

//seqno absoult -> seqno
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    DUMMY_CODE(n, isn, checkpoint);
    uint32_t offset = n - isn;
    auto t = (checkpoint & 0xffffffff00000000) + offset;   
    auto ret = t;  
    auto INT_RANGE  = (1UL << 32);
  
    if (std::abs(static_cast<int64_t>(t + INT_RANGE - checkpoint)) < std::abs(static_cast<int64_t>(ret - checkpoint))) {
        ret = t + INT_RANGE;
    }
    if (std::abs(static_cast<int64_t>(t - INT_RANGE - checkpoint)) < std::abs(static_cast<int64_t>(ret - checkpoint)) && (t >= INT_RANGE)){

        ret = t - INT_RANGE;
    }

    return ret;

    




}


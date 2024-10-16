#include "tcp_receiver.hh"
#include <cstddef>
#include <cstdio>

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;


void TCPReceiver::segment_received(const TCPSegment &seg) {      
    DUMMY_CODE(seg);
    
   if (seg.header().syn) {
         _isn = seg.header().seqno;
        got_syn = true;
    }

    auto checkpoint = _reassembler._next_assembled_idx;

    uint64_t index = unwrap(seg.header().seqno + seg.header().syn,_isn,checkpoint) - 1 ;

    _reassembler.push_substring(seg.payload().copy(), index, seg.header().fin);

 
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (! got_syn){
        return {};
    }
    size_t ackno = _reassembler.stream_out().bytes_written() + 1;

    if (stream_out().input_ended()){
        return wrap(ackno + 1,_isn);
    }


    return {};



}

size_t TCPReceiver::window_size() const { 
    
    
    return _reassembler.stream_out().remaining_capacity();
    
}

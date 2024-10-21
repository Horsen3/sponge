#include "tcp_receiver.hh"
#include "wrapping_integers.hh"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <sys/types.h>

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;


bool TCPReceiver::segment_received(const TCPSegment &seg) {      
    DUMMY_CODE(seg);

    auto &header = seg.header();  
    auto length = seg.length_in_sequence_space();



   if (seg.header().syn) {
         _isn = seg.header().seqno;
        got_syn = true;
    }

    auto checkpoint = _reassembler._next_assembled_idx;

    auto abs_seqno  = unwrap(header.seqno, _isn, checkpoint);
    auto lower = get_window_left();
    //判断是否在窗口内
    if (abs_seqno >=lower + window_size() || (abs_seqno + length <= lower && !header.syn)){
        return false ;
    }
    auto stream_index = abs_seqno - 1 + header.syn;
    _reassembler.push_substring(seg.payload().copy(), stream_index,header.fin);
    return true;

}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (! got_syn){
        return std::nullopt;
    }
    

    return wrap(get_window_left(),_isn);
    

}

size_t TCPReceiver::window_size() const { 
    
    
    return _capacity - _reassembler.stream_out().remaining_capacity();
    
}


//get_window_left

auto TCPReceiver::get_window_left() const ->uint64_t {
    auto abs_ack_no = _reassembler._next_assembled_idx + 1;
    if (_reassembler.stream_out().input_ended()){
        abs_ack_no += 1;
    }

    return abs_ack_no;



}
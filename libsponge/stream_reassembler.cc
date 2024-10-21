#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) :_eof(false),_output(capacity), _capacity(capacity){}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
   
    //index 是文件切片的起始位置
    DUMMY_CODE(data, index, eof);
    if (eof){
        _eof = true;     
        _eof_idx = index + data.size();   //计算的是改流理应结束的位置，理想状态，正好并入，后续再更改
    }
    size_t idx = index; 
    if (idx <= _next_assembled_idx){
        if (idx + data.size() > _next_assembled_idx){
            auto content = data.substr(_next_assembled_idx - idx);
            auto write_count = _output.write(content);
            _next_assembled_idx += write_count;

            auto iter = memo.begin();
            while(iter != memo.end() && iter->first <= _next_assembled_idx){
                auto sz = iter -> second.size();
                if (iter->first + sz >= _next_assembled_idx){
                    //遍历之前存的文件区间
                    auto existed_str = iter->second.substr(_next_assembled_idx - iter->first);
                    auto add_count = _output.write(existed_str);
                    _next_assembled_idx += add_count;
                }
                _unassembled_bytes -= sz;   //未处理完的字节数
                iter = memo.erase(iter);
            }
        }
    }else{
        //与后面的memo储存的文件切片区间整合
        string s = data;
        auto iter = memo.lower_bound(idx);
        while (iter != memo.end() && iter->first <= idx + s.size()) {
            auto sz = iter->second.size();
            if (iter->first + sz > idx + s.size()) {
                s += iter->second.substr(idx + s.size() - iter->first);
            }

            _unassembled_bytes -= sz;
            iter = memo.erase(iter);

            //与前面的memo储存的文件切片区间整合
        }



        iter = memo.lower_bound(idx);
        if (iter != memo.begin() && !memo.empty()) {
            iter--;        
            auto sz = iter->second.size();
            if (iter->first + sz >= idx ) {

                if (iter->first + sz > idx +s.size()){
                    s = iter -> second;

                }else{
                    s = iter -> second + s.substr(iter->first + sz - idx);
                }
                
                _unassembled_bytes -= sz;
                idx = iter-> first;
                memo.erase(iter);
              
            }
           
        }
        
        auto store_count = std::min(s.size(), _capacity - _output.buffer_size() - _unassembled_bytes);
        _unassembled_bytes += store_count;
        memo.emplace(idx, s.substr(0, store_count));
  

            //与前面的memo储存的文件切片区间整合

    }

     if (_eof && empty()) {
        _output.end_input();
    }




}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes; }

bool StreamReassembler::empty() const { return _unassembled_bytes == 0; }

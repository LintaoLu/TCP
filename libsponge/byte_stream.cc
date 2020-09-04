#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t _capacity)
    : capacity(_capacity), all_read(0), all_write(0), buffer() { }

size_t ByteStream::write(const string &data) {
    size_t dataToWrite = min(remaining_capacity(), data.length());
    for (size_t i = 0; i < dataToWrite; i++) {
        buffer.emplace_back(data[i]);
        all_write++;
    }
    return dataToWrite;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    return string(buffer.begin(), buffer.begin() + min(len, buffer.size()));
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t dataToPop = min(len, buffer.size());
    for (size_t i = 0; i < dataToPop; i++) {
        buffer.pop_front();
        all_read++;
    }
}

void ByteStream::end_input() { input_end = true; }

bool ByteStream::input_ended() const { return input_end; }

size_t ByteStream::buffer_size() const { return buffer.size(); }

bool ByteStream::buffer_empty() const { return buffer.empty(); }

bool ByteStream::eof() const { return input_ended() && buffer_empty(); }

size_t ByteStream::bytes_written() const { return all_write; }

size_t ByteStream::bytes_read() const { return all_read; }

size_t ByteStream::remaining_capacity() const { return capacity - buffer.size(); }
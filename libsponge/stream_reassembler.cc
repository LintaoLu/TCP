#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : offset(0), _size(0), interval_tree(), _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t end(index + data.size());

}

//! end is always greater than start
void StreamReassembler::merge(size_t start, size_t end, string &data) {
    // Since I used --upper_bound() method, must handle empty tree at very beginning!
    // Otherwise --upper_bound() method will throw segmentation error!
    // I don't know if there are other way to handle this error. Will check it later.
    if (interval_tree.empty()) {
        interval_tree.insert({start, pair<size_t, string&>(end, data)});
        _size += data.size();
        return;
    }

    auto interval0 = interval_tree.find(start);
    // If input is already stored in interval tree, disregard it.
    if (interval0 != interval_tree.end() && interval0->first == start &&interval0->second.first == end) return;

    // Interval1[0] <= start, Interval2[0] <= end if they exist.
    // Must handle non-exist situations because it equals to interval_tree.begin() if not exist!
    auto interval1 = --interval_tree.upper_bound(start);
    auto interval2 = --interval_tree.upper_bound(end);

    // Must check if interval1 and interval2 exist! Note if interval1 exists, interval2 must exist!
    // First situation: interval1 exists (interval2 must exist too) and it overlaps with current interval,
    // then merge interval1, current interval and interval2 (maybe i1 i2 are the same interval, but doesn't matter).

    // Second situation: cannot merge interval1 with current interval, then try to merge interval2 with current
    // interval. The reason why cannot merge interval1 may because it doesn't exist or it doesn't overlap with
    // current interval. If interval2 exist, we just merge it.

    // Third situation: cannot merge interval1 and interval2, then insert the current interval.
    if (interval1->first <= start && start <= interval1->second.first) {
        string& str1(interval1->second.second), str2(interval2->second.second);
        string sub1(str1.substr(0, start - interval1->first));
        if (interval2->second.first > end) {
            data += str2.substr(str2.size() - (interval2->second.first - end) + 1);
        }
        for (auto it = interval1; it != interval_tree.end(); it++) {
            _size -= it->second.second.size();
            if (it == interval2) break;
        }
        // Must delete everything in between. erase() method will delete range [a, b) exclude b.
        interval_tree.erase(interval1, interval_tree.upper_bound(end));
        size_t left(interval1->first), right(max(end, interval2->second.first));
        data = sub1 + data;
        interval_tree.insert({left, pair<size_t, string&>(right, data)});
    } else if (interval2->first <= end) {
        interval1 = interval1->first > start ? interval_tree.begin() : interval_tree.upper_bound(interval1->first);
        string& str2(interval2->second.second);
        if (interval2->second.first > end) {
            data += str2.substr(str2.size() - (interval2->second.first - end));
        }
        for (auto it = interval1; it != interval_tree.end(); it++) {
            _size -= it->second.second.size();
            if (it == interval2) break;
        }
        interval_tree.erase(interval1, interval_tree.upper_bound(end));
        interval_tree.insert({start, pair<size_t, string&>(max(end, interval2->second.first), data)});
    } else {
        interval_tree.insert({start, pair<size_t, string&>(end, data)});
    }
    _size += data.size();
}

size_t StreamReassembler::unassembled_bytes() const { return _size; }

bool StreamReassembler::empty() const { return _size == 0; }



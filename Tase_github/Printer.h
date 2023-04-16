#pragma once
#ifndef _Printer_h_included
#define _Printer_h_included

#include <iostream>
#include <vector>
#include <map>

template <typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& v) {
    o << "[";
    if (v.empty()) {
        o << "]";
        return o;
    }
    // For every item except the last write "Item, "
    for (auto it = v.begin(); it != --v.end(); it++) {
        o << *it << ", ";
    }
    // Write out the last item
    o << v.back() << "]";
    return o;
}

template <typename T1, typename T2, typename T3>
std::ostream& operator<<(std::ostream& o, const std::vector<std::tuple<T1,T2,T3>>& v) {
    o << "[";
    if (v.empty()) {
        o << "]";
        return o;
    }
    // For every item except the last write "Item, "
    for (auto it = v.begin(); it != --v.end(); it++) {
        T1 value1;
        T2 value2;
        T3 value3;
        std::tie(value1, value2, value3) = *it;
        o << "(" << value1 << ", " << value2 << ", " << value3 << ")" << "\n";
    }
    // Write out the last item
    T1 value1;
    T2 value2;
    T3 value3;
    std::tie(value1, value2, value3) = v.back();
    o << "(" << value1 << ", " << value2 << ", " << value3 << ")" << "]";
    return o;
}

template <typename T>
std::ostream& operator<<(std::ostream& o, const std::set<T>& v) {
    o << "[";
    if (v.empty()) {
        o << "]";
        return o;
    }
    // For every item except the last write "Item, "
    for (auto it = v.begin(); it != --v.end(); it++) {
        o << *it << ", ";
    }
    // Write out the last item
    o << *--v.end() << "]";
    return o;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& o, const std::set < std::tuple< T1, T2 >> & v) {
    o << "[";
    if (v.empty()) {
        o << "]";
        return o;
    }
    // For every item except the last write "Item, "
    for (auto it = v.begin(); it != --v.end(); it++) {
        T1 value1;
        T2 value2;
        std::tie(value1, value2) = *it;
        o << "(" << value1 << "," << value2 << ")" << ", ";
    }
    // Write out the last item
    T1 value1;
    T2 value2;
    std::tie(value1, value2) = *--v.end();
    o << "(" << value1 << "," << value2 << ")" << "]";
    return o;
}

template <typename KeyT, typename ValueT>
std::ostream& operator<<(std::ostream& o, const std::map<KeyT, ValueT>& m) {
    o << "{";
    if (m.empty()) {
        o << "}";
        return o;
    }
    // For every pair except the last write "Key: Value, "
    for (auto it = m.begin(); it != --m.end(); it++) {
        const auto& [key, value] = *it;
        o << key << ": " << value << ", ";
    }
    // Write out the last item
    const auto& [key, value] = *--m.end();
    o << key << ": " << value << "}";
    return o;
}

template <typename KeyT1,typename KeyT2, typename KeyT3, typename ValueT>
std::ostream& operator<<(std::ostream& o, const std::map<std::tuple<KeyT1, KeyT2, KeyT3>, ValueT>& m) {
    o << "{";
    if (m.empty()) {
        o << "}";
        return o;
    }
    // For every pair except the last write "Key: Value, "
    for (auto it = m.begin(); it != --m.end(); it++) {
        KeyT1 key1;
        KeyT2 key2;
        KeyT3 key3;
        ValueT value;
        std::tie(key1, key2, key3) = it->first;
        value = it->second;
        o << "(" << key1 << "," << key2 << "," << key3 << ")" << ": " << value << ", ";
    }
    // Write out the last item
    KeyT1 key1;
    KeyT2 key2;
    KeyT3 key3;
    ValueT value;
    std::tie(key1, key2, key3) = (--m.end())->first;
    value = (--m.end())->second;
    o << "(" << key1 << "," << key2 << "," << key3 << ")" << ": " << value << "}";
    return o;
}


inline bool sortByVal(const std::tuple<std::string, int, int>& a, const std::tuple<std::string, int, int>& b) {
    return (std::get<2>(a) > std::get<2>(b));
}

inline bool sortByVal_2(const std::pair<int, float>& a, const std::pair<int, float>& b) {
    return (a.second < b.second);
}
#endif // !_Printer_h_included

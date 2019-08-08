//
// Created by marcin on 7/24/19.
//

#include "text_buffer.hpp"

std::wstring_view engine::text_buffer::range(size_t begin, size_t end) const {
  assert(begin <= end);
  assert(end <= data.size());

  return {data.data() + begin, end - begin + 1};
}

std::wstring_view engine::text_buffer::from(size_t begin) const {
  return range(begin, data.size() - 1);
}

std::wstring engine::text_buffer::str() const {
  std::wstring s(std::begin(data), std::end(data));
  return s;
}

wchar_t engine::text_buffer::operator[](size_t idx) const {
  return data[idx];
}

size_t engine::text_buffer::length() const {
  return data.size();
}

void engine::text_buffer::push(wchar_t c) {
  data.push_back(c);
}

void engine::text_buffer::truncate(size_t idx) {
  data.resize(idx);
}

void engine::text_buffer::clear() {
  data.clear();
}
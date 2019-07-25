//
// Created by marcin on 7/21/19.
//

#include "converters.h"

std::wstring engine::converters::to_wstr(const std::string &s) {
  std::wstring_convert<converter_t, wchar_t> converter;
  return converter.from_bytes(s);
}

std::wstring engine::converters::to_wstr(const char *s) {
  std::wstring_convert<converter_t, wchar_t> converter;
  return converter.from_bytes(s);
}
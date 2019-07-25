//
// Created by marcin on 7/24/19.
//

#ifndef TYPEWRITER_GAEM_TEXT_BUFFER_H
#define TYPEWRITER_GAEM_TEXT_BUFFER_H

#include <cstddef>
#include <stdexcept>
#include <assert.h>
#include <vector>
#include <string_view>

namespace engine {
class text_buffer {
public:
    text_buffer() = default;

    std::wstring_view at(size_t begin, size_t end) const;

    std::wstring_view from(size_t begin) const;

    std::wstring str() const;

    wchar_t operator[](size_t idx) const;

    size_t length() const;

    void push(wchar_t c);

    void pop();

    void clear();

private:
    std::vector<wchar_t> data;
};
};

#endif //TYPEWRITER_GAEM_TEXT_BUFFER_H

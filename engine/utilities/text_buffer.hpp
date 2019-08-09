//
// Created by marcin on 7/24/19.
//

#ifndef TYPEWRITER_GAEM_TEXT_BUFFER_HPP
#define TYPEWRITER_GAEM_TEXT_BUFFER_HPP

#include <cstddef>
#include <stdexcept>
#include <assert.h>
#include <vector>
#include <string_view>

namespace engine {
class text_buffer {
public:
    text_buffer() = default;

    std::wstring_view range(size_t begin, size_t end) const;

    std::wstring_view from(size_t begin = 0) const;

    std::wstring str() const;

    wchar_t operator[](size_t idx) const;

    size_t length() const;

    void push(wchar_t c);

    void truncate(size_t idx);

    void clear();

private:
    std::vector<wchar_t> data;
};
};

#endif //TYPEWRITER_GAEM_TEXT_BUFFER_HPP
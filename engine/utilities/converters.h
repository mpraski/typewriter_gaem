//
// Created by marcin on 7/21/19.
//

#ifndef TYPEWRITER_GAEM_CONVERTERS_H
#define TYPEWRITER_GAEM_CONVERTERS_H

#include <string>
#include <codecvt>
#include <locale>

namespace engine {
class converters {
    using converter_t = std::codecvt_utf8<wchar_t>;
public:
    static std::wstring to_wstr(const std::string &s);

    static std::wstring to_wstr(const char *s);
};
}

#endif //TYPEWRITER_GAEM_CONVERTERS_H
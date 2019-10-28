//
// Created by Marcin Praski on 28.10.19.
//

#ifndef TYPEWRITER_GAEM_FUNCTION_H
#define TYPEWRITER_GAEM_FUNCTION_H

#include <tuple>
#include <type_traits>

namespace engine::lambda_detail {
template<class Ret, class Cls, class IsMutable, class... Args>
struct types {
    using is_mutable = IsMutable;

    enum {
        arity = sizeof...(Args)
    };

    using return_type = Ret;

    template<size_t i>
    struct arg {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
    };
};

template<class Ld>
struct lambda_type
    : lambda_type<decltype(&Ld::operator())> {
};

template<class Ret, class... Args>
struct lambda_type<Ret(*)(Args...)>
    : lambda_detail::types<Ret, std::true_type, std::true_type, Args...> {
};

template<class Ret, class Cls, class... Args>
struct lambda_type<Ret(Cls::*)(Args...)>
    : lambda_detail::types<Ret, Cls, std::true_type, Args...> {
};

template<class Ret, class Cls, class... Args>
struct lambda_type<Ret(Cls::*)(Args...) const>
    : lambda_detail::types<Ret, Cls, std::false_type, Args...> {
};
}

#endif //TYPEWRITER_GAEM_FUNCTION_H

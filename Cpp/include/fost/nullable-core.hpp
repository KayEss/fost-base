#ifndef FOST_NULLABLE_CORE_HPP
#define FOST_NULLABLE_CORE_HPP
#pragma once


#include <fost/null.hpp>

#include <ostream>


namespace fostlib {


    namespace detail {


        [[noreturn]] void throw_null_exception();


    }


    template<typename T>
    using nullable = std::optional<T>;


}


/// Allow nullable values to be printed
namespace std {
    template<typename C, typename T, typename Y>
    requires requires(Y const t, basic_ostream<C, T> os) { {os << t}; }
    auto &operator<<(basic_ostream<C, T> &o, const optional<Y> &y) {
        return y ? o << *y : o << std::nullopt;
    }
}


#endif // FOST_NULLABLE_HPP

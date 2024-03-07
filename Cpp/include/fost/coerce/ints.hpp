#ifndef FOST_COERCE_INTS_HPP
#define FOST_COERCE_INTS_HPP
#pragma once


#include <fost/coerce.hpp>
#include <fost/exception/out_of_range.hpp>

#include <limits>
#include <type_traits>


namespace fostlib {


    /**
     * ## Coerce between integral types
     */
    template<typename T, typename F>
        requires(not is_brace_constructible<T, F>)
            and std::is_integral_v<T> and std::is_integral_v<F>
    struct coercer<T, F> {
        using limits_T = std::numeric_limits<T>;
        using limits_F = std::numeric_limits<F>;

        T
                coerce(F f,
                       felspar::source_location loc =
                               felspar::source_location::current()) const {
            if constexpr (std::is_same_v<T, bool>) {
                return f ? true : false;
            } else {
                if constexpr (limits_T::is_signed == limits_F::is_signed) {
                    if constexpr (sizeof(T) < sizeof(F)) {
                        if (f < limits_T::min()) {
                            throw fostlib::exceptions::out_of_range<T, F>{
                                    limits_T::min(), limits_T::max(), f,
                                    std::move(loc)};
                        }
                        if (f > limits_T::max()) {
                            throw fostlib::exceptions::out_of_range<T, F>{
                                    limits_T::min(), limits_T::max(), f,
                                    std::move(loc)};
                        }
                    }
                } else if constexpr (limits_T::is_signed) {
                    if constexpr (sizeof(T) <= sizeof(F)) {
                        if (f > limits_T::max()) {
                            throw fostlib::exceptions::out_of_range<T, F>{
                                    limits_T::min(), limits_T::max(), f,
                                    std::move(loc)};
                        }
                    }
                } else if constexpr (limits_F::is_signed) {
                    if (f < 0) {
                        throw fostlib::exceptions::out_of_range<T, F>{
                                limits_T::min(), limits_T::max(), f,
                                std::move(loc)};
                    }
                    if constexpr (sizeof(T) < sizeof(F)) {
                        if (f > F{limits_T::max()}) {
                            throw fostlib::exceptions::out_of_range<T, F>{
                                    limits_T::min(), limits_T::max(), f,
                                    std::move(loc)};
                        }
                    }
                }
                return T(f);
            }
        }
    };


}


#endif // FOST_COERCE_INTS_HPP

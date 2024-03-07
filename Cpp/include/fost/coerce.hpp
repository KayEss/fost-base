#ifndef FOST_COERCE_HPP
#define FOST_COERCE_HPP
#pragma once


#include <fost/null.hpp>

#include <felspar/test/source.hpp>

#include <exception>


namespace fostlib {


    /// Can be used to detect when a type conversion can be done through strict
    /// construction.
    template<typename T, typename F>
    concept is_brace_constructible = requires(F const f) {
        { T{f} };
    };

    /// ## Coercion helper
    /**
     * This struct can be partially specialised to handle whole classes of
     * coercions
     */
    template<typename T, typename F, typename Enable = void>
    struct coercer;

    /// Can be used to detect whether the coercion struct is able to accept a
    /// source file location or not.
    template<typename T, typename F>
    concept coercion_wants_source = requires(coercer<T, F, void> c) {
        {
            c.coerce(
                    std::declval<F>(), std::declval<felspar::source_location>())
        };
    };


    template<typename T, typename F>
    inline T coerce(
            F &&f,
            felspar::source_location loc = felspar::source_location::current()) {
        using from_type = std::decay_t<F>;
        if constexpr (coercion_wants_source<T, from_type>) {
            return coercer<T, from_type>{}.coerce(
                    std::forward<F>(f), std::move(loc));
        } else {
            return coercer<T, from_type>{}.coerce(std::forward<F>(f));
        }
    }


    /// Coercion for brace constructible types
    template<typename T, typename F>
        requires is_brace_constructible<T, F>
    struct coercer<T, F, void> {
        T coerce(
                F const &f,
                felspar::source_location = felspar::source_location::current()) {
            return T{f};
        }
        T coerce(
                F &&f,
                felspar::source_location = felspar::source_location::current()) {
            return T{std::move(f)};
        }
    };


}


#endif // FOST_COERCE_HPP

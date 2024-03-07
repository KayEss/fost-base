#ifndef FOST_COERCE_NULLABLE_HPP
#define FOST_COERCE_NULLABLE_HPP
#pragma once


#include <fost/nullable.hpp>
#include <fost/coerce.hpp>
#include <fost/exception/null.hpp>

#include <felspar/memory/holding_pen.hpp>


namespace fostlib {


    template<typename T, typename F>
    struct coercer<T, std::optional<F>> {
        template<typename A>
        auto coerce(A &&f, felspar::source_location loc) {
            if (not f) {
                throw exceptions::null{loc};
            } else {
                return fostlib::coerce<T>(*std::forward<A>(f), std::move(loc));
            }
        }
    };
    template<typename T, typename F>
    struct coercer<std::optional<T>, F> {
        template<typename A>
        std::optional<T> coerce(A &&f, felspar::source_location loc) {
            return fostlib::coerce<T>(std::forward<A>(f), std::move(loc));
        }
    };
    template<typename T, typename F>
    struct coercer<std::optional<T>, std::optional<F>> {
        template<typename A>
        std::optional<T> coerce(A &&f, felspar::source_location loc) {
            if (not f) {
                return std::nullopt;
            } else {
                return fostlib::coerce<T>(*std::forward<A>(f), std::move(loc));
            }
        }
    };


    template<typename T, typename F>
    struct coercer<T, felspar::memory::holding_pen<F>> {
        template<typename A>
        auto coerce(A &&f, felspar::source_location loc) {
            if (not f) {
                throw exceptions::null{loc};
            } else {
                return fostlib::coerce<T>(*std::forward<A>(f), std::move(loc));
            }
        }
    };
    template<typename T, typename F>
    struct coercer<felspar::memory::holding_pen<T>, F> {
        template<typename A>
        felspar::memory::holding_pen<T>
                coerce(A &&f, felspar::source_location loc) {
            return fostlib::coerce<T>(std::forward<A>(f), std::move(loc));
        }
    };
    template<typename T, typename F>
    struct coercer<
            felspar::memory::holding_pen<T>,
            felspar::memory::holding_pen<F>> {
        template<typename A>
        felspar::memory::holding_pen<T>
                coerce(A &&f, felspar::source_location loc) {
            if (not f) {
                return std::nullopt;
            } else {
                return fostlib::coerce<T>(*std::forward<A>(f), std::move(loc));
            }
        }
    };


}


#endif // FOST_COERCE_NULLABLE_HPP

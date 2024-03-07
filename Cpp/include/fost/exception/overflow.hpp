#ifndef FOST_EXCEPTION_OVERFLOW_HPP
#define FOST_EXCEPTION_OVERFLOW_HPP
#pragma once


#include <fost/exception.hpp>
#include <fost/detail/coerce.hpp>


namespace fostlib {


    namespace exceptions {


        template<typename T>
        class overflow;

        template<>
        class FOST_CORE_DECLSPEC overflow<string> : public exception {
          public:
            overflow(
                    const string &message,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;
            overflow(
                    const string &message,
                    const string &n,
                    const string &d,
                    const string &m,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;
            overflow(
                    const string &n,
                    const string &d,
                    const string &m,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };

        template<typename T>
        class overflow : public overflow<string> {
          public:
            overflow(
                    const string &msg,
                    const T n,
                    const T d,
                    const T m,
                    felspar::source_location sl =
                            felspar::source_location::current()) noexcept
            : overflow<string>{
                    msg, coerce<string>(n), coerce<string>(d),
                    coerce<string>(m), std::move(sl)} {}
            overflow(
                    const T n,
                    const T d,
                    const T m,
                    felspar::source_location sl =
                            felspar::source_location::current()) noexcept
            : overflow<string>{
                    coerce<string>(n), coerce<string>(d), coerce<string>(m),
                    std::move(sl)} {}
        };


    }


}


#endif // FOST_EXCEPTION_OVERFLOW_HPP

#ifndef FOST_EXCEPTION_OUT_OF_RANGE_HPP
#define FOST_EXCEPTION_OUT_OF_RANGE_HPP
#pragma once


#include <fost/exception.hpp>
#include <fost/detail/coerce.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC out_of_range_string : public exception {
          public:
            out_of_range_string(
                    const string &min,
                    const string &max,
                    const string &value,
                    std::source_location =
                            std::source_location::current()) noexcept;
            out_of_range_string(
                    const string &message,
                    const string &min,
                    const string &max,
                    const string &value,
                    std::source_location =
                            std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


        template<typename T, typename U = T>
        class out_of_range : public out_of_range_string {
          public:
            out_of_range(
                    const T min,
                    const T max,
                    const U value,
                    std::source_location sl =
                            std::source_location::current()) noexcept
            : out_of_range_string{
                      coerce<string>(min), coerce<string>(max),
                      coerce<string>(value), std::move(sl)} {}
            out_of_range(
                    const string &message,
                    const T min,
                    const T max,
                    const U value,
                    std::source_location sl =
                            std::source_location::current()) noexcept
            : out_of_range_string{
                      message, coerce<string>(min), coerce<string>(max),
                      coerce<string>(value), std::move(sl)} {}
        };


    }


}


#endif // FOST_EXCEPTION_OUT_OF_RANGE_HPP

#ifndef FOST_EXCEPTION_NULL_HPP
#define FOST_EXCEPTION_NULL_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC null : public exception {
          public:
            null(std::source_location =
                         std::source_location::current()) noexcept;
            null(const string &message,
                 std::source_location =
                         std::source_location::current()) noexcept;
            null(const string &message,
                 const string &info,
                 std::source_location =
                         std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_NULL_HPP

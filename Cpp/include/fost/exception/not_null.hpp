#ifndef FOST_EXCEPTION_NOT_NULL_HPP
#define FOST_EXCEPTION_NOT_NULL_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC not_null : public exception {
          public:
            not_null(
                    std::source_location =
                            std::source_location::current()) noexcept;
            not_null(
                    const string &message,
                    std::source_location =
                            std::source_location::current()) noexcept;
            not_null(
                    const string &message,
                    const string &info,
                    std::source_location =
                            std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_NOT_NULL_HPP

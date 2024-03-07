#ifndef FOST_EXCEPTION_NULL_HPP
#define FOST_EXCEPTION_NULL_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC null : public exception {
          public:
            null(felspar::source_location =
                         felspar::source_location::current()) noexcept;
            null(const string &message,
                 felspar::source_location =
                         felspar::source_location::current()) noexcept;
            null(const string &message,
                 const string &info,
                 felspar::source_location =
                         felspar::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_NULL_HPP

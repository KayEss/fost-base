#ifndef FOST_EXCEPTION_NOT_UNIQUE_HPP
#define FOST_EXCEPTION_NOT_UNIQUE_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC not_unique : public exception {
          public:
            not_unique(
                    const string &error,
                    std::source_location =
                            std::source_location::current()) noexcept;
            not_unique(
                    const string &error,
                    const string &context,
                    const string &alternative1,
                    const string &alternative2,
                    std::source_location =
                            std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_NOT_UNIQUE_HPP

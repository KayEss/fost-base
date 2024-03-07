#ifndef FOST_EXCEPTION_NOT_A_NUMBER_HPP
#define FOST_EXCEPTION_NOT_A_NUMBER_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC not_a_number : public exception {
          public:
            not_a_number(
                    const string &message,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;
            not_a_number(
                    const string &message,
                    const string &value,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_NOT_A_NUMBER_HPP

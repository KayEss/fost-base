#ifndef FOST_EXCEPTION_UNEXPECTED_NIL_HPP
#define FOST_EXCEPTION_UNEXPECTED_NIL_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC unexpected_nil : public exception {
          public:
            unexpected_nil(
                    std::source_location =
                            std::source_location::current()) noexcept;
            unexpected_nil(
                    const string &,
                    std::source_location =
                            std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_UNEXPECTED_NIL_HPP

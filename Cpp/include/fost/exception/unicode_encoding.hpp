#ifndef FOST_EXCEPTION_UNICODE_ENCODING_HPP
#define FOST_EXCEPTION_UNICODE_ENCODING_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC unicode_encoding : public exception {
          public:
            unicode_encoding(
                    const string &message,
                    std::source_location =
                            std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_UNICODE_ENCODING_HPP

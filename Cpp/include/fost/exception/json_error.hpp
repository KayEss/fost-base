#ifndef FOST_EXCEPTION_JSON_ERROR_HPP
#define FOST_EXCEPTION_JSON_ERROR_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC json_error : public exception {
          public:
            json_error(
                    const string &message,
                    std::source_location =
                            std::source_location::current()) noexcept;
            json_error(
                    const string &message,
                    const json &value,
                    std::source_location =
                            std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_JSON_ERROR_HPP

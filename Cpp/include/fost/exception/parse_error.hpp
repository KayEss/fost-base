#ifndef FOST_EXCEPTION_PARSE_ERROR_HPP
#define FOST_EXCEPTION_PARSE_ERROR_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC parse_error : public exception {
          public:
            parse_error(
                    string const &message,
                    std::source_location =
                            std::source_location::current()) noexcept;
            parse_error(
                    string const &,
                    const string &value,
                    std::source_location =
                            std::source_location::current()) noexcept;
            parse_error(
                    string const &,
                    std::size_t line,
                    std::size_t col,
                    std::source_location = std::source_location::current());
            parse_error(
                    string const &,
                    std::filesystem::path const &filename,
                    std::size_t line,
                    std::size_t col,
                    std::source_location = std::source_location::current());

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_PARSE_ERROR_HPP

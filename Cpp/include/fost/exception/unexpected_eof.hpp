#ifndef FOST_EXCEPTION_UNEXPECTED_EOF_HPP
#define FOST_EXCEPTION_UNEXPECTED_EOF_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC unexpected_eof : public exception {
          public:
            unexpected_eof(
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;
            unexpected_eof(
                    const string &message,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;
            unexpected_eof(
                    const string &message,
                    const string &filename,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;
            unexpected_eof(
                    const string &message,
                    std::error_code,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_UNEXPECTED_EOF_HPP

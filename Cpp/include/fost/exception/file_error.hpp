#ifndef FOST_EXCEPTION_FILE_ERROR_HPP
#define FOST_EXCEPTION_FILE_ERROR_HPP
#pragma once


#include <fost/exception.hpp>
#include <fost/detail/coerce.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC file_error : public exception {
          public:
            /// Throw an error about the given filename
            file_error(
                    const string &message,
                    const string &filename,
                    std::source_location =
                            std::source_location::current()) noexcept;
            /// Thow an error from an error code
            file_error(
                    string const &message,
                    std::filesystem::path const &,
                    std::error_code,
                    std::source_location =
                            std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_FILE_ERROR_HPP

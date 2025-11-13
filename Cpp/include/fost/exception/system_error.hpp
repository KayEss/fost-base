#pragma once


#include <fost/exception.hpp>
#include <system_error>


namespace fostlib::exceptions {


    class system_error : public exception {
      public:
        system_error(
                felspar::u8view message,
                std::source_location = std::source_location::current()) noexcept;
        system_error(
                int ev,
                std::error_category const &,
                felspar::u8view message,
                std::source_location = std::source_location::current()) noexcept;

      protected:
        felspar::u8view message() const noexcept;
    };


}

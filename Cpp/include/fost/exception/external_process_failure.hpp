#ifndef FOST_EXCEPTION_EXTERNAL_PROCESS_FAILURE_HPP
#define FOST_EXCEPTION_EXTERNAL_PROCESS_FAILURE_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC external_process_failure : public exception {
          public:
            external_process_failure(
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;
            external_process_failure(
                    const string &message,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_EXTERNAL_PROCESS_FAILURE_HPP

#ifndef FOST_EXCEPTION_CAST_FAULT_HPP
#define FOST_EXCEPTION_CAST_FAULT_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC cast_fault : public exception {
          public:
            cast_fault(
                    const string &message,
                    std::source_location =
                            std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_CAST_FAULT_HPP

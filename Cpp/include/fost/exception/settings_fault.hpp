#ifndef FOST_EXCEPTION_SETTINGS_FAULT_HPP
#define FOST_EXCEPTION_SETTINGS_FAULT_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC settings_fault : public exception {
          public:
            settings_fault(
                    const string &error,
                    const string &domain,
                    const string &section,
                    const string &name,
                    const string &value,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_SETTINGS_FAULT_HPP

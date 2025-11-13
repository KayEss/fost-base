#ifndef FOST_EXCEPTION_MISSING_SETTING_HPP
#define FOST_EXCEPTION_MISSING_SETTING_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC missing_setting : public exception {
          public:
            missing_setting(
                    const string &section,
                    const string &name,
                    std::source_location =
                            std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_MISSING_SETTING_HPP

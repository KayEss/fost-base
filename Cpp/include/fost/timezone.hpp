#ifndef FOST_TIMEZONE_HPP
#define FOST_TIMEZONE_HPP
#pragma once


#include <fost/string.hpp>


namespace fostlib {


    class FOST_CORE_DECLSPEC zoneinfo {
      public:
        zoneinfo();

        accessors<string> name;
    };


}


namespace std {


    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, fostlib::zoneinfo t) {
        return o << t.name();
    }


}


#endif // FOST_TIMEZONE_HPP

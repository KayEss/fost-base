#pragma once


#ifndef FOST_DYNLIB_HPP
#define FOST_DYNLIB_HPP


#include <memory>


namespace fostlib {


    class FOST_CORE_DECLSPEC dynlib final {
        struct impl;
        std::unique_ptr<impl> m_lib;

      public:
        dynlib(const string &path);
        ~dynlib();

        /// Not copyable
        dynlib(const dynlib &) = delete;
        dynlib &operator=(const dynlib &) = delete;
    };


}


#endif // FOST_DYNLIB_HPP

#ifndef FOST_LOG_MESSAGE_HPP
#define FOST_LOG_MESSAGE_HPP
#pragma once


#include <fost/module.hpp>
#include <fost/timestamp.hpp>


namespace fostlib {


    namespace log {


        /// The logging messages themselves
        struct FOST_CORE_DECLSPEC message {
            /// Create a message from this data
            message(fostlib::module const &m,
                    std::size_t l,
                    felspar::u8string n,
                    json j)
            : when{std::chrono::system_clock::now()},
              level{l},
              name{std::move(n)},
              body{std::move(j)},
              m_module{&m} {}

            /// When the message was recorded
            std::chrono::system_clock::time_point when;
            /// The module name that the message is for
            fostlib::module const &module() const { return *m_module; }
            /// The level of the logging message
            std::size_t level;
            /// The name of the logging message
            felspar::u8string name;
            /// The body data of the logging message
            json body;

          private:
            fostlib::module const *m_module;
        };


    }


    /// Allow us to turn a logging::message into a JSON blob
    template<>
    struct FOST_CORE_DECLSPEC coercer<json, fostlib::log::message> {
        json coerce(const fostlib::log::message &);
    };


}


#endif // FOST_LOG_MESSAGE_HPP

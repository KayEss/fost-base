#ifndef FOST_TIMESTAMP_HPP
#define FOST_TIMESTAMP_HPP
#pragma once


#include <fost/date.hpp>
#include <fost/time.hpp>
#include <fost/timediff.hpp>
#include <fost/utility-nullable.hpp>
#include <fost/file.hpp>

#include <chrono>
#include <utility>


namespace fostlib {


    /// This date format is used in emails and HTTP
    struct FOST_CORE_DECLSPEC rfc1123_timestamp_tag {
        static void do_encode(fostlib::nliteral from, ascii_string &into);
        static void do_encode(const ascii_string &from, ascii_string &into);
        static void check_encoded(const ascii_string &s);
    };
    using rfc1123_timestamp =
            tagged_string<rfc1123_timestamp_tag, ascii_string>;

    /// Turn a timestamp into a (near) ISO formatted string for UTC
    template<>
    struct FOST_CORE_DECLSPEC
            coercer<string, std::chrono::system_clock::time_point> {
        /// The default string format is ISO with the 'T' separator exchanged
        /// for a space
        string coerce(std::chrono::system_clock::time_point);
    };
    /// Turns an ISO formatted time stamp string into a timestamp
    template<>
    struct FOST_CORE_DECLSPEC
            coercer<std::chrono::system_clock::time_point, string> {
        /// The default string format is ISO with the 'T' separator exchanged
        /// for a space
        std::chrono::system_clock::time_point coerce(felspar::u8view);
    };

    /// Coerce a timestamp to the standard date format used in SMTP, HTTP etc.
    template<>
    struct FOST_CORE_DECLSPEC
            coercer<rfc1123_timestamp, std::chrono::system_clock::time_point> {
        rfc1123_timestamp coerce(std::chrono::system_clock::time_point);
    };

    /// Parse a timestamp in the common HTTP/SMTP format to a timestamp
    template<>
    struct FOST_CORE_DECLSPEC
            coercer<std::chrono::system_clock::time_point, rfc1123_timestamp> {
        std::chrono::system_clock::time_point coerce(rfc1123_timestamp const &);
    };

    /// We need JSON as well if we are to be able to use this for database fields
    template<>
    struct FOST_CORE_DECLSPEC
            coercer<std::chrono::system_clock::time_point, json> {
        auto coerce(const json &ts) {
            return fostlib::coerce<std::chrono::system_clock::time_point>(
                    fostlib::coerce<string>(ts));
        }
    };
    template<>
    struct coercer<json, std::chrono::system_clock::time_point> {
        json coerce(std::chrono::system_clock::time_point t) {
            return json{fostlib::coerce<string>(t)};
        }
    };

    /// Get a format suitable for use in file names
    template<>
    struct coercer<std::filesystem::path, std::chrono::system_clock::time_point> {
        std::filesystem::path
                coerce(const std::chrono::system_clock::time_point &ts) {
            string s(fostlib::coerce<string>(ts));
            return fostlib::coerce<std::filesystem::path>(replace_all(s, ":"));
        }
    };


}


namespace std {


    inline auto &operator<<(ostream &o, chrono::system_clock::time_point t) {
        return o << fostlib::coerce<fostlib::string>(t);
    }


}


#endif // FOST_TIMESTAMP_HPP

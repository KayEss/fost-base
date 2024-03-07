#ifndef FOST_DETAIL_DATE_HPP
#define FOST_DETAIL_DATE_HPP
#pragma once


#include <fost/rel_ops>
#include <fost/string.hpp>
#include <fost/timediff.hpp>

#include <compare>


namespace fostlib {


    std::chrono::system_clock::time_point timestamp(
            unsigned year,
            unsigned month,
            unsigned day,
            unsigned hour = 0,
            unsigned minute = 0,
            unsigned seconds = 0,
            unsigned us = 0);


    /// A date in the Gregorian calendar
    class FOST_CORE_DECLSPEC date {
        days m_date;

      public:
        /// Construct an empty date
        constexpr date() : m_date{} {}
        /// Pull the date part out of a timestamp
        constexpr explicit date(std::chrono::system_clock::time_point const t)
        : m_date{std::chrono::duration_cast<days>(t.time_since_epoch())} {}
        /// Construct a date from year, month and day values
        date(unsigned year, unsigned month, unsigned day)
        : date{timestamp(year, month, day)} {}

        /// Today (UTC)
        static date today();

        /// Compare dates for equality
        friend constexpr bool operator==(date l, date r) noexcept {
            return l.m_date == r.m_date;
        }
        friend constexpr auto operator<=>(date l, date r) noexcept {
            return l.m_date.count() <=> r.m_date.count();
        }

        constexpr std::time_t to_time_t() const noexcept {
            return 60 * 60 * 24 * m_date.count();
        }
    };


    /// Allow a date to be coerced to a string
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, date> {
        /// Performs the coercions
        string coerce(date);
    };

    /// Allow a date to be coerced to a JSON representation
    template<>
    struct coercer<json, date> {
        /// Performs the coercions
        json coerce(date d) { return json(fostlib::coerce<string>(d)); }
    };
    /// Allow a date to be coerced from a JSON representation
    template<>
    struct FOST_CORE_DECLSPEC coercer<date, json> {
        /// Performs the coercions
        date coerce(const json &);
    };


}


namespace std {


    /// Allow a date to be output to a stream
    inline std::ostream &operator<<(std::ostream &o, fostlib::date d) {
        return o << fostlib::coerce<fostlib::string>(d);
    }


}


#endif // FOST_DETAIL_DATE_HPP

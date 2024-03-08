#include "fost-core.hpp"
#include <fost/exception/parse_error.hpp>
#include <fost/log.hpp>
#include <fost/parse/timestamp.hpp>
#include <fost/timestamp.hpp>
#include <fost/unicode.hpp>

#include <time.h>


/**
 * ## `timestamp`
 */


namespace {
    auto check_range(unsigned v, unsigned low, unsigned max) {
        if (v < low or v > max) {
            throw std::runtime_error{
                    std::to_string(v)
                    + (v < low ? (" < " + std::to_string(low))
                               : (" > " + std::to_string(max)))};
        } else {
            return v;
        }
    }
}
std::chrono::system_clock::time_point fostlib::timestamp(
        unsigned const year,
        unsigned const month,
        unsigned const day,
        unsigned const hour,
        unsigned const minute,
        unsigned const seconds,
        unsigned const us) {
    std::tm t{};
    t.tm_sec = check_range(seconds, 0, 60);
    t.tm_min = check_range(minute, 0, 59);
    t.tm_hour = check_range(hour, 0, 23);
    t.tm_mday = check_range(day, 1, 31);
    t.tm_mon = check_range(month, 1, 12) - 1;
    t.tm_year = check_range(year, 1900, 9999) - 1900;
    std::time_t const time = ::timegm(&t);
    return std::chrono::system_clock::time_point{
            std::chrono::seconds{time} + std::chrono::microseconds{us}};
}


/**
 * ## `date`
 */


fostlib::string
        fostlib::coercer<fostlib::string, fostlib::date>::coerce(date d) {
    std::time_t const tt{d.to_time_t()};
    std::tm *const t = std::gmtime(&tt);
    std::stringstream ss;
    ss << (1900 + t->tm_year) << '-' << std::setfill('0') << std::setw(2)
       << (1 + t->tm_mon) << '-' << std::setw(2) << t->tm_mday;
    return {ss.str()};
}


fostlib::date
        fostlib::coercer<fostlib::date, fostlib::json>::coerce(const json &j) {
    auto const s = j.get<felspar::u8view>();
    if (not s)
        throw exceptions::parse_error{
                "With non-string JSON", json::unparse(j, true)};
    std::tm t{};
    std::istringstream ss{std::string{*s}};
    ss >> std::get_time(&t, "%Y-%m-%d");
    if (ss.fail()) {
        throw exceptions::parse_error{"Not a date format", *s};
    } else {
        return date{
                fostlib::coerce<unsigned>(t.tm_year + 1900),
                fostlib::coerce<unsigned>(t.tm_mon + 1),
                fostlib::coerce<unsigned>(t.tm_mday)};
    }
}


/**
 * ## `std::chrono::system_clock::time_point`
 */


fostlib::string fostlib::
        coercer<fostlib::string, std::chrono::system_clock::time_point>::coerce(
                std::chrono::system_clock::time_point const t) {
    auto const seconds = std::chrono::floor<std::chrono::seconds>(t);
    auto const sec_part = t - seconds;
    auto const us =
            std::chrono::duration_cast<std::chrono::microseconds>(sec_part)
                    .count();
    auto const tt = std::chrono::system_clock::to_time_t(t);

    std::stringstream ss;
    if (us) {
        ss << std::put_time(std::gmtime(&tt), "%FT%T.");
        ss.width(6);
        ss.fill('0');
        ss << us << 'Z';
    } else {
        ss << std::put_time(std::gmtime(&tt), "%FT%T") << 'Z';
    }

    return string{ss.str()};
}


std::chrono::system_clock::time_point fostlib::coercer<
        std::chrono::system_clock::time_point,
        fostlib::string>::coerce(felspar::u8view s) {
    std::tm t{};
    std::int64_t us{};
    std::istringstream ss{std::string{s}};
    switch (s.memory().size()) {
    case 10: ss >> std::get_time(&t, "%Y-%m-%d"); break;
    case 16: [[fallthrough]];
    case 17: ss >> std::get_time(&t, "%Y-%m-%dT%H:%M"); break;
    case 19: [[fallthrough]];
    case 20: ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%SZ"); break;
    case 26: [[fallthrough]];
    case 27: ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S.") >> us; break;
    default: throw exceptions::parse_error{"Not a date format length", s};
    }
    if (ss.fail()) {
        throw exceptions::parse_error{"Not a date format", s};
    } else {
        std::time_t const time = ::timegm(&t);
        return std::chrono::system_clock::time_point{
                std::chrono::seconds{time} + std::chrono::microseconds{us}};
    }
}


/**
 * ## `fostlib::rfc1123_timestamp`
 */


void fostlib::rfc1123_timestamp_tag::do_encode(
        fostlib::nliteral, ascii_string &) {
    throw exceptions::not_implemented(
            "fostlib::rfc1123_timestamp_tag::do_encode( fostlib::nliteral "
            "from, ascii_string &into )");
}


void fostlib::rfc1123_timestamp_tag::do_encode(
        const ascii_string &, ascii_string &) {
    throw exceptions::not_implemented(
            "fostlib::rfc1123_timestamp_tag::do_encode( const ascii_string "
            "&from, ascii_string &into )");
}


void fostlib::rfc1123_timestamp_tag::check_encoded(const ascii_string &s) {
    if (s.memory().size() != 31u) {
        exceptions::out_of_range<std::size_t> err{
                "RFC 1123 date format must be 31 characters", 31u, 31u,
                s.memory().size()};
        fostlib::insert(err.data(), "string", s);
        throw err;
    }
}


fostlib::rfc1123_timestamp fostlib::coercer<
        fostlib::rfc1123_timestamp,
        std::chrono::system_clock::time_point>::
        coerce(std::chrono::system_clock::time_point tp) {
    std::time_t const tm = std::chrono::system_clock::to_time_t(tp);
    std::tm *t = std::gmtime(&tm);
    std::stringstream ss;
    ss << std::put_time(t, "%a, %d %b %Y %H:%M:%S +0000");
    return {ss.str()};
}


std::chrono::system_clock::time_point fostlib::coercer<
        std::chrono::system_clock::time_point,
        fostlib::rfc1123_timestamp>::coerce(rfc1123_timestamp const &ts) {
    std::chrono::system_clock::time_point ret{};
    auto pos = ts.underlying().underlying().begin();
    const auto end = ts.underlying().underlying().end();
    rfc1123_timestamp_parser<decltype(pos)> rule;
    if (boost::spirit::qi::phrase_parse(
                pos, end, rule, boost::spirit::qi::space, ret)
        && pos == end) {
        return ret;
    } else {
        throw exceptions::parse_error(
                "Parsing an RFC1123 time stamp", string(pos, end));
    }
}

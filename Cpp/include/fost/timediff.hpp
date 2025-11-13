#ifndef FOST_TIMEDIFF_HPP
#define FOST_TIMEDIFF_HPP
#pragma once


#include <fost/json.hpp>


namespace fostlib {


    using days = std::chrono::duration<int, std::ratio<86400>>;


    namespace detail {
        template<typename Rb, typename Pb, typename Rv, typename Pv>
        void time_json_cast(
                std::chrono::duration<Rb, Pb> &b,
                json::object_t &ret,
                felspar::u8view n,
                std::chrono::duration<Rv, Pv> v) {
            if (v.count()) { ret[n] = v.count(); }
            b -= v;
        }
    }


    template<>
    struct coercer<json, std::chrono::seconds> {
        json coerce(std::chrono::seconds s) {
            json::object_t ret;
            detail::time_json_cast(
                    s, ret, "days", std::chrono::duration_cast<days>(s));
            detail::time_json_cast(
                    s, ret, "hours",
                    std::chrono::duration_cast<std::chrono::hours>(s));
            detail::time_json_cast(
                    s, ret, "minutes",
                    std::chrono::duration_cast<std::chrono::minutes>(s));
            if (s.count() or ret.empty()) { ret["seconds"] = s.count(); }
            return ret;
        }
    };
    template<>
    struct coercer<json, std::chrono::microseconds> {
        json coerce(std::chrono::microseconds us) {
            json::object_t ret;
            detail::time_json_cast(
                    us, ret, "days", std::chrono::duration_cast<days>(us));
            detail::time_json_cast(
                    us, ret, "hours",
                    std::chrono::duration_cast<std::chrono::hours>(us));
            detail::time_json_cast(
                    us, ret, "minutes",
                    std::chrono::duration_cast<std::chrono::minutes>(us));
            if (us.count() or ret.empty()) {
                ret["seconds"] =
                        static_cast<double>(us.count()) / std::micro::den;
            }
            return ret;
        }
    };


    template<typename R, typename P>
    struct FOST_CORE_DECLSPEC coercer<std::chrono::duration<R, P>, json> {
        auto coerce(
                json const &v,
                std::source_location loc = std::source_location::current()) {
            if (v.isatom()) {
                return std::chrono::duration<R, P>{
                        fostlib::coerce<std::int64_t>(v, loc)};
            } else {
                std::chrono::duration<R, P> ret{};
                ret += std::chrono::days{
                        fostlib::coerce<std::optional<std::int64_t>>(
                                v["days"], loc)
                                .value_or(0)};
                ret += std::chrono::hours{
                        fostlib::coerce<std::optional<std::int64_t>>(
                                v["hours"], loc)
                                .value_or(0)};
                if constexpr (std::ratio_less_equal_v<
                                      P, std::chrono::minutes::period>) {
                    ret += std::chrono::minutes{
                            fostlib::coerce<std::optional<std::int64_t>>(
                                    v["minutes"], loc)
                                    .value_or(0)};
                    if constexpr (std::ratio_less_equal_v<
                                          P, std::chrono::microseconds::period>) {
                        ret += std::chrono::microseconds{std::int64_t(
                                fostlib::coerce<std::optional<double>>(
                                        v["seconds"], loc)
                                        .value_or(0)
                                * std::micro::den)};
                    }
                }
                return ret;
            }
        }
    };


}


#endif // FOST_TIMEDIFF_HPP

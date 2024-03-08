#pragma once


#include <fost/coerce.hpp>
#include <fost/exception.hpp>
#include <fost/filesystem.hpp>

#include <sstream>


namespace fostlib {


    class json;


    /// Convert a Boost error to JSON
    template<>
    struct coercer<json, std::error_code> {
        json coerce(std::error_code e) { return json(e.message().c_str()); }
    };


    /// In all cases, we can treat a `u8string` source as a `u8view`
    template<typename T>
    struct coercer<T, felspar::u8string> {
        T coerce(felspar::u8view s) {
            return fostlib::coercer<T, felspar::u8view>{}.coerce(s);
        }
    };


    /// Coerce fostlib::exceptions::exception instances to a string
    template<typename E>
    struct coercer<
            string,
            E,
            std::enable_if_t<std::is_base_of_v<exceptions::exception, E>>> {
        string coerce(const E &e) {
            std::stringstream ss;
            e.printOn(ss);
            return string(ss.str());
        }
    };
    /// Coerce fostlib::exceptions::exception instances to json
    template<typename E>
    struct coercer<
            json,
            E,
            std::enable_if_t<std::is_base_of_v<exceptions::exception, E>>> {
        json coerce(const E &e) { return e.as_json(); }
    };


    template<>
    struct FOST_CORE_DECLSPEC coercer<string, t_null> {
        string coerce(t_null);
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<string, bool> {
        string coerce(bool b);
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<uint16_t, felspar::u8view> {
        uint16_t coerce(felspar::u8view);
    };
    template<>
    struct coercer<uint16_t, felspar::lstring> {
        auto coerce(felspar::lstring s) {
            return fostlib::coercer<uint16_t, felspar::u8view>().coerce(s);
        }
    };
    template<>
    struct coercer<uint16_t, string> {
        auto coerce(const string &s) {
            return fostlib::coercer<uint16_t, felspar::u8view>().coerce(s);
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<int, felspar::u8view> {
        int coerce(felspar::u8view);
    };
    template<>
    struct coercer<int, felspar::lstring> {
        auto coerce(felspar::lstring s) {
            return fostlib::coercer<int, felspar::u8view>().coerce(s);
        }
    };
    template<>
    struct coercer<int, string> {
        auto coerce(const string &s) {
            return fostlib::coercer<int, felspar::u8view>().coerce(s);
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<int64_t, felspar::u8view> {
        int64_t coerce(felspar::u8view);
    };
    template<>
    struct coercer<int64_t, felspar::lstring> {
        auto coerce(felspar::lstring s) {
            return fostlib::coercer<int64_t, felspar::u8view>().coerce(s);
        }
    };
    template<>
    struct coercer<int64_t, string> {
        auto coerce(const string &s) {
            return fostlib::coercer<int64_t, felspar::u8view>().coerce(s);
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, int64_t> {
        string coerce(int64_t i);
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<uint64_t, felspar::u8view> {
        uint64_t coerce(felspar::u8view);
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<uint64_t, string> {
        uint64_t coerce(string const &s) {
            return coercer<uint64_t, felspar::u8view>().coerce(s);
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, uint64_t> {
        string coerce(uint64_t i);
    };

    template<typename I>
    struct coercer<string, I, std::enable_if_t<std::is_integral_v<I>>> {
        string coerce(I i) {
            return fostlib::coercer<string, int64_t>().coerce(i);
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<float, felspar::u8view> {
        float coerce(felspar::u8view w);
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<float, string> {
        float coerce(string const &s) {
            return coercer<float, felspar::u8view>().coerce(s);
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<double, felspar::u8view> {
        double coerce(felspar::u8view w);
    };
    template<>
    struct coercer<double, felspar::lstring> {
        auto coerce(felspar::lstring s) {
            return fostlib::coercer<double, felspar::u8view>().coerce(s);
        }
    };
    template<>
    struct coercer<double, string> {
        auto coerce(const string &s) {
            return fostlib::coercer<double, felspar::u8view>().coerce(s);
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, double> {
        string coerce(double d);
    };


    template<>
    struct FOST_CORE_DECLSPEC coercer<string, nliteral> {
        string coerce(nliteral str) { return string(str); }
    };
    template<std::size_t L>
    struct FOST_CORE_DECLSPEC coercer<string, char[L]> {
        string coerce(const char str[L]) { return string(str); }
    };
    template<std::size_t L>
    struct FOST_CORE_DECLSPEC coercer<string, const char[L]> {
        string coerce(const char str[L]) { return string(str); }
    };

    template<>
    struct coercer<felspar::u8string, std::vector<unsigned char>> {
        felspar::u8string coerce(std::vector<unsigned char> const &v) {
            return felspar::u8string{
                    reinterpret_cast<char const *>(v.data()), v.size()};
        }
    };


    template<>
    struct coercer<string, float> {
        string coerce(float f) { return coercer<string, double>().coerce(f); }
    };


}

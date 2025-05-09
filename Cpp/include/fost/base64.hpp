#ifndef FOST_DETAIL_BASE64_HPP
#define FOST_DETAIL_BASE64_HPP
#pragma once


#include <fost/tagged-string.hpp>


namespace fostlib {


    struct FOST_CORE_DECLSPEC base64_string_tag {
        static void do_encode(fostlib::nliteral from, ascii_string &into);
        static void do_encode(const ascii_string &from, ascii_string &into);
        static void check_encoded(const ascii_string &s);
    };
    /// A tagged string type for Base-64 representations of binary data
    typedef tagged_string<base64_string_tag, ascii_string> base64_string;

    namespace detail {
        base64_string FOST_CORE_DECLSPEC base64_encode_3bytes(
                const unsigned char *data, std::size_t length);
        std::vector<unsigned char> FOST_CORE_DECLSPEC base64_decode_3bytes(
                const base64_string &string, base64_string::size_type pos);
    }


    template<std::size_t L>
    struct coercer<base64_string, unsigned char[L]> {
        base64_string coerce(const unsigned char pos[L]) {
            fostlib::base64_string ret;
            std::size_t length = L;
            for (; length > 3; length -= 3, pos += 3)
                ret += detail::base64_encode_3bytes(pos, 3);
            return ret + detail::base64_encode_3bytes(pos, length);
        }
    };
    template<std::size_t N>
    struct coercer<base64_string, std::array<felspar::byte, N>> {
        base64_string coerce(const std::array<felspar::byte, N> &a) {
            const auto *pos = reinterpret_cast<const unsigned char *>(a.data());
            fostlib::base64_string ret;
            auto length = N;
            for (; length > 3; length -= 3, pos += 3)
                ret += detail::base64_encode_3bytes(pos, 3);
            return ret + detail::base64_encode_3bytes(pos, length);
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC
            coercer<base64_string, std::vector<unsigned char>> {
        base64_string coerce(const std::vector<unsigned char> &v);
    };

    /// Allow coercion from Base 64 strings to byte vectors
    template<>
    struct FOST_CORE_DECLSPEC
            coercer<std::vector<unsigned char>, base64_string> {
        std::vector<unsigned char> coerce(const base64_string &v);
    };

    template<>
    struct coercer<string, base64_string> {
        string coerce(const base64_string &h) {
            return fostlib::coerce<string>(h.underlying());
        }
    };

    template<>
    struct coercer<base64_string, json> {
        base64_string coerce(json const &j) {
            return base64_string{fostlib::coerce<string>(j)};
        }
    };


}


#endif // FOST_DETAIL_BASE64_HPP

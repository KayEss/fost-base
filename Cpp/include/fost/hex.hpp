#ifndef FOST_DETAIL_HEX_HPP
#define FOST_DETAIL_HEX_HPP
#pragma once


#include <fost/tagged-string.hpp>


namespace fostlib {


    struct FOST_CORE_DECLSPEC hex_string_tag {
        static void do_encode(fostlib::nliteral from, ascii_string &into);
        static void do_encode(const ascii_string &from, ascii_string &into);
        static void check_encoded(const ascii_string &s);
    };
    typedef tagged_string<hex_string_tag, ascii_string> hex_string;

    template<>
    struct FOST_CORE_DECLSPEC coercer<hex_string, unsigned char> {
        hex_string coerce(unsigned char c);
    };

    /// Allow a hex string to be generated from a span
    template<std::size_t N>
    struct coercer<hex_string, std::span<unsigned char const, N>> {
        /// Perform the coercion
        hex_string coerce(std::span<unsigned char const> const v) {
            hex_string r;
            r.reserve((v.size() + 1) / 2);
            for (std::size_t c = 0; c != v.size(); ++c) {
                r += fostlib::coerce<hex_string>(v[c]);
            }
            return r;
        }
    };
    template<std::size_t N>
    struct coercer<hex_string, std::span<std::byte const, N>> {
        hex_string coerce(std::span<std::byte const, N> const b) {
            return fostlib::coerce<hex_string>(std::span{
                    reinterpret_cast<unsigned char const *>(b.data()),
                    b.size()});
        }
    };
    template<std::size_t N>
    struct coercer<hex_string, std::array<unsigned char, N>> {
        hex_string coerce(std::array<unsigned char, N> const &a) {
            return fostlib::coerce<hex_string>(std::span{a});
        }
    };
    template<std::size_t N>
    struct coercer<hex_string, std::array<std::byte, N>> {
        hex_string coerce(std::array<std::byte, N> const &a) {
            return fostlib::coerce<hex_string>(std::span{a});
        }
    };

    /// Allow a hex string to be generated from a vector of byte values
    template<>
    struct FOST_CORE_DECLSPEC coercer<hex_string, std::vector<unsigned char>> {
        /// Perform the coercion
        hex_string coerce(const std::vector<unsigned char> &v);
    };
    template<>
    struct coercer<hex_string, std::vector<std::byte>> {
        hex_string coerce(std::vector<std::byte> const &v) {
            return fostlib::coerce<hex_string>(std::span{
                    reinterpret_cast<unsigned char const *>(v.data()),
                    v.size()});
        }
    };

    /// Allow a normal string to be generated from a hex string
    template<>
    struct coercer<string, hex_string> {
        /// Perform the coercion
        string coerce(const hex_string &h) {
            return fostlib::coerce<string>(h.underlying());
        }
    };

    /// Allow a hex string to be interpreted as a number
    template<>
    struct FOST_CORE_DECLSPEC coercer<std::size_t, hex_string> {
        /// Perform the coercion
        std::size_t coerce(const hex_string &h);
    };

    /// Convert a hex string to a vector of bytes
    template<>
    struct FOST_CORE_DECLSPEC coercer<std::vector<std::byte>, hex_string> {
        std::vector<std::byte> coerce(hex_string const &);
    };

}


#endif // FOST_DETAIL_HEX_HPP

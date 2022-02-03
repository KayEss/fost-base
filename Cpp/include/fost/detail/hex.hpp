/**
    Copyright 2009-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_DETAIL_HEX_HPP
#define FOST_DETAIL_HEX_HPP
#pragma once


#include <fost/tagged-string.hpp>
#include <boost/array.hpp>
#include <boost/range/functions.hpp>

#include <span>


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

    /// Convert a hex string to a vector of bytes
    template<>
    struct FOST_CORE_DECLSPEC coercer<std::vector<std::byte>, hex_string> {
        std::vector<std::byte> coerce(hex_string const &);
    };

    /// Allow a hex string to be generated from a vector of byte values
    template<>
    struct FOST_CORE_DECLSPEC
            coercer<hex_string, std::span<unsigned char const>> {
        /// Perform the coercion
        hex_string coerce(std::span<unsigned char const>);
    };
    template<>
    struct coercer<hex_string, std::vector<unsigned char>> {
        hex_string coerce(std::vector<unsigned char> const &v) {
            return fostlib::coerce<hex_string>(
                    std::span<unsigned char const>{v});
        }
    };
    template<std::size_t L>
    struct coercer<hex_string, boost::array<unsigned char, L>> {
        hex_string coerce(const boost::array<unsigned char, L> &v) {
            return fostlib::coerce<hex_string>(
                    std::span<unsigned char const>{v});
        }
    };
    template<>
    struct coercer<hex_string, std::span<std::byte const>> {
        hex_string coerce(std::span<std::byte const> s) {
            return fostlib::coerce<hex_string>(std::span<unsigned char const>{
                    reinterpret_cast<unsigned char const *>(s.data()),
                    s.size()});
        }
    };
    template<std::size_t N>
    struct coercer<hex_string, std::array<std::byte const, N>> {
        hex_string coerce(std::array<std::byte const, N> const &a) {
            return fostlib::coerce<hex_string>(std::span<std::byte const>{a});
        }
    };
    template<std::size_t N>
    struct coercer<hex_string, std::array<std::byte, N>> {
        hex_string coerce(std::array<std::byte, N> const &a) {
            return fostlib::coerce<hex_string>(std::span<std::byte const>{a});
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


}


#endif // FOST_DETAIL_HEX_HPP

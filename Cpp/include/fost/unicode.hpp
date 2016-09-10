/*
    Copyright 2001-2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_UNICODE_HPP
#define FOST_UNICODE_HPP
#pragma once


#include <fost/array>
#include <fost/file.hpp>
#include <fost/tagged-string.hpp>


namespace fostlib {


    namespace utf {


        const utf32 c_bom = 0xFEFF;
        const std::size_t utf32_utf8_max_length = 4;
        const std::size_t utf32_utf16_max_length = 2;

        // Checks that a given utf32 character is valid - throws an exception if it isn't
        FOST_CORE_DECLSPEC utf32 assertValid( utf32 codepoint );

        // Returns the number utf32 code points in the string
        FOST_CORE_DECLSPEC std::size_t length( nliteral );
        FOST_CORE_DECLSPEC std::size_t length( nliteral, nliteral );
        FOST_CORE_DECLSPEC std::size_t length( wliteral );
        FOST_CORE_DECLSPEC std::size_t length( wliteral, wliteral );

        // Return how many utf8/utf16 characters are needed for this code point or string
        FOST_CORE_DECLSPEC std::size_t utf8length( utf32 codepoint );
        FOST_CORE_DECLSPEC std::size_t utf16length( utf32 codepoint );
        FOST_CORE_DECLSPEC std::size_t native_length( utf32 codepoint );

        // The number of native characters to encode the length
        FOST_CORE_DECLSPEC std::size_t native_length( nliteral );
        FOST_CORE_DECLSPEC std::size_t native_length( wliteral );

        // Fetch the next utf32 character from the sequence (or throw an exception)
        FOST_CORE_DECLSPEC utf32 decode( nliteral begin, nliteral end );
        FOST_CORE_DECLSPEC utf32 decode( wliteral begin, wliteral end );
        FOST_CORE_DECLSPEC utf32 decode( wchar_t first, wchar_t second );
        FOST_CORE_DECLSPEC utf32 decode( wchar_t first );

        // Encode the character into the stream. Return the number of utf8/utf16 characters written (0 if the buffer isn't long enough)
        FOST_CORE_DECLSPEC std::size_t encode( utf32 codepoint, utf8 *begin, const utf8 *end );
        inline std::size_t encode( utf32 codepoint, char *begin, char *end ) {
            return encode( codepoint, reinterpret_cast< utf8 * >( begin ), reinterpret_cast< utf8 * >( end ) );
        }
        FOST_CORE_DECLSPEC std::size_t encode( utf32 codepoint, utf16 *begin, const utf16 *end );


        /// For unsigned char types with an UTF-8 encoding
        class u8_view {
            array_view<unsigned char> buffer;
        public:
            u8_view(array_view<unsigned char> b)
            : buffer(b) {
            }

            u8_view(nliteral b, std::size_t s)
            : buffer(reinterpret_cast<const unsigned char *>(b), s) {
            }

            u8_view(const utf8_string &u8)
            : buffer(
                    reinterpret_cast<const unsigned char *>(u8.underlying().data()),
                    u8.underlying().size())
            {
            }

            /// An iterator that spits out UTF32 code points from the string
            class const_iterator {
                friend class u8_view;
                array_view<unsigned char> buffer;

                const_iterator(array_view<unsigned char> b)
                : buffer(b) {
                }
            public:
                typedef void difference_type;
                typedef utf32 value_type;
                typedef unsigned char *pointer;
                typedef utf32 reference;
                typedef std::forward_iterator_tag iterator_category;

                utf32 operator * () const {
                    return decode(reinterpret_cast<nliteral>(buffer.begin()),
                        reinterpret_cast<nliteral>(buffer.end()));
                }
                const_iterator &operator ++ () {
                    const auto here = **this;
                    const auto bytes = utf8length(here);
                    buffer = array_view<unsigned char>(buffer.data() + bytes, buffer.size() - bytes);
                    return *this;
                }
                bool operator == (const_iterator it) const {
                    return buffer == it.buffer;
                }
                bool operator != (const_iterator it) const {
                    return buffer != it.buffer;
                }
            };

            /// Construct a u8_view from part of another
            u8_view(const_iterator s, const_iterator e)
            : buffer(s.buffer.data(), s.buffer.size() - e.buffer.size()) {
            }

            std::size_t bytes() const {
                return buffer.size();
            }

            const_iterator begin() const {
                return buffer;
            }
            const_iterator end() const {
                return array_view<unsigned char>(buffer.data() + buffer.size(), 0u);
            }
        };


    }


    /// Allow us to coerce a UTF8 sequence to a UTF16 std::wstring
    template<>
    struct FOST_CORE_DECLSPEC coercer<std::wstring, utf::u8_view> {
        std::wstring coerce(utf::u8_view);
    };
    /// Turn a u8_view into JSON
    template<>
    struct coercer<json, utf::u8_view> {
        json coerce(utf::u8_view str) {
            return json(string(str.begin(), str.end()));
        }
    };


}


#endif // FOST_UNICODE_HPP


#ifndef FOST_STRING_UTILITY_NULLABLE_HPP
#define FOST_STRING_UTILITY_NULLABLE_HPP
#pragma once


#include <fost/string.hpp>
#include <fost/nullable.hpp>


namespace fostlib {

    /// Removes white space from the start and end of a string
    FOST_CORE_DECLSPEC nullable<felspar::u8view> trim(felspar::u8view);
    FOST_CORE_DECLSPEC nullable<felspar::u8view>
            trim(felspar::u8view, felspar::u8view);
    FOST_CORE_DECLSPEC nullable<felspar::u8view> trim(nullable<felspar::u8view>);
    inline nullable<string> trim(string const &text) {
        return trim(felspar::u8view{text});
    }
    inline nullable<string> trim(string const &text, felspar::u8view chars) {
        return trim(felspar::u8view{text}, chars);
    }
    inline nullable<string> trim(nullable<string> const &text) {
        if (not text)
            return null;
        else
            return trim(nullable<felspar::u8view>{text});
    }
    /// Removes white space from the start and end of a string
    template<typename Tag, typename Impl>
    inline nullable<tagged_string<Tag, Impl>>
            trim(tagged_string<Tag, Impl> const &text) {
        nullable<typename tagged_string<Tag, Impl>::impl_type> r =
                trim(felspar::u8view{text});
        if (not r)
            return null;
        else
            return tagged_string<Tag, Impl>{r.value()};
    }
    template<typename Tag, typename Impl>
    inline nullable<tagged_string<Tag, Impl>> trim(
            nullable<typename tagged_string<Tag, Impl>::impl_type> const &text) {
        if (not text)
            return null;
        else
            return trim(text.value());
    }


    /// Concatenate two strings with a separator (if needed)
    FOST_CORE_DECLSPEC nullable<string>
            concat(const nullable<felspar::u8view> &left,
                   felspar::u8view mid,
                   const nullable<felspar::u8view> &right);
    /// Concatenate two strings
    FOST_CORE_DECLSPEC nullable<string>
            concat(const nullable<felspar::u8view> &left,
                   const nullable<felspar::u8view> &right);

    /// Replace all occurrences of the requested string with the specified one
    FOST_CORE_DECLSPEC utf8_string replace_all(
            const utf8_string &text,
            felspar::u8view findThis,
            const nullable<felspar::u8view> &replaceWith = fostlib::null);
    /// Replace all occurrences of the requested string with the specified one
    FOST_CORE_DECLSPEC string replace_all(
            const string &text,
            felspar::u8view findThis,
            const nullable<felspar::u8view> &replaceWith = fostlib::null);
    /// Replace all occurrences of the requested string with the specified one
    FOST_CORE_DECLSPEC nullable<string> replace_all(
            const nullable<felspar::u8view> &text,
            felspar::u8view findThis,
            const nullable<felspar::u8view> &replaceWith = fostlib::null);


    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<utf8_string, nullable<utf8_string>>
            partition(const utf8_string &text, felspar::u8view bound);
    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<utf8_string, nullable<utf8_string>>
            partition(const nullable<utf8_string> &text, felspar::u8view bound);
    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<utf8_string, nullable<utf8_string>>
            partition(const utf8_string &text);
    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<utf8_string, nullable<utf8_string>>
            partition(const nullable<utf8_string> &text);

    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<string, nullable<string>>
            partition(const string &text, felspar::u8view bound);
    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<string, nullable<string>>
            partition(const nullable<string> &text, felspar::u8view bound);
    /// Partition (and trim) a string into two parts around the first space
    FOST_CORE_DECLSPEC
    std::pair<string, nullable<string>> partition(const string &text);
    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<string, nullable<string>> partition(const nullable<string> &text);

    /// Split a string in the form `first OPEN second CLOSE` returning first,
    /// second
    FOST_CORE_DECLSPEC std::pair<string, nullable<string>> crack(
            felspar::u8view text, felspar::u8view open, felspar::u8view close);


}


#endif // FOST_STRING_UTILITY_NULLABLE_HPP

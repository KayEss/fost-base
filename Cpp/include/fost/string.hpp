#ifndef FOST_STRING_HPP
#define FOST_STRING_HPP
#pragma once


#include <fost/string-fwd.hpp>
#include <felspar/cord/iostream.hpp>

#include <ostream>


namespace fostlib {


    /// ## String
    /**
        Interim wrapper around u8string. The intention is that we merge the
        types together so that we can switch over to u8string later on.
    */
    class string : public felspar::u8string {
      public:
        using size_type = std::size_t;
        /// Marker for end of string
        constexpr static const size_type npos =
                std::numeric_limits<size_type>::max();

        /// ### Constructors
        using felspar::u8string::u8string;
        string(felspar::u8string s) : felspar::u8string{s} {}
        string(std::string s) : felspar::u8string{std::move(s)} {}
        explicit string(std::string_view s) : string{std::string{s}} {}
        string(nliteral s) : felspar::u8string{std::string(s)} {}
        string(size_type, char32_t);
        string(const string &, size_type, size_type = npos);
        string(std::string::const_iterator b, std::string::const_iterator e)
        : felspar::u8string{std::string{b, e}} {}

        /// ### Conversions
        using felspar::u8string::operator std::string_view;
        using felspar::u8string::operator std::string;
        using felspar::u8string::operator felspar::u8view;
        using felspar::u8string::shrink_to_fit;

        /// We also need to allow this type to be changed to its super class
        /// to help the transition over to this implementation. Because we
        /// want the inheritance to be private (to stop all sorts of problematic
        /// things from happening), and we want to be able to use `u8string`
        /// in new code, we need a way to cleanly convert over. This is done
        /// in a new function that can be deprecated once the rest of the
        /// old string API has been removed.
        felspar::u8string u8string_transition() const {
            return felspar::u8string{*this};
        }

        /// ### Simple query APIs
        using felspar::u8string::bytes;
        using felspar::u8string::empty;
        using felspar::u8string::memory;
        auto data() const { return memory(); }
        using felspar::u8string::code_points;

        /// ### String operations
        string operator+(felspar::u8view v) const {
            return felspar::u8view{*this} + v;
        }
        string operator+(felspar::lstring v) const {
            return felspar::u8view{*this} + felspar::u8view{v};
        }
        string operator+(string const &v) const {
            return felspar::u8view{*this} + felspar::u8view{v};
        }
        string operator+(std::string const &v) const {
            return felspar::u8view{*this} + felspar::u8view{v};
        }
        string operator+(std::string_view const v) const {
            return felspar::u8view{*this} + felspar::u8view{v};
        }
        string operator+(char32_t) const;
        string operator+(nliteral r) const {
            return felspar::u8view{*this} + felspar::u8view{r, std::strlen(r)};
        }
        /// Because our underlying string type is immutable it makes no sense
        /// to do any reservation.
        void reserve(size_type) {}

        /// ### Iteration based APIs
        using felspar::u8string::begin;
        using felspar::u8string::const_iterator;
        using felspar::u8string::end;

        /// ### Comparison operators

        /// We need these explicitly here to resolve ambiguities otherwise
        /// things get too complicated The implementation in felspar::u8string
        /// is generic though, so that's good
        friend bool operator==(string const &l, string const &r) {
            return felspar::u8view{l} == felspar::u8view{r};
        }
        friend bool operator==(string const &l, felspar::u8view r) {
            return felspar::u8view{l} == r;
        }
        friend bool operator==(string const &l, std::string const &r) {
            return felspar::u8view{l} == r;
        }
        friend bool operator==(string const &l, char const *r) {
            return felspar::u8view{l} == r;
        }
        friend bool operator==(felspar::u8string l, string const &r) {
            return l == felspar::u8view{r};
        }
        friend bool operator==(felspar::lstring l, string const &r) {
            return l == felspar::u8view{r};
        }
        template<size_type N>
        friend bool operator==(string const &l, char const (&r)[N]) {
            return felspar::u8view{l} == r;
        }
        template<size_type N>
        friend bool operator==(char const (&l)[N], string const &r) {
            return l == felspar::u8view{r};
        }

        friend bool operator!=(string const &l, string const &r) {
            return felspar::u8view{l} != felspar::u8view{r};
        }
        friend bool operator!=(string const &l, felspar::u8view r) {
            return felspar::u8view{l} != r;
        }
        friend bool operator!=(felspar::u8view l, string const &r) {
            return l != felspar::u8view{r};
        }
        template<size_type N>
        friend bool operator!=(string const &l, char const (&r)[N]) {
            return felspar::u8view{l} != r;
        }
        template<size_type N>
        friend bool operator!=(char const (&l)[N], string const &r) {
            return l != felspar::u8view{r};
        }

        auto operator<=>(string const &right) const = default;

        /// ### Algorithmic APIs
        size_type find(char32_t, size_type = 0u) const;
        size_type find(const string &, size_type = 0u) const;
        size_type find_first_of(char32_t c) const { return find(c); }
        size_type find_first_of(const string &) const;
        size_type find_first_not_of(const string &) const;
        size_type find_last_not_of(const string &) const;
        using felspar::u8string::starts_with;
        bool startswith(const string &s) const {
            return starts_with(felspar::u8view{s});
        }
        bool startswith(char32_t c) const {
            return not empty() && *begin() == c;
        }
        using felspar::u8string::ends_with;
        bool endswith(const string &s) const {
            return ends_with(felspar::u8view{s});
        }
        bool endswith(char32_t c) const { return ends_with(string{1u, c}); }

        /// ### Mutation APIs (to be deprecated)
        string &operator+=(wchar_t c) { return *this = (*this + c); }
        string &operator+=(const string &s) {
            return *this = felspar::u8view{*this} + felspar::u8view{s};
        }
        string &clear() { return *this = felspar::u8string{}; }
        string &erase() { return clear(); }
        string &erase(size_type s); // Erase from s to end
        string &erase(size_type s, size_type c); // Erase c chars from s
        string &insert(size_type, const string &);
        string &
                replace(size_type,
                        size_type,
                        const string &,
                        size_type = 0u,
                        size_type = npos);

        /// ### Substrings and slicing
        string substr() const { return *this; }
        string substr(size_type b) const {
            return felspar::u8string::substr(b);
        }
        string substr(size_type b, size_type c) const {
            return felspar::u8string::substr_pos(b, b + c);
        }
        char32_t at(size_type) const;
        char32_t operator[](size_type p) const { return at(p); }
    };


    /// ADL `std::size`and `std::data`  implementations
    inline auto size(const string &s) { return s.bytes(); }
    inline auto data(const string &s) {
        return reinterpret_cast<char const *>(s.memory().data());
    }


    /// ### Binary operators needed outside of the class
    inline felspar::u8string operator+(felspar::u8view l, string const &r) {
        return l + felspar::u8view{r};
    }
    inline felspar::u8string operator+(felspar::u8string l, string const &r) {
        return felspar::u8view{l} + felspar::u8view{r};
    }
    inline string operator+(nliteral l, const string &r) {
        return felspar::u8view{l, std::strlen(l)} + r;
    }
    inline string operator+(felspar::u8string l, std::string_view const r) {
        return l + felspar::u8view(r);
    }


}

#endif // FOST_STRING_HPP

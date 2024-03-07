#ifndef FOST_JSON_HPP
#define FOST_JSON_HPP
#pragma once


#include <fost/json-core.hpp>
#include <fost/nullable.hpp>
#include <fost/coerce/ints.hpp>
#include <fost/coerce/nullable.hpp>


namespace fostlib {


    template<>
    inline json::json(const nullable<string> &s) : m_element() {
        if (s) m_element = s->u8string_transition();
    }
    template<>
    inline json::json(nullable<string> &&s) : m_element() {
        if (s) m_element = s->u8string_transition();
    }

    template<>
    inline nullable<json::object_t> json::get() const {
        const object_p *o = std::get_if<object_p>(&m_element);
        if (o)
            return **o;
        else
            return null;
    }

    template<>
    inline nullable<json::array_t> json::get() const {
        const array_p *a = std::get_if<array_p>(&m_element);
        if (a)
            return **a;
        else
            return null;
    }


    class FOST_CORE_DECLSPEC jcursor {
      public:
        using string_t = string;
        using number_t = json::array_t::size_type;
        using index_t = std::variant<number_t, string_t>;
        using stack_t = std::vector<index_t>;

        /// Create an empty jcursor representing the root of a JSON blob
        jcursor();
        /// Allow a jcursor to be implicitly created from a narrow char literal
        jcursor(nliteral n); // TODO Make explicit
        /// Copy & move constructor
        jcursor(const jcursor &j) : m_position(j.m_position) {}
        jcursor(jcursor &&j) : m_position(std::move(j.m_position)) {}
        /// Converting constructors
        explicit jcursor(int i);
        explicit jcursor(json::array_t::size_type i);
        explicit jcursor(felspar::u8view);
        explicit jcursor(felspar::u8string s);
        explicit jcursor(string);
        explicit jcursor(const json &j);

        /// Construct a jcursor from a string using the requested string as
        /// separator
        static jcursor split(const string &s, const string &separator);

        /// Variadic jcursor constructor
        template<typename A1, typename A2, typename... As>
        explicit jcursor(A1 &&a1, A2 &&a2, As &&...a)
        : jcursor(std::forward<A1>(a1)) {
            append(std::forward<A2>(a2), std::forward<As>(a)...);
        }

        /// Assignment
        jcursor &operator=(jcursor &&j) {
            m_position = std::move(j.m_position);
            return *this;
        }
        jcursor &operator=(const jcursor &j) {
            m_position = j.m_position;
            return *this;
        }
        template<typename A1>
        jcursor &operator=(A1 &&a1) {
            m_position.clear();
            append(std::forward<A1>(a1));
            return *this;
        }

        /// Append operators
        jcursor &operator/=(json::array_t::size_type);
        jcursor &operator/=(int i) {
            return (*this) /= fostlib::coerce<json::array_t::size_type>(i);
        }
        jcursor &operator/=(nliteral n) {
            return (*this) /= fostlib::string(n);
        }
        jcursor &operator/=(felspar::u8view);
        jcursor &operator/=(const json &j);
        jcursor &operator/=(const jcursor &jc);

        template<typename T>
        jcursor operator/(T &&i) const {
            return jcursor(*this) /= std::forward<T>(i);
        }

        jcursor &enter();
        jcursor &enter(const string &i);
        jcursor &pop();

        jcursor &operator++();

        /// Push to the back of this location in the passed in `json`
        json &push_back(json &j, json &&v) const;
        template<typename V>
        json &push_back(json &j, V &&v) const {
            return push_back(j, json{std::forward<V>(v)});
        }
        template<typename V>
        [[nodiscard]] json push_back(json &&j, V &&v) const {
            auto copy{std::move(j)};
            push_back(copy, std::forward<V>(v));
            return copy;
        }

        /// Insert at this location in the passed in `json`. Fails if the
        /// location is occupied.
        json &
                insert(json &j,
                       json &&v,
                       felspar::source_location const & =
                               felspar::source_location::current()) const;
        template<typename V>
        json &
                insert(json &j,
                       V &&v,
                       felspar::source_location const &loc =
                               felspar::source_location::current()) const {
            return insert(j, json{std::forward<V>(v)}, loc);
        }
        template<typename V>
        [[nodiscard]] json
                insert(json &&j,
                       V &&v,
                       felspar::source_location const &loc =
                               felspar::source_location::current()) const {
            auto copy{std::move(j)};
            insert(copy, std::forward<V>(v), loc);
            return copy;
        }

        /// Replace the item at this location in the passed in `json`. Fails
        /// if there is nothing there already.
        json &replace(json &j, json &&v) const;
        template<typename V>
        json &replace(json &j, V &&v) const {
            return replace(j, json{std::forward<V>(v)});
        }
        template<typename V>
        [[nodiscard]] json replace(json &&j, V &&v) const {
            auto copy{std::move(j)};
            replace(copy, std::forward<V>(v));
            return copy;
        }

        /// Set the item at this location in the passed in `json` irrespective
        /// of its current value.
        json &set(json &j, json &&v) const;
        template<typename V>
        json &set(json &j, V &&v) const {
            return set(j, json{std::forward<V>(v)});
        }

        /// Delete the item at this location in the passed in `json`. Fails if
        /// the location doesn't have a value.
        json &del_key(json &j) const;
        [[nodiscard]] json del_key(json &&j) const {
            auto copy{std::move(j)};
            del_key(copy);
            return copy;
        }

        /// Compare for equality
        bool operator==(const jcursor &j) const;

        /// Allow this jcursor to look a bit like a container
        typedef stack_t::const_iterator const_iterator;
        const_iterator begin() const { return m_position.begin(); }
        const_iterator end() const { return m_position.end(); }
        typedef stack_t::size_type size_type;
        size_type size() const { return m_position.size(); }
        typedef index_t value_type;
        value_type operator[](size_type i) const { return m_position.at(i); }

        /// Construct a jcursor from part of a different one
        jcursor(stack_t s) : m_position(std::move(s)) {}
        jcursor(stack_t::const_iterator b, stack_t::const_iterator e);


        /// ### JSON pointer

        /// From a JSON pointer URL fragment or JSON string
        static jcursor parse_json_pointer_string(felspar::u8view);
        static jcursor parse_json_pointer_fragment(felspar::u8view);
        /// Turn into a JSON pointer fragment (without leading #)
        ascii_printable_string as_json_pointer() const;


      private:
        stack_t m_position;
        friend class json;

        json &copy_from_root(json &) const;

        template<typename A1>
        void append(A1 &&a1) {
            (*this) /= jcursor{std::forward<A1>(a1)};
        }
        template<typename A1, typename... As>
        void append(A1 &&a1, As &&...a) {
            (*this) /= jcursor{std::forward<A1>(a1)};
            append(std::forward<As>(a)...);
        }
    };


    /// Allow comparison of parts of a jcursor to a literal
    bool operator==(const jcursor::value_type &, felspar::u8view);


    template<typename F>
    struct coercer<json, nullable<F>> {
        json coerce(const nullable<F> &f) {
            if (not f) {
                return json{};
            } else {
                return fostlib::coercer<json, F>().coerce(f.value());
            }
        }
    };
    template<typename T>
    struct coercer<std::optional<T>, json> {
        std::optional<T> coerce(json const &f, felspar::source_location loc) {
            if (f.isnull()) {
                return std::nullopt;
            } else {
                return coercer<T, json>{}.coerce(f, std::move(loc));
            }
        }
    };


    template<typename F>
    struct coercer<json, felspar::memory::holding_pen<F>> {
        json coerce(felspar::memory::holding_pen<F> const &f) {
            if (not f) {
                return json{};
            } else {
                return fostlib::coercer<json, F>().coerce(*f);
            }
        }
    };
    template<typename T>
    struct coercer<felspar::memory::holding_pen<T>, json> {
        felspar::memory::holding_pen<T>
                coerce(json const &f, felspar::source_location loc) {
            if (f.isnull()) {
                return {};
            } else {
                return coercer<T, json>{}.coerce(f, std::move(loc));
            }
        }
    };


    /// Allow us to coerce to any integral type from JSON
    template<typename T>
        requires std::is_integral_v<T>
    struct coercer<T, fostlib::json> {
        T coerce(const fostlib::json &j, felspar::source_location loc) {
            try {
                return fostlib::coerce<T>(fostlib::coerce<int64_t>(j, loc));
            } catch (fostlib::exceptions::exception &e) {
                jcursor("action").insert(
                        e.data(),
                        json("Trying to cast from JSON to an integral type"));
                jcursor("json").insert(e.data(), j);
                throw;
            }
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<int64_t, json> {
        int64_t coerce(const json &j, felspar::source_location);
    };

    /// Bool and json
    template<>
    struct FOST_CORE_DECLSPEC coercer<bool, json> {
        bool coerce(const json &f, felspar::source_location);
    };

    /// Allow conversion of JSON to a double
    template<>
    struct FOST_CORE_DECLSPEC coercer<double, json> {
        double coerce(const json &j, felspar::source_location);
    };

    /// Allow conversion of JSON into strings. Coercion to a felspar::u8view
    /// only works where the JSON is a string of some sort. If the JSON may be
    /// a number (for example) and you still want to try to get a string
    /// then you need to coerce to a fostlib::string.
    template<>
    struct FOST_CORE_DECLSPEC coercer<felspar::u8view, json> {
        felspar::u8view coerce(json const &, felspar::source_location loc);
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, json> {
        string coerce(const json &f, felspar::source_location loc);
    };
    template<>
    struct coercer<felspar::u8string, json> {
        felspar::u8string coerce(const json &f, felspar::source_location loc) {
            return fostlib::coerce<string>(f, loc).u8string_transition();
        }
    };
    /// Convert to a nullable u8view. If the contained type is not a
    /// string then this will return null rather than throw an error
    template<>
    struct FOST_CORE_DECLSPEC coercer<nullable<felspar::u8view>, json> {
        nullable<felspar::u8view> coerce(const json &);
    };


    /// Allow conversion from a jcursor to an JSON array
    template<>
    struct FOST_CORE_DECLSPEC coercer<json, jcursor> {
        json coerce(const jcursor &);
    };
    /// Allow us to turn a JSON array into a jcursor
    template<>
    struct FOST_CORE_DECLSPEC coercer<jcursor, json> {
        jcursor coerce(const json &);
    };


    /// Coercion of jcursor elements to underlying types
    template<>
    struct coercer<nullable<string>, jcursor::value_type> {
        nullable<string> coerce(const jcursor::value_type &v) {
            const auto *inner = std::get_if<string>(&v);
            if (inner)
                return *inner;
            else
                return null;
        }
    };
    template<>
    struct coercer<nullable<json::array_t::size_type>, jcursor::value_type> {
        nullable<json::array_t::size_type> coerce(const jcursor::value_type &v) {
            const auto *inner = std::get_if<json::array_t::size_type>(&v);
            if (inner)
                return *inner;
            else
                return null;
        }
    };

    /// Allow coerceion from a std::vector of something coerceable
    /// to JSON
    template<typename V>
    struct coercer<json, std::vector<V>> {
        json coerce(const std::vector<V> &v) {
            json::array_t a;
            for (const auto &d : v) a.push_back(fostlib::coerce<json>(d));
            return a;
        }
    };


}


namespace std {


    /// Print monostate values
    template<typename C>
    basic_ostream<C> &operator<<(basic_ostream<C> &o, monostate) {
        return o << "** null **";
    }
    /// When output unparse the JSON and pretty print it
    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, const fostlib::json &s) {
        return o << fostlib::json::unparse(s, true);
    }
    /// When output convert to JSON and print that
    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, const fostlib::jcursor &p) {
        return o << fostlib::json::unparse(
                       fostlib::coerce<fostlib::json>(p), false);
    }


}


#endif // FOST_JSON_HPP

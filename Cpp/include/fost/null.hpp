#pragma once


#include <optional>
#include <ostream>


namespace fostlib {


    /// Alias `t_null` to the standard version.
    using t_null = std::nullopt_t;

    /// The null value
    constexpr auto const null = std::nullopt;


}


namespace std {
    /// We want the nulls to compare equal
    inline bool operator==(nullopt_t, nullopt_t) { return true; }
    inline bool operator!=(nullopt_t, nullopt_t) { return false; }

    /// Print nullopt_t values
    template<typename C, typename T>
    basic_ostream<C, T> &operator<<(basic_ostream<C, T> &o, nullopt_t) {
        return o << "** null **";
    }
}

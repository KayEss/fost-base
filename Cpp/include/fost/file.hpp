#ifndef FOST_FILE_HPP
#define FOST_FILE_HPP
#pragma once


#include <fost/string-fwd.hpp>
#include <fost/filesystem.hpp>


namespace fostlib {


    namespace utf {


        /// Load a text file which is UTF-8 encoded
        FOST_CORE_DECLSPEC string
                load_file(const std::filesystem::path &filename);
        /// Load a text file which is UTF-8 encoded, giving default content to
        /// use if there is an error
        FOST_CORE_DECLSPEC string load_file(
                const std::filesystem::path &filename,
                const string &default_content);

        /// Save a string into a file UTF-8 encoded
        FOST_CORE_DECLSPEC void save_file(
                const std::filesystem::path &filename, const string &content);


    }


    typedef std::filesystem::directory_iterator directory_iterator;


    /// Coerce a string to a file path
    template<>
    struct coercer<std::filesystem::path, string> {
        std::filesystem::path coerce(const string &s) {
            return static_cast<std::string>(s);
        }
    };
    /// Coerce a file path to a string
    template<>
    struct coercer<string, std::filesystem::path> {
        string coerce(const std::filesystem::path &p) {
            return fostlib::coerce<string>(p.string());
        }
    };
    /// Coerce a file path to JSON
    template<>
    struct coercer<json, std::filesystem::path> {
        json coerce(const std::filesystem::path &p) {
            return json(fostlib::coerce<string>(p));
        }
    };
    /// Coerce JSON to a file path
    template<>
    struct coercer<std::filesystem::path, fostlib::json> {
        std::filesystem::path
                coerce(const json &j, felspar::source_location loc) {
            return fostlib::coerce<std::filesystem::path>(
                    fostlib::coerce<string>(j, loc), loc);
        }
    };


    /// Return a path unique pathname
    FOST_CORE_DECLSPEC std::filesystem::path unique_filename();


    /// Join two paths. If path is rooted it is returned, otherwise it is joined
    /// to root
    FOST_CORE_DECLSPEC std::filesystem::path join_paths(
            const std::filesystem::path &root,
            const std::filesystem::path &path);


}


namespace std {


    /// Allow a path to be printed to an ostream
    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, const std::filesystem::path &p) {
        return o << fostlib::coerce<fostlib::string>(p);
    }


}


#endif // FOST_FILE_HPP

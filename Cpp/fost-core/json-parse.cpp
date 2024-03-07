#include "fost-core.hpp"
#include <fost/parse/json.hpp>

#include <fost/unicode.hpp>
#include <fost/exception/parse_error.hpp>


namespace {
    fostlib::json_parser<felspar::cord::const_u32u16_iterator<
            fostlib::string::const_iterator>> const c_json_str_rule;
    fostlib::json_parser<felspar::cord::const_u32u16_iterator<
            felspar::u8view::const_iterator>> const c_json_u8v_rule;
    fostlib::json_parser<felspar::u16view::const_u16_iterator> const
            c_json_u16_rule;

    fostlib::json_parser<
            char const *,
            fostlib::json_string_parser_no_unicode_escapes> const c_json_quick;

    fostlib::json quick_parse(felspar::u8view text) {
        fostlib::json ret{};
        char const *pos = reinterpret_cast<char const *>(text.memory().begin());
        char const *const end =
                reinterpret_cast<char const *>(text.memory().end());
        try {
            if (boost::spirit::qi::parse(pos, end, c_json_quick, ret)
                && pos == end) {
                return ret;
            } else {
                throw fostlib::exceptions::parse_error{
                        "Whilst quick parsing JSON string",
                        fostlib::string{std::string{pos, end}}};
            }
        } catch (std::runtime_error &e) {
            throw fostlib::exceptions::parse_error{
                    e.what(), fostlib::string{std::string{pos, end}}};
        }
    }

    fostlib::json quick_parse(felspar::u8view text, fostlib::json const &def) {
        fostlib::json ret{};
        char const *pos = reinterpret_cast<char const *>(text.memory().begin());
        char const *const end =
                reinterpret_cast<char const *>(text.memory().end());
        try {
            if (boost::spirit::qi::parse(pos, end, c_json_quick, ret)
                && pos == end) {
                return ret;
            } else {
                return def;
            }
        } catch (std::runtime_error &e) {
            throw fostlib::exceptions::parse_error{
                    e.what(), fostlib::string{std::string{pos, end}}};
        }
    }
}


fostlib::json fostlib::json::parse(felspar::u8view toparse) {
    if (std::string_view{toparse}.find("\\u") == std::string_view::npos) {
        return quick_parse(toparse);
    }
    fostlib::json ret{};
    auto pos =
            felspar::cord::make_u32u16_iterator(toparse.begin(), toparse.end());
    try {
        if (boost::spirit::qi::parse(pos.first, pos.second, c_json_u8v_rule, ret)
            && pos.first == pos.second) {
            return ret;
        } else {
            throw exceptions::parse_error(
                    "Whilst parsing JSON string",
                    string(pos.first.u32_iterator(), pos.second.u32_iterator()));
        }
    } catch (std::runtime_error &e) {
        throw exceptions::parse_error(
                e.what(),
                string(pos.first.u32_iterator(), pos.second.u32_iterator()));
    }
}


fostlib::json fostlib::json::parse(felspar::u16view s) {
    fostlib::json ret{};
    auto pos = s.u16begin();
    try {
        if (boost::spirit::qi::parse(pos, s.u16end(), c_json_u16_rule, ret)
            && pos == s.u16end()) {
            return ret;
        } else {
            throw exceptions::parse_error{
                    "Whilst parsing JSON string",
                    fostlib::coerce<felspar::u8string>(
                            felspar::u16view(&*pos, s.u16end() - pos))};
        }
    } catch (std::runtime_error &e) {
        throw exceptions::parse_error{
                e.what(),
                fostlib::coerce<felspar::u8string>(
                        felspar::u16view(&*pos, s.u16end() - pos))};
    }
}


fostlib::json fostlib::json::parse(string const &toparse, json const &def) {
    if (std::string_view{toparse}.find("\\u") == std::string_view::npos) {
        return quick_parse(toparse, def);
    }
    fostlib::json ret{};
    auto pos =
            felspar::cord::make_u32u16_iterator(toparse.begin(), toparse.end());
    try {
        if (boost::spirit::qi::parse(pos.first, pos.second, c_json_str_rule, ret)
            && pos.first == pos.second) {
            return ret;
        } else {
            return def;
        }
    } catch (std::runtime_error &e) {
        throw exceptions::parse_error(
                e.what(),
                string(pos.first.u32_iterator(), pos.second.u32_iterator()));
    }
}

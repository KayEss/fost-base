#include "fost-core.hpp"
#include <fost/counter.hpp>
#include <fost/insert.hpp>
#include <fost/push_back.hpp>


namespace {
    fostlib::performance p_absorbed(fostlib::c_fost, "exceptions", "absorbed");
}


void fostlib::absorb_exception() noexcept {
    // An exception is in the process of being thrown away.
    // We want to be very careful not to do anything that may throw again.
    ++p_absorbed;
}


void fostlib::detail::throw_null_exception() {
    throw exceptions::null("Empty nullable instance");
}


/**
    fostlib::exceptions::exception
*/


namespace {
    void source_description(
            fostlib::json &js, felspar::source_location const &sl) {
        fostlib::json::object_t thrown;
        thrown["filename"] = sl.file_name();
        thrown["function"] = sl.function_name();
        thrown["line"] = sl.line();
        thrown["column"] = sl.column();
        insert(js, "source", thrown);
    }
}
fostlib::exceptions::exception::exception(felspar::source_location sl) noexcept
: m_source_location{std::move(sl)} {
    try {
        source_description(m_data, m_source_location);
    } catch (...) { absorb_exception(); }
}


fostlib::exceptions::exception::exception(
        felspar::u8view m, felspar::source_location sl) noexcept
: m_source_location{std::move(sl)} {
    try {
        source_description(m_data, m_source_location);
        insert(m_data, "message", m);
    } catch (...) { absorb_exception(); }
}


fostlib::exceptions::exception::~exception() noexcept try {
} catch (...) { absorb_exception(); }


const fostlib::json &fostlib::exceptions::exception::data() const {
    return m_data;
}


fostlib::json &fostlib::exceptions::exception::data() { return m_data; }


char const *fostlib::exceptions::exception::what() const noexcept {
    try {
        std::stringstream ss;
        ss << message() << '\n' << data();
        m_what_string = felspar::u8string{ss.str()};
        return m_what_string.shrink_to_fit();
    } catch (...) {
        return "Unknown fostlib::exceptions::exception -- "
               "Exception thrown during generation of exception description";
    }
}


fostlib::ostream &
        fostlib::exceptions::exception::printOn(fostlib::ostream &o) const {
    return o << what() << "\nData: " << data();
}


fostlib::json fostlib::exceptions::exception::as_json() const {
    json out;
    insert(out, "exception", message());
    insert(out, "data", data());
    return out;
}


/**
    ## Individual exceptions
*/


#include <fost/exception/cast_fault.hpp>
fostlib::exceptions::cast_fault::cast_fault(
        const string &message, felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {}
felspar::u8view fostlib::exceptions::cast_fault::message() const noexcept {
    return "The requested cast is not possible";
}


#include <fost/exception/external_process_failure.hpp>
fostlib::exceptions::external_process_failure::external_process_failure(
        felspar::source_location sl) noexcept
: exception{std::move(sl)} {}
fostlib::exceptions::external_process_failure::external_process_failure(
        const string &command, felspar::source_location sl) noexcept
: exception{command, std::move(sl)} {}
felspar::u8view
        fostlib::exceptions::external_process_failure::message() const noexcept {
    return "External process exit code not zero";
}


#include <fost/exception/file_error.hpp>
fostlib::exceptions::file_error::file_error(
        const string &message,
        const string &filename,
        felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {
    try {
        insert(data(), "filename", filename);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::file_error::file_error(
        const string &message,
        std::filesystem::path const &file,
        std::error_code error,
        felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {
    try {
        insert(data(), "filename", file.string().c_str());
        insert(data(), "error", error);
    } catch (...) { absorb_exception(); }
}
felspar::u8view fostlib::exceptions::file_error::message() const noexcept {
    return "A File I/O error occured";
}


#include <fost/exception/json_error.hpp>
fostlib::exceptions::json_error::json_error(
        const string &message, felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {}
fostlib::exceptions::json_error::json_error(
        const string &message,
        const json &value,
        felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {
    try {
        insert(data(), "value", value);
    } catch (...) { fostlib::absorb_exception(); }
}
felspar::u8view fostlib::exceptions::json_error::message() const noexcept {
    return "JSON error";
}


#include <fost/exception/missing_setting.hpp>
fostlib::exceptions::missing_setting::missing_setting(
        const string &section,
        const string &name,
        felspar::source_location sl) noexcept
: exception{std::move(sl)} {
    try {
        insert(data(), "section", section);
        insert(data(), "name", name);
    } catch (...) { fostlib::absorb_exception(); }
}
felspar::u8view fostlib::exceptions::missing_setting::message() const noexcept {
    return "Setting is missing";
}


#include <fost/exception/not_a_number.hpp>
fostlib::exceptions::not_a_number::not_a_number(
        const string &message, felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {}
fostlib::exceptions::not_a_number::not_a_number(
        const string &message,
        const string &value,
        felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {
    try {
        insert(data(), "value", value);
    } catch (...) { absorb_exception(); }
}
felspar::u8view fostlib::exceptions::not_a_number::message() const noexcept {
    return "Not a number";
}


#include <fost/exception/not_implemented.hpp>
fostlib::exceptions::not_implemented::not_implemented(
        felspar::source_location sl) noexcept
: exception{std::move(sl)} {}
fostlib::exceptions::not_implemented::not_implemented(
        const string &mess, felspar::source_location sl) noexcept
: exception{mess, std::move(sl)} {}
fostlib::exceptions::not_implemented::not_implemented(
        const string &mes,
        const string &extra,
        felspar::source_location sl) noexcept
: exception{mes, std::move(sl)} {
    try {
        insert(data(), "detail", extra);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::not_implemented::not_implemented(
        const string &mes, const json &extra, felspar::source_location sl)
: exception{mes, std::move(sl)} {
    insert(data(), "detail", extra);
}
felspar::u8view fostlib::exceptions::not_implemented::message() const noexcept {
    return "Feature not implemented";
}


#include <fost/exception/not_null.hpp>
fostlib::exceptions::not_null::not_null(felspar::source_location sl) noexcept
: exception{std::move(sl)} {}
fostlib::exceptions::not_null::not_null(
        const string &mes, felspar::source_location sl) noexcept
: exception{mes, std::move(sl)} {}
fostlib::exceptions::not_null::not_null(
        const string &mes,
        const string &inf,
        felspar::source_location sl) noexcept
: exception{mes, std::move(sl)} {
    try {
        insert(data(), "info", inf);
    } catch (...) { absorb_exception(); }
}
felspar::u8view fostlib::exceptions::not_null::message() const noexcept {
    return "A value was found where none was expected";
}


#include <fost/exception/not_unique.hpp>
fostlib::exceptions::not_unique::not_unique(
        const string &message, felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {}
fostlib::exceptions::not_unique::not_unique(
        const string &error,
        const string &context,
        const string &alternative1,
        const string &alternative2,
        felspar::source_location sl) noexcept
: exception{error, std::move(sl)} {
    try {
        insert(data(), "context", context);
        push_back(data(), "alternatives", alternative1);
        push_back(data(), "alternatives", alternative2);
    } catch (...) { absorb_exception(); }
}
felspar::u8view fostlib::exceptions::not_unique::message() const noexcept {
    return "Uniqueness violation";
}


#include <fost/exception/null.hpp>
fostlib::exceptions::null::null(felspar::source_location sl) noexcept
: exception{std::move(sl)} {}
fostlib::exceptions::null::null(
        const string &mes, felspar::source_location sl) noexcept
: exception{mes, std::move(sl)} {}
fostlib::exceptions::null::null(
        const string &mes,
        const string &inf,
        felspar::source_location sl) noexcept
: exception{mes, std::move(sl)} {
    try {
        insert(data(), "info", inf);
    } catch (...) { absorb_exception(); }
}
felspar::u8view fostlib::exceptions::null::message() const noexcept {
    return "Attempt to de-reference null value";
}


#include <fost/exception/out_of_range.hpp>
fostlib::exceptions::out_of_range_string::out_of_range_string(
        const string &mn,
        const string &mx,
        const string &v,
        felspar::source_location sl) noexcept
: exception{std::move(sl)} {
    try {
        insert(data(), "value", v);
        insert(data(), "min", mn);
        insert(data(), "max", mx);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::out_of_range_string::out_of_range_string(
        const string &mes,
        const string &mn,
        const string &mx,
        const string &v,
        felspar::source_location sl) noexcept
: exception{mes, std::move(sl)} {
    try {
        insert(data(), "value", v);
        insert(data(), "min", mn);
        insert(data(), "max", mx);
    } catch (...) { absorb_exception(); }
}
felspar::u8view
        fostlib::exceptions::out_of_range_string::message() const noexcept {
    return "Value outside of allowable range";
}


#include <fost/exception/overflow.hpp>
fostlib::exceptions::overflow<fostlib::string>::overflow(
        const string &message, felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {}
fostlib::exceptions::overflow<fostlib::string>::overflow(
        const string &message,
        const string &n,
        const string &d,
        const string &m,
        felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {
    try {
        insert(data(), "numerator", n);
        insert(data(), "denominator", d);
        insert(data(), "multiplier", m);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::overflow<fostlib::string>::overflow(
        const string &n,
        const string &d,
        const string &m,
        felspar::source_location sl) noexcept
: exception{std::move(sl)} {
    try {
        insert(data(), "numerator", n);
        insert(data(), "denominator", d);
        insert(data(), "multiplier", m);
    } catch (...) { absorb_exception(); }
}
felspar::u8view fostlib::exceptions::overflow<fostlib::string>::message()
        const noexcept {
    return "An Overflow occured";
}


#include <fost/exception/parse_error.hpp>
fostlib::exceptions::parse_error::parse_error(
        const string &message, felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {}
fostlib::exceptions::parse_error::parse_error(
        const string &message,
        const string &value,
        felspar::source_location sl) noexcept
: exception{message, std::move(sl)} {
    try {
        insert(data(), "value", value);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::parse_error::parse_error(
        string const &message,
        std::size_t line,
        std::size_t col,
        felspar::source_location sl)
: exception{message, std::move(sl)} {
    try {
        insert(data(), "pos", "line", line);
        insert(data(), "pos", "column", col);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::parse_error::parse_error(
        string const &message,
        std::filesystem::path const &fn,
        std::size_t line,
        std::size_t col,
        felspar::source_location sl)
: exception{message, std::move(sl)} {
    try {
        insert(data(), "filename", fn);
        insert(data(), "pos", "line", line);
        insert(data(), "pos", "column", col);
    } catch (...) { absorb_exception(); }
}
felspar::u8view fostlib::exceptions::parse_error::message() const noexcept {
    return "Parsing error";
}


#include <fost/exception/settings_fault.hpp>
fostlib::exceptions::settings_fault::settings_fault(
        const string &error,
        const string &domain,
        const string &section,
        const string &name,
        const string &value,
        felspar::source_location sl) noexcept
: exception{error, std::move(sl)} {
    insert(data(), "domain", domain);
    insert(data(), "section", section);
    insert(data(), "name", name);
    insert(data(), "value", value);
}
felspar::u8view fostlib::exceptions::settings_fault::message() const noexcept {
    return "Internal Setting library fault";
}


#include <fost/exception/system_error.hpp>
fostlib::exceptions::system_error::system_error(
        felspar::u8view message, felspar::source_location loc) noexcept
: exception{message, std::move(loc)} {}
fostlib::exceptions::system_error::system_error(
        int ev,
        std::error_category const &cat,
        felspar::u8view message,
        felspar::source_location loc) noexcept
: exception{message, std::move(loc)} {
    insert(data(), "error", "code", ev);
    auto condition = cat.default_error_condition(ev);
    insert(data(), "error", "message", condition.message());
}
felspar::u8view fostlib::exceptions::system_error::message() const noexcept {
    return "System error";
}


#include <fost/exception/unexpected_eof.hpp>
fostlib::exceptions::unexpected_eof::unexpected_eof(
        felspar::source_location sl) noexcept
: exception{std::move(sl)} {}
fostlib::exceptions::unexpected_eof::unexpected_eof(
        const string &msg, felspar::source_location sl) noexcept
: exception{msg, std::move(sl)} {}
fostlib::exceptions::unexpected_eof::unexpected_eof(
        const string &msg, const string &f, felspar::source_location sl) noexcept
: exception{msg, std::move(sl)} {
    try {
        insert(data(), "filename", f);
    } catch (...) { fostlib::absorb_exception(); }
}
fostlib::exceptions::unexpected_eof::unexpected_eof(
        const string &msg,
        std::error_code error,
        felspar::source_location sl) noexcept
: exception{msg, std::move(sl)} {
    try {
        insert(data(), "error", error);
    } catch (...) { absorb_exception(); }
}
felspar::u8view fostlib::exceptions::unexpected_eof::message() const noexcept {
    return "Unexpected EOF";
}


#include <fost/exception/unexpected_nil.hpp>
fostlib::exceptions::unexpected_nil::unexpected_nil(
        felspar::source_location sl) noexcept
: exception{std::move(sl)} {}
fostlib::exceptions::unexpected_nil::unexpected_nil(
        const string &e, felspar::source_location sl) noexcept
: exception{e, std::move(sl)} {}
felspar::u8view fostlib::exceptions::unexpected_nil::message() const noexcept {
    return "Unexpected zero when processing Unicode stream";
}


#include <fost/exception/unicode_encoding.hpp>
fostlib::exceptions::unicode_encoding::unicode_encoding(
        const string &e, felspar::source_location sl) noexcept
: exception{e, std::move(sl)} {}
felspar::u8view fostlib::exceptions::unicode_encoding::message() const noexcept {
    return "Unicode encoding and decoding";
}

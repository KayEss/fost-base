#include "fost-core.hpp"
#include <fost/insert.hpp>
#include <fost/log.hpp>
#include <fost/threadsafe-store.hpp>

#include <utility>


namespace {


    fostlib::module const c_log{fostlib::c_fost, "logging"};


    /// Logging cannot be performed until after `main` has been entered
    std::mutex g_log_mtx;
    std::vector<std::unique_ptr<fostlib::log::logging_sink_function_type>>
            g_log_proc;
    std::atomic<fostlib::log::logging_sink_function_type *> g_log_sink = []() {
        g_log_proc.push_back(
                std::make_unique<fostlib::log::logging_sink_function_type>(
                        [](fostlib::log::message) {}));
        return g_log_proc.back().get();
    }();


}


/**
    ## fostlib::logging::message
*/


fostlib::json fostlib::coercer<fostlib::json, fostlib::log::message>::coerce(
        const fostlib::log::message &m) {
    json::object_t js, lv;

    lv["value"] = fostlib::coerce<json>(m.level);
    lv["name"] = m.name;

    js["when"] = fostlib::coerce<json>(m.when);
    js["module"] = fostlib::coerce<json>(m.module());
    js["level"] = std::move(lv);
    js["body"] = m.body;

    return js;
}


/**
    ## fostlib::log functions
*/


void fostlib::log::log(message m) { (*g_log_sink)(std::move(m)); }


void fostlib::log::logging_function(logging_sink_function_type fn) {
    {
        std::scoped_lock _{g_log_mtx};
        g_log_proc.push_back(
                std::make_unique<logging_sink_function_type>(std::move(fn)));
        g_log_sink = g_log_proc.back().get();
    }
    info(c_log, "New logging function in use");
}


/**
    ## fostlib::log::log_object
*/


fostlib::log::detail::log_object::log_object(
        const module &m, std::size_t level, felspar::u8string name)
: part(m), level(level), name(name) {}


fostlib::log::detail::log_object::log_object(log_object &&right)
: part(right.part),
  level(right.level),
  name(std::move(right.name)),
  log_message(std::move(right.log_message)) {}


fostlib::log::detail::log_object::~log_object() try {
    fostlib::log::log(part, level, name, log_message);
} catch (...) { absorb_exception(); }

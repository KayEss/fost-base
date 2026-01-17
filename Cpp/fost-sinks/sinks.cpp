#include <fost/sinks.hpp>


const fostlib::module fostlib::c_fost_sinks(c_fost, "sinks");


const fostlib::setting<fostlib::string> fostlib::c_log_sink_file_root(
        "sinks.cpp", "Log sinks", "Log files directory", "var/log/fost", true);

#include "fost-cli.hpp"
#include <felspar/cord/iostream.hpp>
#include <fost/cli>

#include <iostream>


namespace {
    const char *nl_space = "\n                            ";

    struct disp {
        typedef void result_type;
        std::ostream &channel;

        template<typename T>
        void operator()(T const &t) const {
            channel << nl_space << t << '\n' << std::endl;
        }
        void operator()(const fostlib::json::object_p &o) const {
            if (o->find(fostlib::string()) != o->end()) {
                fostlib::json copy = *o;
                fostlib::string m = fostlib::coerce<fostlib::string>(copy[""]);
                fostlib::jcursor("").del_key(copy);
                channel << nl_space << m << '\n';
                if (copy.size()) channel << copy;
                channel << std::endl;
            } else {
                channel << '\n' << *o << std::endl;
            }
        }
        void operator()(const fostlib::json::array_p &a) const {
            if (a->size() > 0 && (*a)[0].isatom()) {
                channel << nl_space << fostlib::coerce<fostlib::string>((*a)[0])
                        << '\n';
                for (std::size_t i(1); i != a->size(); ++i) {
                    channel << (*a)[i] << '\n';
                }
                channel << std::endl;
            } else {
                channel << '\n' << *a << std::endl;
            }
        }
    };

    class ostream_logger {
        const std::size_t log_level;
        const bool colour;
        std::ostream &channel;

      public:
        ostream_logger(const fostlib::json &conf)
        : log_level(fostlib::coerce<fostlib::nullable<int>>(conf["log-level"])
                            .value_or(fostlib::log::error_level_tag::level())),
          colour(fostlib::coerce<fostlib::nullable<bool>>(conf["color"])
                         .value_or(false)),
          channel(conf["channel"] == fostlib::json("stderr") ? std::cerr
                                                             : std::cout) {}
        bool operator()(fostlib::log::message m) {
            fostlib::simple_logger(std::move(m), log_level, colour, channel);
            return true;
        }
    };

    const fostlib::log::sink_function<ostream_logger> std_out("stdout");
}


void fostlib::simple_logger(
        log::message const &m,
        std::size_t const log_level,
        bool const colour,
        std::ostream &channel) {
    if (m.level >= log_level) {
        if (colour) {
            if (m.level <= fostlib::log::debug_level_tag::level() + 0x100) {
                channel << "\33[0;37m";
            } else if (m.level <= fostlib::log::info_level_tag::level() + 0x300) {
                channel << "\33[0;32m";
            } else if (m.level <= fostlib::log::warning_level_tag::level() + 0x2000) {
                channel << "\33[1;33m";
            } else if (m.level <= fostlib::log::error_level_tag::level() + 0x20000) {
                channel << "\33[0;31m";
            } else {
                channel << "\33[1;31m";
            }
        }
        channel << m.when << " " << m.name << " " << m.module();
        if (colour) {
            disp d{channel};
            m.body.apply_visitor(d);
            channel << "\33[0;39m";
        } else {
            channel << '\n' << m.body << std::endl;
        }
    }
}

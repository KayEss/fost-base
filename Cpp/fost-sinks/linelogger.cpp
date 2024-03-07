#include <fost/log>

#include <iostream>


namespace {


    struct line_logger {
        const std::size_t log_level;
        std::ostream &channel;

      public:
        line_logger(const fostlib::json &conf)
        : log_level(fostlib::coerce<fostlib::nullable<int>>(conf["log-level"])
                            .value_or(fostlib::log::error_level_tag::level())),
          channel(conf["channel"] == fostlib::json("stderr") ? std::cerr
                                                             : std::cout) {}
        bool operator()(const fostlib::log::message &m) {
            if (m.level >= log_level) {
                channel << fostlib::json::unparse(
                        fostlib::coerce<fostlib::json>(m), false)
                        << std::endl;
            }
            return true;
        }
    };


    const fostlib::log::sink_function<line_logger> line{"line"};


}

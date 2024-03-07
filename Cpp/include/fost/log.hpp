#ifndef FOST_LOG_HPP
#define FOST_LOG_HPP
#pragma once


#include <fost/module.hpp>
#include <fost/timestamp.hpp>
#include <fost/insert.hpp>
#include <fost/push_back.hpp>


#include <fost/log-message.hpp>
#include <fost/log-sinks.hpp>

#include <functional>
#include <type_traits>


namespace fostlib {


    namespace log {


        /// Log a logging message
        FOST_CORE_DECLSPEC
        void log(message);

        /// Add a message to the logs at a given level
        inline void
                log(const module &m,
                    std::size_t level,
                    felspar::u8string name,
                    const json &data) {
            log::log(message{m, level, name, data});
        }
        /// Add a message to the logs at a given level
        inline void
                log(const module &m,
                    std::size_t level,
                    felspar::u8string name,
                    json::array_t a) {
            log::log(message{m, level, name, a});
        }
        /// Add a message to the logs at a given level
        template<typename A, typename... J>
        inline void
                log(const module &m,
                    std::size_t level,
                    felspar::u8string name,
                    json::array_t array,
                    const A &a,
                    J &&...j) {
            push_back(array, fostlib::coerce<fostlib::json>(a));
            log(m, level, name, std::move(array), std::forward<J>(j)...);
        }


        /// Create an instance of this class to register a global sink
        template<typename F>
        class sink_function : detail::global_sink_base {
            /// Create the logging object itself
            std::shared_ptr<detail::global_sink_wrapper_base>
                    construct(const json &configuration) const {
                return std::make_shared<detail::global_sink_wrapper<F>>(
                        name(), configuration);
            }

          public:
            /// Create a global sink providing the configuration name
            sink_function(const string &name) : global_sink_base(name) {}

            /// The name of the sink used for configuration
            using global_sink_base::name;
        };


        namespace detail {
            class log_queue;

            /// Used to make the logging of JSON objects easier
            class FOST_CORE_DECLSPEC log_object {
                /// The module this is part of
                const module &part;
                /// The log level
                std::size_t level;
                /// The name of the log level
                felspar::u8string name;
                /// The log message being constructed
                json::object_t log_message;

              public:
                /// Start the log message
                log_object(const module &, std::size_t, felspar::u8string);
                /// Move constructor
                log_object(log_object &&);
                /// Send the constructed log message
                ~log_object();

                /// Log the value at the requested key
                template<typename P1, typename V>
                log_object &operator()(const P1 &p1, V &&v) {
                    log_message[p1] = coerce<json>(std::forward<V>(v));
                    return *this;
                }

                /// Log the message at the requested key path
                template<typename P1, typename P2, typename... P>
                log_object &operator()(const P1 &p1, P2 &&p2, P &&...p) {
                    insert(log_message[p1], std::forward<P2>(p2),
                           std::forward<P>(p)...);
                    return *this;
                }
            };
        }


        /// The function type used to process log messages. Note that the
        /// implementation must be thread safe
        using logging_sink_function_type =
                std::function<void(fostlib::log::message)>;

        /// Provide the logging sink function that is to be used
        void logging_function(logging_sink_function_type);

        /// Return a named logging function
        logging_sink_function_type
                named_logging_function(felspar::u8view, json const &);

        /// Return a logging function based on the provided JSON configuration.
        /// The resulting logging function will only be thread safe if the
        /// underlying logging are
        logging_sink_function_type
                configured_logging_function(json::array_t const &);

        /// Single function to set up logging given a configuration. Assumes
        /// that the underlying log functions are not thread safe so will add
        /// thread synchronisation
        // void load_logging_configuration(json::array_t const &);


/// Used to create a logging level
#define FSL_DEFINE_LOGGING_LEVEL(N, value) \
    const struct N##_level_tag { \
        static std::size_t level() { return value; } \
        static felspar::u8string name() { return #N; } \
        fostlib::log::detail::log_object \
                operator()(const fostlib::module &m) const { \
            return fostlib::log::detail::log_object(m, level(), name()); \
        } \
        void operator()(const fostlib::module &m, const char *msg) const { \
            fostlib::log::log(m, level(), name(), fostlib::json(msg)); \
        } \
        template<typename J> \
        void operator()(const fostlib::module &m, J &&j) const { \
            fostlib::log::log( \
                    m, level(), name(), \
                    fostlib::coerce<fostlib::json>(std::forward<J>(j))); \
        } \
        template<typename F, typename... J> \
        void operator()(const fostlib::module &m, F &&f, J &&...j) const { \
            fostlib::log::log( \
                    m, level(), name(), fostlib::json::array_t(), \
                    std::forward<F>(f), std::forward<J>(j)...); \
        } \
    } N = {}

        /// The debug level logger
        FSL_DEFINE_LOGGING_LEVEL(debug, 0x100u);
        /// The info level logger
        FSL_DEFINE_LOGGING_LEVEL(info, 0x400u);
        /// The warning level logger
        FSL_DEFINE_LOGGING_LEVEL(warning, 0x1000u);
        /// The error level logger
        FSL_DEFINE_LOGGING_LEVEL(error, 0x4000u);
        /// The critical level logger
        FSL_DEFINE_LOGGING_LEVEL(critical, 0x10000u);

        /// Can be used to choose all logging levels
        const struct all_level_tag {
            static std::size_t level() { return 0u; }
            static felspar::u8string name() { return "all"; }
        } all = {};


    }


}


#endif // FOST_LOG_HPP

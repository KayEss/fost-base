#ifndef FOST_COUNTER_HPP
#define FOST_COUNTER_HPP
#pragma once


#include <fost/log.hpp>
#include <atomic>


namespace fostlib {


    /// A single performance counter
    class FOST_CORE_DECLSPEC performance final {
        std::atomic<std::int64_t> count;
        const module *const parent;
        const jcursor path;

      public:
        /// Construct a performance counter
        performance(const module &, jcursor, std::int64_t value = 0);
        /// Construct a performance counter with extended path
        template<typename... Ss>
        performance(const module &m, Ss &&...ss)
        : performance(m, jcursor(std::forward<Ss>(ss)...)) {}
        /// Make movable
        performance(performance &&p);
        /// Destruct the performance counter
        ~performance();

        /// Increase the performance count
        std::int64_t operator++() { return ++count; }
        /// Add to the performance count
        std::int64_t operator+=(std::int64_t v) { return count += v; }
        /// Decrease the performance count
        std::int64_t operator--() { return --count; }

        /// The current value
        std::int64_t value() const { return count.load(); }
        auto value(std::int64_t v) { return count.exchange(v); }
        /// The module relative path
        const jcursor &rel_path() const { return path; }

        /// Return a JSON structure that describes all of the current
        /// counter values
        static json current();
        /// Return a vector of values with their locations
        static std::vector<std::pair<jcursor, std::int64_t>> values();
    };


    namespace log {
        /// Constant containing the performance log level
        const unsigned g_performance_level = 0x700u;
        /// Define a new logger level 'performance'
        FSL_DEFINE_LOGGING_LEVEL(perf, g_performance_level);
    }


    template<typename Duration>
    class performance_duration_hist {
        using sample_store = std::vector<std::pair<Duration, performance>>;
        sample_store samples;

      public:
        inline performance_duration_hist(
                Duration width,
                double factor,
                std::size_t buckets,
                fostlib::module const &perf_module,
                fostlib::jcursor perf_path,
                Duration offset = Duration{}) {
            auto last = offset;
            auto new_last = offset;
            for (std::size_t n = 0; n <= buckets; n++) {
                new_last = last + width;
                if (n < buckets) {
                    samples.push_back(std::make_pair(
                            new_last,
                            fostlib::performance{
                                    perf_module,
                                    fostlib::jcursor(
                                            perf_path,
                                            fostlib::string(
                                                    std::to_string(last.count())
                                                    + "_"
                                                    + std::to_string(
                                                            new_last.count())))}));
                } else {
                    samples.push_back(std::make_pair(
                            new_last,
                            fostlib::performance{
                                    perf_module,
                                    fostlib::jcursor(
                                            perf_path,
                                            fostlib::string(
                                                    std::to_string(last.count())
                                                    + "_infinite"))}));
                }
                last = new_last;
                width = Duration(
                        typename Duration::rep(width.count() * factor));
            }
        }

        void record(Duration tm) {
            auto into = std::upper_bound(
                    samples.begin(), samples.end(), tm,
                    [](auto &left, auto &right) { return left < right.first; });
            if (into == samples.end()) {
                ++samples.back().second;
            } else {
                ++into->second;
            }
        }
    };


}


#endif // FOST_COUNTER_HPP

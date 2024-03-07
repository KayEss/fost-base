#ifndef FOST_PROGRESS_BAR_CLI_HPP
#define FOST_PROGRESS_BAR_CLI_HPP
#pragma once


#include <fost/progress>

#include <future>


namespace fostlib::cli {


    namespace detail {


        FOST_CLI_DECLSPEC
        /// Return a suitable default prefix
        string prefix(const fostlib::progress::reading &);

        FOST_CLI_DECLSPEC
        /// Return a suitable default suffix
        string suffix(const fostlib::progress::reading &);


    }


    FOST_CLI_DECLSPEC
    /// Return a string of the requested length representing the reading
    string bar(const progress::reading &, std::size_t width);

    /// Monitor a future and report progress
    template<typename R, typename S, typename P>
    void
            monitor(fostlib::ostream &out,
                    std::future<R> &future,
                    S suffix,
                    P prefix,
                    std::chrono::milliseconds const delay =
                            std::chrono::milliseconds(50),
                    std::size_t const pips = 50) {
        while (future.wait_for(delay) != std::future_status::ready) {
            auto const current{progress::take_reading()};
            out << prefix(current) << bar(current, pips) << suffix(current)
                << '\r' << std::flush;
        }
        auto const current{progress::take_reading()};
        out << prefix(current) << bar(current, pips) << suffix(current)
            << std::endl;
    }
    template<typename R, typename S>
    void monitor(fostlib::ostream &out, std::future<R> &future, S suffix) {
        monitor(out, future, suffix, detail::prefix);
    }
    template<typename R>
    void monitor(fostlib::ostream &out, std::future<R> &future) {
        monitor(out, future, detail::suffix, detail::prefix);
    }


}


#endif // FOST_PROGRESS_BAR_CLI_HPP

#ifndef FOST_PROGRESS_HPP
#define FOST_PROGRESS_HPP
#pragma once


#include <fost/filesystem.hpp>
#include <fost/timestamp.hpp>

#include <set>


namespace fostlib {


    /// The type of the work measurement
    using work_amount = uint64_t;


    /// Record the progress towards completing a task
    class FOST_CORE_DECLSPEC progress {
        std::atomic<work_amount> now = {};
        work_amount last = {};

        /// Meta data about the work being done
        json meta = {};

        /// Common initialisation code
        void init();

      public:
        /// Progress recording which isn't explicitly part of a larger process
        /// for up to the specified number
        progress(json meta, work_amount upto);

        /// Progress recording for a file content
        progress(std::filesystem::path const &file);

        /// Allow tracking of removal of progress recorders. Not virtual as
        /// we're not going to sub-class this
        ~progress();

        /// Returns true if the current value is at least as much as the total
        bool is_complete() const { return now >= last; }

        /// Mark one step as having been completed
        work_amount operator++();

        /// Mark a certain amount of work as having been done
        progress &operator+=(work_amount amount);

        /// Return the current value of the progress
        work_amount current() const { return now; }

        /// The amount of work to be done
        work_amount total() const { return last; }

        /// A reading
        class reading {
            std::chrono::system_clock::time_point captured;

          public:
            /// Allow default constructing
            reading() : is_complete(true) {}
            /// A reading with a given value
            reading(bool c) : is_complete(c) {}
            /// A reading for a certain amount of work
            reading(json m,
                    bool c,
                    work_amount done,
                    work_amount outof,
                    std::chrono::system_clock::time_point time =
                            std::chrono::system_clock::now())
            : captured{time}, is_complete(c), work(outof), done(done), meta(m) {}

            /// The time that the reading was taken
            auto when() const noexcept { return captured; }

            /// Determine whether the progress is complete
            accessors<bool> is_complete;
            /// The amount of work we know needs to be done
            accessors<nullable<work_amount>> work;
            /// The amount of work we know has been done
            accessors<work_amount> done;
            /// Information about what we are processing
            accessors<json> meta;
        };

        static reading take_reading();
    };


}


#endif // FOST_PROGRESS_HPP

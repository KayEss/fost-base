#pragma once


#include <filesystem>


namespace fostlib {


    inline auto last_write_time_as_time_t(std::filesystem::path const &p) {
        auto const ftime = std::filesystem::last_write_time(p);
        return std::chrono::duration_cast<std::chrono::seconds>(
                       ftime.time_since_epoch())
                .count();
    }


}

#include <fost/sinks.panoptes.hpp>


namespace {
    const fostlib::module c_legacy("-unnamed-");
}


fostlib::log::detail::archive_pathname::archive_pathname()
: modulep(&c_legacy), max_size_kb(128) {}


fostlib::log::detail::archive_pathname::archive_pathname(const class module &m)
: modulep(&m), max_size_kb(128) {}


fostlib::log::detail::archive_pathname::fileloc_type
        fostlib::log::detail::archive_pathname::pathname(
                std::chrono::system_clock::time_point when) const {
    fostlib::string ts = fostlib::replace_all(coerce<string>(when), ":", null);
    auto const directory =
            coerce<std::filesystem::path>(c_log_sink_file_root.value())
            / coerce<std::filesystem::path>(ts.substr(0, 7))
            / coerce<std::filesystem::path>(ts.substr(8, 2));
    auto const data_path(
            directory
            / coerce<std::filesystem::path>(
                    modulep->as_string() + "/" + ts + ".jsonl"));
    if (not std::filesystem::exists(data_path.parent_path())) {
        std::filesystem::create_directories(data_path.parent_path());
    }
    date const day{when};
    fileloc_type fl = {day, data_path};
    return fl;
}


std::filesystem::path fostlib::log::detail::archive_pathname::operator()(
        std::chrono::system_clock::time_point when) {
    const date day{when};
    if (not fileloc) {
        fileloc = pathname(when);
    } else if (std::filesystem::exists(fileloc.value().pathname)) {
        uintmax_t size = std::filesystem::file_size(fileloc.value().pathname);
        if (rotate(size) || day != fileloc.value().date) {
            fileloc = pathname(when);
        }
    }
    return fileloc.value().pathname;
}


bool fostlib::log::detail::archive_pathname::rotate(uintmax_t size) {
    const uintmax_t kb = size >> 10u;
    return kb >= max_size_kb;
}

#include <fost/filesystem.hpp>
#include <fost/sinks.panoptes.hpp>

#include <fstream>


fostlib::log::panoptes::panoptes(const fostlib::json &) {}


bool fostlib::log::panoptes::operator()(const fostlib::log::message &m) {
    fostlib::string entry(
            fostlib::json::unparse(fostlib::coerce<fostlib::json>(m), false));
    const auto *modp = &m.module();

    std::filesystem::path filename;
    auto archive = logfile_pathnames.find(modp);
    if (archive == logfile_pathnames.end()) {
        filename =
                (logfile_pathnames[modp] =
                         detail::archive_pathname(m.module()))(m.when);
    } else {
        filename = (archive->second)(m.when);
    }

    std::ofstream file{filename, std::ios_base::out | std::ios_base::app};
    file << entry << std::endl;

    return true;
}

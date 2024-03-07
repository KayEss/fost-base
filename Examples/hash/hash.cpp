#include <fost/cli>
#include <fost/crypto>
#include <fost/file>
#include <fost/main>
#include <fost/progress-cli>

#include <future>


namespace {
    fostlib::string hash(std::filesystem::path const &file) {
        fostlib::digester hasher(fostlib::md5);
        hasher << file;
        return fostlib::coerce<fostlib::string>(
                fostlib::coerce<fostlib::hex_string>(hasher.digest()));
    }

    void process(std::filesystem::path const path) {
        if (std::filesystem::is_directory(path)) {
            for (fostlib::directory_iterator file(path);
                 file != fostlib::directory_iterator(); ++file) {
                process(*file);
            }
        } else {
            auto result = std::async([path]() { return hash(path); });
            while (result.wait_for(std::chrono::milliseconds{50})
                   != std::future_status::ready) {
                auto const current{fostlib::progress::take_reading()};
                std::cerr << "[" << fostlib::cli::bar(current, 38) << "] "
                          << path << "\r" << std::flush;
            }
            auto const current{fostlib::progress::take_reading()};
            std::cerr << "[" << fostlib::cli::bar(current, 38) << "] " << path
                      << "\r\n";
            std::cout << result.get() << "  " << path << std::endl;
        }
    }
}


FSL_MAIN("hash", "File hashing")
(fostlib::ostream &, fostlib::arguments &args) {
    for (std::size_t n(1); n < args.size(); ++n) {
        auto path(fostlib::coerce<std::filesystem::path>(args[n].value()));
        process(path);
    }
    return 0;
}

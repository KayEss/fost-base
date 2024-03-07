#include "fost-cli.hpp"
#include <fost/progress-cli>


fostlib::string
        fostlib::cli::bar(progress::reading const &r, std::size_t const width) {
    if (r.is_complete()) {
        return string(width, '.');
    } else if (not r.work().value_or(0)) {
        return string(width, '-');
    } else {
        const std::size_t dots(r.done() * width / *r.work());
        return string(dots, '.') + string(width - dots, ' ');
    }
}


fostlib::string fostlib::cli::detail::prefix(progress::reading const &) {
    return "[";
}


fostlib::string fostlib::cli::detail::suffix(progress::reading const &) {
    return "]";
}

#include "fost-core.hpp"

#include <fost/insert.hpp>
#include <fost/log.hpp>
#include <fost/progress.hpp>
#include <fost/unicode.hpp>

#include <fost/exception/file_error.hpp>

#include <mutex>


/**
 * ## fostlib::progress
 */


namespace {
    std::recursive_mutex g_lock;
    std::vector<fostlib::progress *> g_progress;
}


fostlib::progress::reading fostlib::progress::take_reading() {
    std::chrono::system_clock::time_point time =
            std::chrono::system_clock::now();
    fostlib::json::array_t metas;
    work_amount done{}, outof{};
    std::recursive_mutex g_lock;
    metas.reserve(g_progress.size());
    for (auto *p : g_progress) {
        metas.push_back(p->meta);
        done += p->now.load();
        outof += p->last;
    }
    return {std::move(metas), done >= outof, done, outof, time};
}


fostlib::progress::progress(json meta, work_amount upto)
: last{upto}, meta{std::move(meta)} {
    init();
}


fostlib::progress::progress(std::filesystem::path const &file) {
    insert(meta, "filename", file);
    try {
        int64_t bytes(coerce<int64_t>(std::filesystem::file_size(file)));
        last = bytes;
        insert(meta, "stat", "size", "bytes", bytes);
        std::time_t modified(fostlib::last_write_time_as_time_t(file));
        insert(meta, "stat", "modified", modified);
    } catch (std::filesystem::filesystem_error &e) {
        throw fostlib::exceptions::file_error(e.what(), coerce<string>(file));
    }
    init();
}


void fostlib::progress::init() {
    std::lock_guard<std::recursive_mutex> lock(g_lock);
    g_progress.push_back(this);
}


fostlib::progress::~progress() {
    std::lock_guard<std::recursive_mutex> lock(g_lock);
    auto const p{std::find(g_progress.begin(), g_progress.end(), this)};
    if (p != g_progress.end()) {
        g_progress.erase(p);
    } else {
        log::error(c_fost_base_core)(
                "function", "fostlib::progress::~progress")("meta", meta)(
                "error", "Not found in progress collection");
    }
}


fostlib::work_amount fostlib::progress::operator++() {
    ++now;
    return now;
}


fostlib::progress &fostlib::progress::operator+=(work_amount amount) {
    now += amount;
    return *this;
}

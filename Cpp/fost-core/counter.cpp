#include "fost-core.hpp"
#include <fost/counter.hpp>
#include <felspar/threading/set.hpp>


namespace {
    auto &counters() {
        static felspar::threading::tsset<const fostlib::performance *> c_counters;
        return c_counters;
    }
}


fostlib::performance::performance(const module &m, jcursor p, int64_t value)
: count(value), parent(&m), path(std::move(p)) {
    counters().insert_if_not_found(this);
}


fostlib::performance::performance(performance &&p)
: count(p.count.load()),
  parent(p.parent),
  path(p.path) // We can't move from this because the member is const
{
    counters().insert_if_not_found(this);
}


fostlib::performance::~performance() {
    counters().remove_if([this](auto v) { return v == this; });
}


fostlib::json fostlib::performance::current() {
    json ret;
    counters().for_each([&ret](auto v) {
        const auto value = v->value(); // Read one time
        if (value) (v->parent->as_jcursor() / v->path).insert(ret, value);
    });
    return ret;
}
auto fostlib::performance::values()
        -> std::vector<std::pair<jcursor, int64_t>> {
    std::vector<std::pair<jcursor, int64_t>> values;
    counters().for_each([&values](auto v) {
        auto const value = v->value();
        if (value) {
            values.emplace_back(v->parent->as_jcursor() / v->rel_path(), value);
        }
    });
    return values;
}

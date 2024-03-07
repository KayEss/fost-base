#include "fost-core.hpp"
#include <fost/workerpool.hpp>

#include <mutex>


using namespace fostlib;


struct fostlib::workerpool::implementation {
    std::mutex m_mutex;
    std::vector<std::shared_ptr<worker>> m_available;
    std::size_t m_peak;
};


fostlib::workerpool::workerpool() : impl(new implementation) {
    impl->m_peak = 0;
}
fostlib::workerpool::~workerpool() { delete impl; }


std::shared_ptr<worker> fostlib::workerpool::assign() {
    std::lock_guard<std::mutex> lock(impl->m_mutex);
    if (!impl->m_available.size())
        return std::shared_ptr<worker>(new worker);
    else {
        std::shared_ptr<worker> w = impl->m_available.back();
        impl->m_available.pop_back();
        return w;
    }
}
void fostlib::workerpool::replace(std::shared_ptr<worker> w) {
    std::lock_guard<std::mutex> lock(impl->m_mutex);
    impl->m_available.push_back(w);
    impl->m_peak = std::max(impl->m_peak, impl->m_available.size());
}


std::size_t fostlib::workerpool::available() {
    std::lock_guard<std::mutex> lock(impl->m_mutex);
    return impl->m_available.size();
}
std::size_t fostlib::workerpool::peak_used() {
    std::lock_guard<std::mutex> lock(impl->m_mutex);
    return impl->m_peak;
}

#include "fost-core-test.hpp"

#include <fost/dynlib>
#include <fost/main>


namespace {
    const fostlib::setting<fostlib::string> c_loadlib(
            "dynlib.cpp", "dynlib tests", "Load library", "fost-datetime", true);
}


FSL_TEST_SUITE(dynlib);


FSL_TEST_FUNCTION(datetime) {
    fostlib::ini_file base_settings_file("/etc/fost.conf");
    FSL_CHECK_NOTHROW(fostlib::dynlib(c_loadlib.value()));
}

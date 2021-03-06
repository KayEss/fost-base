/**
    Copyright 2009-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"
#include <fost/core>
#include <fost/insert>


FSL_TEST_SUITE(settings);


FSL_TEST_FUNCTION(load_base_settings_file) {
    try {
        FSL_CHECK(fostlib::setting<bool>::value(
                "Settings database tests", "Loaded base settings file"));
    } catch (fostlib::exceptions::exception &e) {
        fostlib::insert(
                e.data(), "resolution",
                "This test will fail if you don't have a global FOST settings "
                "file.\n"
                "This is at /etc/fost.conf on UNIX based systems and %windir% "
                "on Windows systems.\n"
                "The file contents needs to include the following text:\n"
                "   [Settings database tests]\n"
                "   Loaded base settings file=true");
        throw;
    }
}

#include <fost/test>
#include <fost/cli>
#include <fost/inifile.hpp>


namespace {
    fostlib::ini_file base_settings_file("/etc/fost.conf");
}


fostlib::setting<fostlib::string> const fostlib::test::c_files_folder(
        "fost-core-test/fost-core-test.cpp",
        "Tests",
        "Test file folder",
        fostlib::coerce<fostlib::string>(fostlib::unique_filename()),
        true);

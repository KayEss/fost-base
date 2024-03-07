#include "fost-core-test.hpp"
#include <fost/unicode>


FSL_TEST_SUITE(boost_filesystem_path);


FSL_TEST_FUNCTION(coercion_string) {
    std::filesystem::path p("somefilepath");
    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(p), "somefilepath");
}


FSL_TEST_FUNCTION(coercion_json) {
    std::filesystem::path p("somefilepath");
    fostlib::json j("somefilepath");
    FSL_CHECK_EQ(fostlib::coerce<fostlib::json>(p), j);
    FSL_CHECK_EQ(fostlib::coerce<std::filesystem::path>(j), p);
}

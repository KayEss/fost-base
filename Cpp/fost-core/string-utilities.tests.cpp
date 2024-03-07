#include "fost-core-test.hpp"
#include <fost/string>


FSL_TEST_SUITE(string_utilities);


FSL_TEST_FUNCTION(uuid) {
    fostlib::string id(fostlib::guid());
    /// These tests won't compile with libstdc++ due to deleted operator<<
    // FSL_CHECK_EQ(id[8], '-');
    // FSL_CHECK_EQ(id[13], '-');
}


FSL_TEST_FUNCTION(replace) {
    fostlib::string tstr("p1.p2.p3");
    FSL_CHECK_EQ(replace_all(tstr, ".", "-"), "p1-p2-p3");
}

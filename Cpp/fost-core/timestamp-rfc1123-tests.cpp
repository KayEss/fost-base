#include <fost/datetime>
#include <fost/test>


FSL_TEST_SUITE(timestamps_rfc1123);


FSL_TEST_FUNCTION(roundtrip) {
    auto const ts = fostlib::timestamp(2012, 8, 12, 15, 33, 24);
    fostlib::rfc1123_timestamp s1("Sun, 12 Aug 2012 15:33:24 +0000");
    FSL_CHECK_EQ(fostlib::coerce<fostlib::rfc1123_timestamp>(ts), s1);
    FSL_CHECK_EQ(
            fostlib::coerce<std::chrono::system_clock::time_point>(s1), ts);
}

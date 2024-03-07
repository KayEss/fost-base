#include <fost/datetime>
#include <fost/log>
#include <fost/test>
#include <fost/unicode>


FSL_TEST_SUITE(timestamps);


FSL_TEST_FUNCTION(basic) {
    auto const n = std::chrono::system_clock::now();
    fostlib::string iso(fostlib::coerce<fostlib::string>(n));
    /// These won't work on libstdc++ due to deleted operator<<
    // FSL_CHECK_EQ(iso[0], '2');
    // FSL_CHECK_EQ(iso[1], '0');
    // FSL_CHECK_EQ(iso[4], '-');

    FSL_CHECK_NOTHROW(fostlib::rfc1123_timestamp rfc1123(
            fostlib::coerce<fostlib::rfc1123_timestamp>(n)));
}


FSL_TEST_FUNCTION(json) {
    auto const n = fostlib::timestamp(2012, 10, 4, 13, 45);
    fostlib::json j(fostlib::coerce<fostlib::json>(n));
    FSL_CHECK(j.get<felspar::u8view>()); // Check we do have a string
    FSL_CHECK_EQ(j, fostlib::json("2012-10-04T13:45:00Z"));
    FSL_CHECK_EQ(fostlib::coerce<std::chrono::system_clock::time_point>(j), n);
}


FSL_TEST_FUNCTION(json_now) {
    auto const n = std::chrono::system_clock::now();
    fostlib::json j(fostlib::coerce<fostlib::json>(n));
    fostlib::log::debug(fostlib::c_fost_base_core)(
            "n", "string", fostlib::coerce<fostlib::string>(n))("n", "json", j);
    FSL_CHECK(j.get<felspar::u8view>());
    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(n).code_points(), 27u);
    // Coerce both to strings below to normalise differences in available
    // precision.
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(
                    fostlib::coerce<std::chrono::system_clock::time_point>(j)),
            fostlib::coerce<fostlib::string>(n));
}


FSL_TEST_FUNCTION(json_microsecond) {
    auto const n = fostlib::timestamp(2012, 10, 4, 13, 45, 3, 456789);
    fostlib::json j(fostlib::coerce<fostlib::json>(n));
    FSL_CHECK(j.get<felspar::u8view>());
    FSL_CHECK_EQ(j, fostlib::json("2012-10-04T13:45:03.456789Z"));
    FSL_CHECK_EQ(fostlib::coerce<std::chrono::system_clock::time_point>(j), n);
}


FSL_TEST_FUNCTION(parse_with_Z_short) {
    fostlib::string n("2014-01-13T10:45Z");
    FSL_CHECK_EQ(
            fostlib::coerce<std::chrono::system_clock::time_point>(n),
            fostlib::timestamp(2014, 1, 13, 10, 45));
}


FSL_TEST_FUNCTION(parse_without_Z_short) {
    fostlib::string n("2014-01-13T10:45");
    FSL_CHECK_EQ(
            fostlib::coerce<std::chrono::system_clock::time_point>(n),
            fostlib::timestamp(2014, 1, 13, 10, 45));
}


FSL_TEST_FUNCTION(parse_with_Z_long) {
    fostlib::string n("2014-01-13T10:45:34.456734Z");
    FSL_CHECK_EQ(
            fostlib::coerce<std::chrono::system_clock::time_point>(n),
            fostlib::timestamp(2014, 1, 13, 10, 45, 34, 456734));
}


FSL_TEST_FUNCTION(parse_without_Z_long) {
    fostlib::string n("2014-01-13T10:45:34.456734");
    FSL_CHECK_EQ(
            fostlib::coerce<std::chrono::system_clock::time_point>(n),
            fostlib::timestamp(2014, 1, 13, 10, 45, 34, 456734));
}


FSL_TEST_FUNCTION(parse_with_date) {
    fostlib::string n("2014-01-13");
    FSL_CHECK_EQ(
            fostlib::coerce<std::chrono::system_clock::time_point>(n),
            fostlib::timestamp(2014, 1, 13));
}

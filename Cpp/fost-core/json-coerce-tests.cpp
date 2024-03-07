#include "fost-core-test.hpp"


using namespace felspar::literals;


FSL_TEST_SUITE(json_coerce);


FSL_TEST_FUNCTION(nullable) {
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::json>(fostlib::nullable<fostlib::string>()),
            fostlib::json());
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::json>(
                    fostlib::nullable<fostlib::string>("t")),
            fostlib::json("t"));
}


FSL_TEST_FUNCTION(bool) {
    FSL_CHECK(not fostlib::coerce<bool>(fostlib::json("")));
    FSL_CHECK(fostlib::coerce<bool>(fostlib::json("x")));
    FSL_CHECK(not fostlib::coerce<bool>(fostlib::json(""_l)));
    FSL_CHECK(fostlib::coerce<bool>(fostlib::json("x"_l)));
}


FSL_TEST_FUNCTION(string) {
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::json>("hello"), fostlib::json("hello"));
    const auto forwarder = [](auto &&c) {
        return fostlib::coerce<fostlib::json>(std::forward<decltype(c)>(c));
    };
    FSL_CHECK_EQ(forwarder("hello"), fostlib::json("hello"));
}


FSL_TEST_FUNCTION(int64_t) {
    FSL_CHECK_EQ(fostlib::coerce<int64_t>("123"_l), int64_t(123));
}


FSL_TEST_FUNCTION(object) {
    FSL_CHECK_EQ(
            fostlib::json(fostlib::json::object_t()),
            fostlib::coerce<fostlib::json>(fostlib::json::object_t()));
}


FSL_TEST_FUNCTION(array) {
    FSL_CHECK_EQ(
            fostlib::json(fostlib::json::array_t()),
            fostlib::coerce<fostlib::json>(fostlib::json::array_t()));
}


FSL_TEST_FUNCTION(optional) {
    FSL_CHECK_EQ(
            fostlib::json(),
            fostlib::coerce<fostlib::json>(std::optional<int>{}));
    FSL_CHECK_EQ(
            fostlib::json(42),
            fostlib::coerce<fostlib::json>(std::optional<int>{42}));
    FSL_CHECK_EQ(
            std::optional<int>{},
            fostlib::coerce<std::optional<int>>(fostlib::json()));
    FSL_CHECK_EQ(
            std::optional<int>{42},
            fostlib::coerce<std::optional<int>>(fostlib::json(42)));
}


FSL_TEST_FUNCTION(holding_pen) {
    FSL_CHECK_EQ(
            fostlib::json(),
            fostlib::coerce<fostlib::json>(felspar::memory::holding_pen<int>{}));
    FSL_CHECK_EQ(
            fostlib::json(42),
            fostlib::coerce<fostlib::json>(
                    felspar::memory::holding_pen<int>{42}));
    FSL_CHECK_EQ(
            felspar::memory::holding_pen<int>{},
            fostlib::coerce<felspar::memory::holding_pen<int>>(fostlib::json()));
    FSL_CHECK_EQ(
            felspar::memory::holding_pen<int>{42},
            fostlib::coerce<felspar::memory::holding_pen<int>>(
                    fostlib::json(42)));
}

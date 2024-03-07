#include <fost/test>
#include <fost/progress-cli>


FSL_TEST_SUITE(progress_bar);


FSL_TEST_FUNCTION(bar) {
    FSL_CHECK_EQ(
            "..........",
            fostlib::cli::bar(fostlib::progress::reading(true), 10));
    FSL_CHECK_EQ(
            "----------",
            fostlib::cli::bar(fostlib::progress::reading(false), 10));
    FSL_CHECK_EQ(
            "----------",
            fostlib::cli::bar(
                    fostlib::progress::reading(fostlib::json(), false, 0, 0),
                    10));
    FSL_CHECK_EQ(
            "          ",
            fostlib::cli::bar(
                    fostlib::progress::reading(fostlib::json(), false, 0, 10),
                    10));
    FSL_CHECK_EQ(
            "...       ",
            fostlib::cli::bar(
                    fostlib::progress::reading(fostlib::json(), false, 3, 10),
                    10));
    FSL_CHECK_EQ(
            ".......   ",
            fostlib::cli::bar(
                    fostlib::progress::reading(fostlib::json(), false, 7, 10),
                    10));
}

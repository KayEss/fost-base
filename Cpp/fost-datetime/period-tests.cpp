#include <fost/datetime>
#include <fost/test>


FSL_TEST_SUITE(period);


FSL_TEST_FUNCTION(constructors) {
    fostlib::test::default_copy_constructable<fostlib::period<void>>();
    fostlib::test::default_copy_constructable<fostlib::period<int>>();
    fostlib::test::default_copy_constructable<fostlib::period<fostlib::string>>();
    fostlib::period<void> p1(
            fostlib::timestamp(2010, 1, 1), std::chrono::system_clock::now());
}


// FSL_TEST_FUNCTION( basic ) {
// }


// FSL_TEST_FUNCTION( json ) {
// }

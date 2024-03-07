#include "fost-core-test.hpp"


FSL_TEST_SUITE(exceptions);


FSL_TEST_FUNCTION(not_implemented) {
    FSL_CHECK_EXCEPTION(
            throw fostlib::exceptions::not_implemented(
                    "Not implemented exception thrown"),
            fostlib::exceptions::not_implemented &);
}

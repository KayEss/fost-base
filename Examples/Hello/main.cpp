#include <fost/cli>
#include <fost/main.hpp>


FSL_MAIN("Hello", "Hello world!")(fostlib::ostream &, fostlib::arguments &) {
    return 0;
}

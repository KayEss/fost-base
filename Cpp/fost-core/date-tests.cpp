#include <fost/datetime>
#include <fost/exception/parse_error.hpp>
#include <felspar/test.hpp>


namespace {


    auto const dts = felspar::testsuite("dates");


    auto const ctrs = dts.test("constructors", [](auto check) {
        check(fostlib::date{}) == fostlib::date{};
        check(fostlib::date{2009, 02, 29}) == fostlib::date{2009, 03, 01};
    });


    auto const js = dts.test("json", [](auto check) {
        auto const cj = [&](auto &&d, auto &&j) {
            check(fostlib::coerce<fostlib::json>(d)) == fostlib::json(j);
            check(d) == fostlib::coerce<fostlib::date>(fostlib::json(j));
        };

        cj(fostlib::date{1970, 9, 3}, "1970-09-03");
        cj(fostlib::date{1998, 2, 14}, "1998-02-14");
        cj(fostlib::date{2009, 1, 1}, "2009-01-01");

        check([]() {
            fostlib::coerce<fostlib::date>(fostlib::json::parse("{}"));
        })
                .throws(fostlib::exceptions::parse_error{
                        "With non-string JSON", "{}\n"});
    });


}

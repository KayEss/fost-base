#include <fost/datetime>
#include <fost/insert>
#include <felspar/test.hpp>


using namespace std::literals::chrono_literals;


namespace {
    auto const timediff = felspar::testsuite("timediff");


    auto const tjwh = timediff.test("to_json_with_hours", [](auto check) {
        auto const td = 2h + 13min + 45s;
        fostlib::json jtd(fostlib::coerce<fostlib::json>(td));
        check(jtd["hours"]) == fostlib::json(2);
        check(jtd["minutes"]) == fostlib::json(13);
        check(jtd["seconds"]) == fostlib::json(45);
    });


    auto const thwoh = timediff.test("to_json_without_hours", [](auto check) {
        auto const td = 13min + 45s;
        fostlib::json jtd(fostlib::coerce<fostlib::json>(td));
        check(jtd.has_key("hours")).is_falsey();
        check(jtd["minutes"]) == fostlib::json(13);
        check(jtd["seconds"]) == fostlib::json(45);
    });


    auto const tjwf = timediff.test("to_json_with_fraction", [](auto check) {
        auto const td = 13min + 45s + 123456us;
        fostlib::json jtd(fostlib::coerce<fostlib::json>(td));
        check(jtd.has_key("hours")).is_falsey();
        check(jtd["minutes"]), fostlib::json(13);
        check(jtd["seconds"]), fostlib::json(45.123456);
    });


    auto const fjh = timediff.test("from_json_hours", [](auto check) {
        fostlib::json td;
        fostlib::insert(td, "hours", 3);
        check(fostlib::coerce<std::chrono::hours>(td)) == std::chrono::hours{3};
    });


    auto const fjm = timediff.test("from_json_minutes", [](auto check) {
        fostlib::json td;
        fostlib::insert(td, "minutes", 5);
        check(fostlib::coerce<std::chrono::minutes>(td))
                == std::chrono::minutes{5};
    });


    auto const fjs = timediff.test("from_json_seconds", [](auto check) {
        fostlib::json td;
        fostlib::insert(td, "seconds", 25.00034);
        check(fostlib::coerce<std::chrono::microseconds>(td))
                == std::chrono::seconds{25} + std::chrono::microseconds{340};
    });


}

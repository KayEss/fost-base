#ifndef FOST_TEST_HPP
#define FOST_TEST_HPP
#pragma once


#include <fost/pointers>
#include <fost/exception.hpp>
#include <fost/string.hpp>

#include <fost/insert>
#include <fost/log>

#include <felspar/test.hpp>


namespace fostlib {


    extern const module c_fost_base_test;


    namespace test {


        /// Location that tests can use on the filesystem for writing
        /// and loading files from. Individual tests should use their own
        /// folder within this to ensure that they don't interfere with
        /// other tests.
        extern setting<string> const c_files_folder;

        /// Control verbosity of test output
        extern setting<bool> const c_output_verbose;
        /// Whether the test run should continue after a failure
        extern setting<bool> const c_output_continue;
        /// The time before a warning is generated for a long running
        /// test
        extern setting<double> const c_output_warning_test_duration;


    }


}


#ifdef FOST_OS_WINDOWS
#define FSL_TEST_EXPORT __declspec(dllexport)
#else
#define FSL_TEST_EXPORT
#endif


#define FSL_TEST_SUITE(name) auto const g_suite = felspar::testsuite(#name)

#define FSL_TEST_FUNCTION(name) \
    namespace { \
        const struct FSL_TEST_EXPORT test_##name { \
            test_##name() { g_suite.test(#name, &outer); } \
\
          private: \
            static void outer(felspar::test::injected) { inner(); } \
            static void inner(); \
        } g_test##name; \
    } \
    void ::test_##name::inner()


#define FSL_CHECK(condition) felspar::test::injected{}(condition).is_truthy()

#define FSL_CHECK_NULL(condition) \
    felspar::test::injected{}(condition) == fostlib::null

#define FSL_CHECK_EQ(left, right) felspar::test::injected{}(left) == right

#define FSL_CHECK_NEQ(left, right) felspar::test::injected{}(left) != right

#define FSL_CHECK_EXCEPTION(code, except) \
    felspar::test::injected{}([&]() { code; }).template throws_type<except>()

#define FSL_CHECK_NOTHROW(code) \
    { code; }

#define FSL_CHECK_ERROR(left, right, error) \
    felspar::test::injected{}(left) >= right - error; \
    felspar::test::injected{}(right) <= right + error


namespace fostlib {


    namespace test {


        /**
            Abstracted tests
        */


        template<typename T>
        void default_copy_constructable() {
            felspar::test::injected{}(T()) == T();

            T def;
            def = T();
            felspar::test::injected{}(def) == T();
        }

        // template<typename T>
        // void default_isnull() {
        //     T def;
        //     FSL_CHECK(def.isnull());
        //     FSL_CHECK(def == fostlib::null);
        //     FSL_CHECK(!(def != fostlib::null));
        //     FSL_CHECK(fostlib::null == def);
        //     FSL_CHECK(!(fostlib::null != def));
        //     FSL_CHECK_EXCEPTION(def.value(), exceptions::null &);
        // }


    }


}


#endif // FOST_TEST_HPP

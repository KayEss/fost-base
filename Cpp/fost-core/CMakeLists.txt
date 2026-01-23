
add_library(fost-core
        atexit.cpp
        base32.cpp
        coerce.cpp
        counter.cpp
        dynlib.cpp
        exception.cpp
        file.cpp
        jcursor.cpp
        json.cpp
        json-coerce.cpp
        json-order.cpp
        json-parse.cpp
        json-sloppy-parse.cpp
        json-unparse.cpp
        log.cpp
        module.cpp
        pool.cpp
        progress.cpp
        setting.cpp
        string.cpp
        string-ascii.cpp
        string-tagged.cpp
        string-utilities.cpp
        thread.cpp
        time.cpp
        timestamp.cpp
        timezone.cpp
        unicode.cpp
    )
target_include_directories(fost-core PUBLIC ../include)
target_compile_options(fost-core PUBLIC -fdiagnostics-color)
if("${toolset}" STREQUAL "gcc")
    target_compile_options(fost-core PUBLIC -Wno-deprecated-declarations)
endif()
target_link_libraries(fost-core ${CMAKE_THREAD_LIBS_INIT} dl
    Boost::date_time Boost::filesystem Boost::spirit Boost::system Boost::uuid
    felspar-cord felspar-test felspar-threading)
install(TARGETS fost-core LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY ../include/fost DESTINATION include)

if(TARGET check)
    add_test_run(check fost-core TESTS
            base32.tests.cpp
            char-encoding-tests.cpp
            coercion-int-tests.cpp
            coercion-tests.cpp
            date-tests.cpp
            exception-tests.cpp
            file-tests.cpp
            filesystem-path.tests.cpp
            hex-tests.cpp
            jcursor-tests.cpp
            json-array-basic-tests.cpp
            json-atom-basic-tests.cpp
            json-coerce-tests.cpp
            json-iterator-tests.cpp
            json-jcursor-tests.cpp
            json-object-basic-tests.cpp
            json-order.tests.cpp
            json-parse-tests.cpp
            json-sloppy-parse-tests.cpp
            json-to-tests.cpp
            log-tests.cpp
            module.tests.cpp
            nullable.tests.cpp
            progress-tests.cpp
            push_back-tests.cpp
            string-tagged-tests.cpp
            string-tests.cpp
            string-utilities.tests.cpp
            timediff-tests.cpp
            timestamp-rfc1123-tests.cpp
            timestamp-tests.cpp
            trim.tests.cpp
            unicode.tests.cpp
        )
endif()

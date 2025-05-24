add_library(fost-core-test
        fost-core-test.cpp
    )
target_link_libraries(fost-core-test PUBLIC fost-cli)


if(TARGET stress)
    add_test_run(stress fost-core fost-cli fost-core-test TESTS
            dynlib.cpp
            settings.cpp
            thread.cpp
            workerpool.cpp
        )

    add_executable(fost-core-test-fileio EXCLUDE_FROM_ALL
            file-io.cpp
        )
    target_link_libraries(fost-core-test-fileio fost-cli)
    add_custom_command(TARGET fost-core-test-fileio
            POST_BUILD COMMAND fost-core-test-fileio fost-core-test-fileio.txt)
    add_dependencies(stress fost-core-test-fileio)
    add_test(NAME fost-core-test-fileio COMMAND fost-core-test-fileio)
endif()

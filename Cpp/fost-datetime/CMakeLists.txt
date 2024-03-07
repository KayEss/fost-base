add_library(fost-datetime
        timer.cpp
    )
target_link_libraries(fost-datetime fost-core)
set_target_properties(fost-datetime PROPERTIES DEBUG_POSTFIX "-d")
install(TARGETS fost-datetime LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

if(TARGET check)
    add_test_run(check fost-datetime TESTS
            period-tests.cpp
            time_period.tests.cpp
        )
endif()

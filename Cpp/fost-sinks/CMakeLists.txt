add_library(fost-sinks
        linelogger.cpp
        panoptes.cpp
        panoptes.pathname.cpp
        sinks.cpp
    )
target_link_libraries(fost-sinks fost-core)
set_target_properties(fost-sinks PROPERTIES DEBUG_POSTFIX "-d")
install(TARGETS fost-sinks LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

if(TARGET check)
    add_test_run(check fost-sinks TESTS
            panoptes.pathname.tests.cpp
        )
endif()

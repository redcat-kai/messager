include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(server_tests
    test_server.cxx
    $<TARGET_OBJECTS:server_obj>
)

target_link_libraries(server_tests
    PRIVATE
        common::common
        GTest::gtest_main
)

if(TARGET project_options)
    target_link_libraries(server_tests PRIVATE project_options project_warnings)
endif()

include(GoogleTest)
gtest_discover_tests(server_tests)

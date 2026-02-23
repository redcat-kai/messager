include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(common_tests
    test_main.cxx
)

target_link_libraries(common_tests
    PRIVATE
        common
        GTest::gtest_main
)

if(TARGET project_options)
    target_link_libraries(common_tests PRIVATE project_options project_warnings)
endif()

include(GoogleTest)
gtest_discover_tests(common_tests)

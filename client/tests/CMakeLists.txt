include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(client_tests
    test_client.cxx
    $<TARGET_OBJECTS:client_obj>
)

target_include_directories(client_tests
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
)


target_link_libraries(client_tests
    PRIVATE
        common::common
        GTest::gtest_main
)

if(TARGET project_options)
    target_link_libraries(client_tests PRIVATE project_options project_warnings)
endif()

include(GoogleTest)
gtest_discover_tests(client_tests)

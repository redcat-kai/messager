cmake_minimum_required(VERSION 3.23)
project(common LANGUAGES CXX)

# ---- Standalone fallback ----
if(NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # Built as subdirectory — ProjectOptions already included by parent
else()
    # Standalone build
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    include(CTest)

    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ProjectOptions.cmake")
        include(ProjectOptions)
    endif()
endif()

# ---- Library ----
add_library(common STATIC
    src/connection.cxx
)

add_library(common::common ALIAS common)

target_include_directories(common
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(common PUBLIC cxx_std_23)

# Attach repo-wide options if available
if(TARGET project_options)
    target_link_libraries(common PRIVATE project_options project_warnings)
endif()

# ---- Tests ----
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

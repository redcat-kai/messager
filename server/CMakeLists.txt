cmake_minimum_required(VERSION 3.23)
project(server LANGUAGES CXX)

# Standalone fallback
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    include(CTest)

    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
    include(ProjectOptions)

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common common_build)
endif()

# ---- Compile server sources to object files ----
add_library(server_obj OBJECT
    src/server.cxx
)

target_link_libraries(server_obj
    PUBLIC common::common
)

target_compile_features(server_obj PUBLIC cxx_std_23)

if(TARGET project_options)
    target_link_libraries(server_obj PRIVATE project_options project_warnings)
endif()

# ---- Final executable ----
add_executable(server
    main.cxx
    $<TARGET_OBJECTS:server_obj>
)

target_link_libraries(server
  PRIVATE common::common
)
if(TARGET project_options)
    target_link_libraries(server PRIVATE project_options project_warnings)
endif()

# ---- Tests ----
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

cmake_minimum_required(VERSION 3.23)
project(messager LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(CTest)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ProjectOptions)

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/client/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/server/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/common/compile_commands.json
)

# formatting CI
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_CXX_FILES
        ${CMAKE_SOURCE_DIR}/common/*.cxx
        ${CMAKE_SOURCE_DIR}/common/src/*.cxx
        ${CMAKE_SOURCE_DIR}/common/tests/*.cxx
        ${CMAKE_SOURCE_DIR}/common/include/*.h
        ${CMAKE_SOURCE_DIR}/client/*.cxx
        ${CMAKE_SOURCE_DIR}/client/src/*.cxx
        ${CMAKE_SOURCE_DIR}/client/tests/*.cxx
        ${CMAKE_SOURCE_DIR}/client/include/*.h
        ${CMAKE_SOURCE_DIR}/server/*.cxx
        ${CMAKE_SOURCE_DIR}/server/src/*.cxx
        ${CMAKE_SOURCE_DIR}/server/tests/*.cxx
        ${CMAKE_SOURCE_DIR}/server/include/*.h
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${ALL_CXX_FILES}
        COMMENT "Running clang-format on all source files"
    )
endif()

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")

    file(GLOB_RECURSE ALL_CXX_FILES
        ${CMAKE_SOURCE_DIR}/common/src/*.cxx
        ${CMAKE_SOURCE_DIR}/client/src/*.cxx
        ${CMAKE_SOURCE_DIR}/server/src/*.cxx
        ${CMAKE_SOURCE_DIR}/common/tests/*.cxx
        ${CMAKE_SOURCE_DIR}/client/tests/*.cxx
        ${CMAKE_SOURCE_DIR}/server/tests/*.cxx
    )

    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_EXE}
                -p ${CMAKE_BINARY_DIR}
                -warnings-as-errors=*
                ${ALL_CXX_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy on all .cxx files"
        VERBATIM
    )
else()
    message(WARNING "clang-tidy not found")
endif()

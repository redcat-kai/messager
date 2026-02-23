add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)

# -------- Warnings --------
if (MSVC)
    target_compile_options(project_warnings INTERFACE /W4 /permissive-)
else()
    target_compile_options(project_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
    )
endif()

# -------- Debug flags --------
target_compile_options(project_options INTERFACE
    $<$<CONFIG:Debug>:
        -g3
        -O0
        -fno-omit-frame-pointer
    >
)

# -------- Sanitizers (Clang/GCC only) --------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")

    target_compile_options(project_options INTERFACE
        $<$<CONFIG:Debug>:
            -fsanitize=address
            -fsanitize=undefined
        >
    )

    target_link_options(project_options INTERFACE
        $<$<CONFIG:Debug>:
            -fsanitize=address
            -fsanitize=undefined
        >
    )

endif()

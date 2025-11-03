cmake_minimum_required(VERSION 3.20)
project(lexer)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wunused -Wextra -pthread")
add_compile_options(
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O2>
)

option(LEXER_BUILD_TESTS "Build tests for lexer" ON)
if (LEXER_BUILD_TESTS)
    enable_testing()
endif ()

add_subdirectory(external)
add_subdirectory(libs)
add_subdirectory(tools)

add_library(${PROJECT_NAME}
        INTERFACE
)

target_link_libraries(${PROJECT_NAME}
        INTERFACE
        lexer_core
        lexer_tokenizer
)

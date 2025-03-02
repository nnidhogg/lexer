cmake_minimum_required(VERSION 3.20)
project(lexer)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wunused -Wextra -pthread")
add_compile_options(
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O2>
)
set(CMAKE_CXX_STANDARD 23)

option(LEXER_BUILD_TESTS "Build tests for lexer" ON)
if (LEXER_BUILD_TESTS)
    enable_testing()
endif ()

add_subdirectory(external)
add_subdirectory(libs)

add_library(${PROJECT_NAME}
        src/builder.cpp
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        include
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        lexer_boost
        PUBLIC
        lexer_common
        lexer_regex
        lexer_dfa
        lexer_nfa
)

if (LEXER_BUILD_TESTS)
    add_executable(${PROJECT_NAME}_tests
            tests/lexer_test.cpp
    )

    target_link_libraries(${PROJECT_NAME}_tests
            PRIVATE
            ${PROJECT_NAME}
            lexer_dfa_tools
            lexer_nfa_tools
            gtest_main
    )

    add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)

    target_compile_definitions(${PROJECT_NAME}_tests PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")
endif ()

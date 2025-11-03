cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0097 NEW)
project(lexer_tokenizer)

add_library(${PROJECT_NAME}
        INTERFACE
)

target_include_directories(${PROJECT_NAME}
        INTERFACE
        include
)

target_link_libraries(${PROJECT_NAME}
        INTERFACE
        lexer_core
)

if (LEXER_BUILD_TESTS)
    add_executable(${PROJECT_NAME}_tests
            tests/tokenizer_test.cpp
    )

    target_link_libraries(${PROJECT_NAME}_tests
            PRIVATE
            ${PROJECT_NAME}
            lexer_core
            lexer_regex
            gtest_main
    )

    add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)

    target_compile_definitions(${PROJECT_NAME}_tests PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")
endif ()

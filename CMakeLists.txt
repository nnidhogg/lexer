cmake_minimum_required(VERSION 3.22)
project(lexer)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wunused -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CMAKE_CXX_STANDARD 23)

option(ENABLE_TESTS "Enable tests" ON)
if (ENABLE_TESTS)
    enable_testing()
endif ()

add_subdirectory(external)
add_subdirectory(libs)

add_executable(${PROJECT_NAME} main/main.cpp)

target_link_libraries(${PROJECT_NAME}
        nfa
        nfa_tools
        regex
)

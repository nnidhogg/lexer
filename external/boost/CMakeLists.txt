cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0097 NEW)
project(boost)

add_library(${PROJECT_NAME}_hash
        INTERFACE
)

target_include_directories(${PROJECT_NAME}_hash
        INTERFACE
        ${CMAKE_SOURCE_DIR}/external/boost/config/include
        ${CMAKE_SOURCE_DIR}/external/boost/describe/include
        ${CMAKE_SOURCE_DIR}/external/boost/mp11/include
        ${CMAKE_SOURCE_DIR}/external/boost/container_hash/include
)

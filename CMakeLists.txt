cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(simple_config)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
set(SIMPLE_CONFIG_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

include(cmake/nlohmann_json.cmake)
include(cmake/common.cmake)


if (CMAKE_DEBUG)
    message(STATUS "simple_config CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
endif ()

add_library(simple_config STATIC
        include/simple_config/config.h
        src/config.cpp
)
if (CMAKE_DEBUG)
    message(STATUS "simple_config SIMPLE_COLOR_INCLUDE ${SIMPLE_COLOR_INCLUDE}")
    message(STATUS "simple_config CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
    message(STATUS "simple_config SIMPLE_CONFIG_INCLUDE ${SIMPLE_CONFIG_INCLUDE}")
    message(STATUS "simple_config COMMON_INCLUDE ${COMMON_INCLUDE}")
    message(STATUS "simple_config NLOHMANN_JSON_INCLUDE ${NLOHMANN_JSON_INCLUDE}")
endif ()

target_include_directories(simple_config
        PUBLIC
        ${SIMPLE_CONFIG_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
)

install(TARGETS simple_config DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

option(NO_SIMPLE_CONFIG_TESTS "simple_config Disable Current tests" OFF)

if (NOT NO_SIMPLE_CONFIG_TESTS)
    include(FetchContent)
    FetchContent_Declare(Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.4.0
    )
    FETCHCONTENT_GETPROPERTIES(Catch2)
    FETCHCONTENT_MAKEAVAILABLE(Catch2)

    add_subdirectory(test)
endif ()
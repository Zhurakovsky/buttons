cmake_minimum_required(VERSION 3.0.0)

project(logger VERSION 0.0.1 LANGUAGES CXX)

if(NOT DEFINED COMMON_API_DIR)
    set(COMMON_API_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pub")
endif()

message(STATUS "Using commmon-api in ${COMMON_API_DIR}")

option(BUILD_TESTS "Build test" ON)
option(BUTTONS_BUILD_TESTS "Build blinker" ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -lArduiPi_OLED")

include_directories(
    ${COMMON_API_DIR}
)

set(PUBLIC_HEADERS
    "${COMMON_API_DIR}/ILogger.hpp"
)

set(HEADERS
    ${PUBLIC_HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Logger.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/LogHandler.hpp"
)

set(SOURCES
    ${HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Logger.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/LogHandler.cpp"
)

add_library(${PROJECT_NAME} SHARED "${SOURCES}")
target_link_libraries(${PROJECT_NAME}
    pthread
)

target_include_directories(${PROJECT_NAME} INTERFACE ${COMMON_API_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER ${PUBLIC_HEADERS}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)


if(BUILD_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
endif()
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/app")


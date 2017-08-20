cmake_minimum_required(VERSION 2.8)

project(Buttons LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

include_directories("${CMAKE_SOURCE_DIR}/src")

set(HEADERS
    "${CMAKE_SOURCE_DIR}/src/buttonListener.hpp"
#    "${CMAKE_SOURCE_DIR}/src/MsgProcessor.hpp"
#    "${CMAKE_SOURCE_DIR}/src/ThreadPool.hpp"
)

set(SOURCES
    ${HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/buttonListener.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} bcm2835 rt pthread)

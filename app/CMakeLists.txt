#cmake_minimum_required(VERSION 2.8)

#project(Buttons LANGUAGES CXX)


#SET(TOOLROOT /home/yzhurakovskyi/opt/gcc-4.7-linaro-rpi-gnueabihf/arm-linux-gnueabihf)

# specify the cross compiler
#SET(CMAKE_C_COMPILER   ${TOOLROOT}/bin/gcc)
#SET(CMAKE_CXX_COMPILER ${TOOLROOT}/bin/g++)


#SET(CMAKE_SYSTEM_NAME Linux) 

option(BUTTONS_BUILD_TESTS "Build blinker" ON)

# -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -lbcm2835 -lArduiPi_OLED")

include_directories("${CMAKE_SOURCE_DIR}/app/src")

set(APP_HEADERS
    "${CMAKE_SOURCE_DIR}/app/src/buttonListener.hpp"
    "${CMAKE_SOURCE_DIR}/app/src/commontypes.hpp"
    "${CMAKE_SOURCE_DIR}/app/src/menuitem.hpp"
    "${CMAKE_SOURCE_DIR}/app/src/menubuilder.hpp"
    "${CMAKE_SOURCE_DIR}/app/src/configparser.hpp"
    "${CMAKE_SOURCE_DIR}/app/src/displayoled.hpp"
)

set(APP_SOURCES
    ${APP_HEADERS}
    "${CMAKE_SOURCE_DIR}/app/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/app/src/buttonListener.cpp"
    "${CMAKE_SOURCE_DIR}/app/src/menuitem.cpp"
    "${CMAKE_SOURCE_DIR}/app/src/menubuilder.cpp"
    "${CMAKE_SOURCE_DIR}/app/src/configparser.cpp"
    "${CMAKE_SOURCE_DIR}/app/src/displayoled.cpp"
)

add_executable(Buttons ${APP_SOURCES})

target_link_libraries(Buttons bcm2835 rt pthread logger)

if(BUTTONS_BUILD_TESTS)
    add_subdirectory("${CMAKE_SOURCE_DIR}/app/testapp")
endif()

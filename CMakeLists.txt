cmake_minimum_required(VERSION 2.8)

project(Buttons LANGUAGES CXX)


#SET(TOOLROOT /home/yzhurakovskyi/opt/gcc-4.7-linaro-rpi-gnueabihf/arm-linux-gnueabihf)

# specify the cross compiler
#SET(CMAKE_C_COMPILER   ${TOOLROOT}/bin/gcc)
#SET(CMAKE_CXX_COMPILER ${TOOLROOT}/bin/g++)


#SET(CMAKE_SYSTEM_NAME Linux) 


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s")

include_directories("${CMAKE_SOURCE_DIR}/src")

set(HEADERS
    "${CMAKE_SOURCE_DIR}/src/buttonListener.hpp"
    "${CMAKE_SOURCE_DIR}/src/commontypes.hpp"
    "${CMAKE_SOURCE_DIR}/src/menuitem.hpp"
    "${CMAKE_SOURCE_DIR}/src/menubuilder.hpp"
    #"${CMAKE_SOURCE_DIR}/src/menuprocessor.hpp"
    "${CMAKE_SOURCE_DIR}/src/configparser.hpp"
    "${CMAKE_SOURCE_DIR}/src/displayoled.hpp"
)

set(SOURCES
    ${HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/buttonListener.cpp"
    "${CMAKE_SOURCE_DIR}/src/menuitem.cpp"
    "${CMAKE_SOURCE_DIR}/src/menubuilder.cpp"
    #"${CMAKE_SOURCE_DIR}/src/menuprocessor.cpp"
    "${CMAKE_SOURCE_DIR}/src/configparser.cpp"
    "${CMAKE_SOURCE_DIR}/src/displayoled.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} bcm2835 rt pthread ArduiPi_OLED)

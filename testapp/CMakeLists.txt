set(HEADERS Gpioclass.hpp)

set(SOURCES ${HEADERS} Blinker.cpp Gpioclass.cpp)

add_executable(Blinker ${SOURCES})
target_link_libraries(Blinker pthread)

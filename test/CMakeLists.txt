set(SOURCES main.cpp)

add_executable(TestLog ${SOURCES})
target_link_libraries(TestLog logger pthread)

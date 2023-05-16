set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.18)
project(strategie)

set(CMAKE_CXX_FLAGS -pthread)
include_directories(includes)

find_package(Threads REQUIRED)
find_package(PkgConfig)
pkg_check_modules(CAN REQUIRED CAN)
pkg_check_modules(XBEE REQUIRED XBee)

add_executable(${PROJECT_NAME} src/main.cpp
        includes/graph.h src/graph.cpp
        includes/parser.h src/parser.cpp
        includes/instructions.h src/instructions.cpp
        includes/lcd.h src/lcd.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CAN_INCLUDE_DIRS} ${XBEE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CAN_LIBRARIES} ${XBEE_LIBRARIES} Threads::Threads)
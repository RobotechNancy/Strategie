set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.24)
project(Graph)
set(CMAKE_CXX_FLAGS -pthread)

find_package(PkgConfig)
pkg_check_modules(LOGS REQUIRED Logs)
pkg_check_modules(CAN REQUIRED CAN)
pkg_check_modules(XBEE REQUIRED Xbee)

include_directories(includes)
include_directories(${PROJECT_NAME} ${LOGS_INCLUDE_DIRS} ${CAN_INCLUDE_DIRS} ${XBEE_INCLUDE_DIRS})

add_executable(Graph src/main.cpp includes/graph.h src/graph.cpp src/nodes.cpp includes/nodes.h)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LOGS_LIBRARIES} ${CAN_LIBRARIES} ${XBEE_LIBRARIES})
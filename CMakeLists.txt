cmake_minimum_required(VERSION 3.13)
project(trySendData)

set(CMAKE_CXX_STANDARD 14)

include_directories(/home/wenyu/Documents/labstreaminglayer/build/install/include/)

file(GLOB LIBRARIES "/home/wenyu/Documents/labstreaminglayer/build/install/lib/*")

add_executable(trySendData main.cpp)

target_link_libraries(trySendData ${LIBRARIES})
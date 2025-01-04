cmake_minimum_required(VERSION 3.30)

project(rest-server)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

find_package(Boost CONFIG COMPONENTS system json)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(rest-server main.cpp)

target_link_libraries(rest-server Threads::Threads ${Boost_LIBRARIES})

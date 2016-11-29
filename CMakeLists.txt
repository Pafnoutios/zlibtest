cmake_minimum_required(VERSION 2.4.4)

project(zlibtests)

include_directories(zlib)

add_subdirectory(zlib)

add_executable(zlibtests main.cpp)

target_link_libraries(zlibtests zlibstatic)

file(COPY source-kjv.txt
  DESTINATION .)
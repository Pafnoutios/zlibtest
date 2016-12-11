cmake_minimum_required(VERSION 2.4.4)

project(zlibtests)

include_directories(zlib)

add_subdirectory(zlib)

add_executable(zlibtests
  main.cpp
  comdec.h
  comdec.cpp
  comdec_zlib.h
  comdec_zlib.cpp)

target_link_libraries(zlibtests zlibstatic)

file(COPY source-kjv.txt source-nile.jpg source-lena.tiff
  DESTINATION .)
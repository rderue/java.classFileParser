cmake_minimum_required(VERSION 3.10)
project(ExtractClass)

set(CMAKE_CXX_STANDARD 17)

add_executable(ExtractClass main.cpp extract.h classFile.cpp classFile.h cPoolTagTypes.h)
target_link_libraries(ExtractClass stdc++fs)

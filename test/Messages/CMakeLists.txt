cmake_minimum_required(VERSION 3.5.0)
project(Messages VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Messages main.cpp)
target_link_libraries(Messages curlpp curl)


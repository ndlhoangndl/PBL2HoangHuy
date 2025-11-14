cmake_minimum_required(VERSION 3.16)
project(PBL2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PBL2
        main.cpp
        System.cpp
        Job.cpp
)


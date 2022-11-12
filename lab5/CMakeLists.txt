cmake_minimum_required(VERSION 3.23)
project(lab5)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(lab5
        main.cpp
        TGeneticAlgorithm.cpp
        TGeneticAlgorithm.h
        TTimer.cpp
        TTimer.h
        TTester.cpp
        TTester.h
        include/pbPlots.cpp
        include/pbPlots.hpp
        include/supportLib.cpp
        include/supportLib.hpp)

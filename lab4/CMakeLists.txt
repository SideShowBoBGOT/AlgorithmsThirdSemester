cmake_minimum_required(VERSION 3.23)
project(lab4)

set(CMAKE_CXX_STANDARD 20)

include_directories(AntsAlgorithm/AntsAlgorithm)
include_directories(AntsAlgorithm/include)

add_executable(lab4
        AntsAlgorithm/AntsAlgorithm/AntsAlgorithm.cpp
        AntsAlgorithm/AntsAlgorithm/NNHelpFunctions.cpp
        AntsAlgorithm/AntsAlgorithm/NNHelpFunctions.h
        AntsAlgorithm/AntsAlgorithm/TAntAlgorithm.cpp
        AntsAlgorithm/AntsAlgorithm/TAntAlgorithm.h
        AntsAlgorithm/AntsAlgorithm/TAntAlgorithmTester.cpp
        AntsAlgorithm/AntsAlgorithm/TAntAlgorithmTester.h
        AntsAlgorithm/include/pbPlots.cpp
        AntsAlgorithm/include/pbPlots.hpp
        AntsAlgorithm/include/supportLib.cpp
        AntsAlgorithm/include/supportLib.hpp)

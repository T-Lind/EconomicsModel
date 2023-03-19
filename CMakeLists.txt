cmake_minimum_required(VERSION 3.24)
project(EconomicsModel)

set(CMAKE_CXX_STANDARD 17)

add_executable(EconomicsModel Rand.h Bonds.h main.cpp Investments.h)
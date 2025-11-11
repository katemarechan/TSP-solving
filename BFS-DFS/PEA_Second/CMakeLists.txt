cmake_minimum_required(VERSION 3.26)
project(PEA_Better)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEA_Better main.cpp
        bfs.cpp
        bfs.h
        dfs.h
        dfs.cpp
        lc.cpp
        lc.h
        file_reader.cpp
        file_reader.h)

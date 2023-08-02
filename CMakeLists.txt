cmake_minimum_required(VERSION 3.10)
project(ChessGame VERSION 1.0)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

add_subdirectory(libs/raylib)

include_directories(libs/raylib/include)

add_executable(ChessGame src/main.cpp src/Pieces/Pieces.cpp src/Board/Board.cpp src/Board/Setup/Setup.cpp
        src/GameLoop/GameLoop.cpp src/Render/Renderer.cpp src/Board/MoveGenerator/MoveGenerator.cpp
        src/Board/Move/Move.cpp)
target_link_libraries(ChessGame raylib)

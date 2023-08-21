cmake_minimum_required(VERSION 3.12)
project(ChessC)

enable_language(C ASM)

SET(ASM_OPTIONS "-x assembler-with-cpp -fPIE")
SET(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" )

# Add the raylib library
add_subdirectory(libs/raylib)  # Make sure to update this path to the actual raylib source directory

# Set the source files for your project
set(SOURCES
        src/Board/Setup/Setup.cpp
        src/main.cpp
        src/Board/Board.cpp
        src/Board/Move/Move.cpp
        src/Board/MoveGenerator/MoveGenerator.cpp
        src/Render/Renderer.cpp
        src/Board/Pieces/Pieces.cpp
        src/Board/GameLoop/GameLoop.cpp
        src/Board/Types/Types.h
)

# Include directories for your project
include_directories(
        libs/raylib/src
)

# Add the executable
add_executable(ChessC ${SOURCES})

# Link against raylib and other libraries
target_link_libraries(ChessC raylib)

# Set properties for the executable
set_target_properties(ChessC
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)




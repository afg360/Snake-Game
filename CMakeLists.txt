cmake_minimum_required(VERSION 3.28)
set(CMAKE_C_COMPILER gcc)
project(Snake_Game)

# Set source files
set(SOURCES
    src/main.c
    src/Snake.c
    src/Food.c
    src/utils.c
    src/rendering.c
)

# Set executable name and sources
add_executable(Snake_Game ${SOURCES})

# Include directories if needed
target_include_directories(Snake_Game PRIVATE include)

# Specify additional library directories
link_directories(Snake_Game PRIVATE lib, bin)

# Link against dynamic libraries
target_link_libraries(Snake_Game PRIVATE SDL2main, SDL2, mingw32, SDL2_ttf)

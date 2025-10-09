cmake_minimum_required(VERSION 3.30)
project(VoxelGame LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        "src/*.h"
        "Libraries/src/glad.c"
)

add_executable(VoxelGame ${SOURCES})

include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e
)

FetchContent_MakeAvailable(glm)

target_link_libraries(VoxelGame PRIVATE glm::glm)

target_include_directories(VoxelGame PRIVATE
        ${CMAKE_SOURCE_DIR}/Libraries/include
)

target_link_libraries(VoxelGame PRIVATE
        "${CMAKE_SOURCE_DIR}/Libraries/lib/glfw3.lib"
        opengl32
)

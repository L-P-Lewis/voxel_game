cmake_minimum_required(VERSION 3.6...3.14)
project(VoxelGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "Libraries/src/glad.c")

add_executable(VoxelGame ${SOURCES})

add_custom_command(
		TARGET VoxelGame
		PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
										$<TARGET_FILE_DIR:VoxelGame>/assets)

include(FetchContent)

# FetchContent_Declare( glm GIT_REPOSITORY https://github.com/g-truc/glm.git
# GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863)

# FetchContent_MakeAvailable(glm)

# target_include_directories(VoxelGame PRIVATE
# ${CMAKE_SOURCE_DIR}/lib/glad/include)
target_link_libraries(VoxelGame PRIVATE glad glfw glm::glm)

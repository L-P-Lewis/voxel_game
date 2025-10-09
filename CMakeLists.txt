cmake_minimum_required(VERSION 3.6...3.14)
project(VoxelGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

# Include library subdirectory and build third party libs
add_subdirectory(lib)

# Include src directory to build voxel game lib
add_subdirectory(src)

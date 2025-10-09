add_executable(voxel_core "./main.cpp" "./voxel_game.cpp" "./renderer.cpp"
																										"./window.cpp")
target_include_directories(voxel_core
																											PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(voxel_core PRIVATE glad glfw glm::glm)

# Create core executable
add_executable(voxel_core
 "./main.cpp"
 "./voxel_game.cpp"
 "./window.cpp"
 "./app_layer_manager.cpp"
 "./layer/game_layer.cpp"
 "./camera.cpp"
 "./assets/shader.cpp"
 "./assets/assets.cpp"
 "./assets/texture.cpp"
 "./block.cpp"
 "./chunk.cpp"
 "./game_world.cpp"
)

# Copy assets to output directory on build
add_custom_command(
        TARGET voxel_core POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets
                ${CMAKE_CURRENT_BINARY_DIR}/assets/)

# Setup include dir
target_include_directories(voxel_core
 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)




# Link libs
target_link_libraries(voxel_core PRIVATE glad glfw glm::glm)

# Create core executable
add_executable(voxel_core 
	"./main.cpp" 
	"./voxel_game.cpp" 
	"./window.cpp"
)

# Copy assets to output directory on build
add_custom_command(TARGET voxel_core PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/assets
		$<TARGET_FILE_DIR:voxel_core>/assets)

# Setup include dir
target_include_directories(voxel_core
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

# Link libs
target_link_libraries(voxel_core PRIVATE glad glfw glm::glm)

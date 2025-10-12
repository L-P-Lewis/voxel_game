#pragma once

#include <vector>

// Dummy Declarations
class Window;
class GLFWwindow;
class AppLayer;

/// Superclass representing the whole game
class VoxelGame {
	// Singleton variable
    static VoxelGame *instance;
    static constexpr float TICKS_PER_SECOND = 20.0;
    static constexpr float SECONDS_PER_TICK = 1.0 / TICKS_PER_SECOND;

	// App layer stack
	std::vector<AppLayer*> app_layers;

	// Window handle
    Window *window; // We don't need a smart pointer here, all this just gets cleaned up by the OS when we quit the program

    double accumulator = 0.0;
    double currentTime = 0.0;
    long tickCount = 0;

public:
    VoxelGame();
    ~VoxelGame();
    void run();
    void processInputs();
    void tick();
    void render(float deltaTime);
    void onResize(int width, int height) const;

    Window& getWindow() const;

    static VoxelGame* getInstance();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

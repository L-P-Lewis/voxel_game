#pragma once
#include <memory>

// Dummy Declarations
class Window;
class GLFWwindow;
class AppLayer;
class AppLayerManager;

/// Superclass representing the whole game
class VoxelGame {
    static constexpr float TICKS_PER_SECOND = 30.0;
    static constexpr float SECONDS_PER_TICK = 1.0 / TICKS_PER_SECOND;

    static std::unique_ptr<Window> window;
	static AppLayerManager *appLayerManager;

    static double accumulator;
    static double currentTime;
    static long tickCount;

public:
	static void init();
	static void destroy();
    static void run();
    static void processInputs();
    static void tick();
    static void render(float deltaTime);
    static void onResize(int width, int height);

	/// Queues up an app layer to be pushed onto the layer stack at the end of the frame
	/// *layer: The layer to push
	static void pushAppLayer(AppLayer *layer);
	/// Queues up a pop opperation on the app layer stack at the end of the frame
	static void popAppLayer();

    static Window& getWindow();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

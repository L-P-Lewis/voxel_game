#pragma once
#include <memory>

// Dummy Declarations
class Window;
class GLFWwindow;
class AppLayer;
class AppLayerManager;

/// Superclass representing the whole game
class VoxelGame {
	// Singleton variable
    static VoxelGame *instance;
    static constexpr float TICKS_PER_SECOND = 20.0;
    static constexpr float SECONDS_PER_TICK = 1.0 / TICKS_PER_SECOND;

	// Window handle
    std::unique_ptr<Window> window;
	AppLayerManager *appLayerManager;

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
	
	/// Queues up an app layer to be pushed onto the layer stack at the end of the frame
	/// *layer: The layer to push
	void pushAppLayer(AppLayer *layer) const;
	/// Queues up a pop opperation on the app layer stack at the end of the frame
	void popAppLayer() const;

    Window& getWindow() const;

    static VoxelGame* getInstance();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

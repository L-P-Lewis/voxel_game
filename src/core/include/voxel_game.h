#pragma once

#include <vector>

#define MAX_APP_LAYER_OPS 16

// Dummy Declarations
class Window;
class GLFWwindow;
class AppLayer;

/// Superclass representing the whole game
class VoxelGame {
	// Layer Opperation Struct
	struct LayerOpp {
		enum {
			LAYER_PUSH,
			LAYER_POP
		} type;
		union {
			// Tagged union with just a single pointer in it, how scandalously inefficient, well tough
			struct { AppLayer *layer; };
		};
	};

	// Singleton variable
    static VoxelGame *instance;
    static constexpr float TICKS_PER_SECOND = 20.0;
    static constexpr float SECONDS_PER_TICK = 1.0 / TICKS_PER_SECOND;


	// Simple staticly sized array for layer operations, even this is probably overkill
	LayerOpp layer_operation_queue[MAX_APP_LAYER_OPS];
	int layer_operation_count = 0;

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
	
	/// Queues up an app layer to be pushed onto the layer stack at the end of the frame
	/// *layer: The layer to push
	void pushAppLayer(AppLayer *layer);
	/// Queues up a pop opperation on the app layer stack at the end of the frame
	void popAppLayer();

    Window& getWindow() const;

    static VoxelGame* getInstance();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

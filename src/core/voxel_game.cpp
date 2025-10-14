#include "voxel_game.h"
#include "layer/test_layer.h"
#include "window.h"
#include "app_layer.h"
#include "app_layer_manager.h"

#include <GLFW/glfw3.h>

std::unique_ptr<Window> VoxelGame::window = nullptr;
AppLayerManager* VoxelGame::appLayerManager = nullptr;
double VoxelGame::accumulator = 0.0;
double VoxelGame::currentTime = 0.0;
long VoxelGame::tickCount = 0;

void VoxelGame::init() {
    window = std::make_unique<Window>();
    window->init();
    glfwSetFramebufferSizeCallback(window->getHandle(), framebuffer_size_callback);
    appLayerManager = new AppLayerManager();
	pushAppLayer(new TestLayer());
}

void VoxelGame::destroy() {
    delete appLayerManager;
}

void VoxelGame::pushAppLayer(AppLayer *layer) {
	appLayerManager->pushAppLayer(layer);
}

void VoxelGame::popAppLayer() {
	appLayerManager->popAppLayer();
}


void VoxelGame::run() {
    while (!glfwWindowShouldClose(getWindow().getHandle())) {

        appLayerManager->update();
		if (appLayerManager->isEmpty()) {
			glfwSetWindowShouldClose(getWindow().getHandle(), true);
		}

        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;

        if (frameTime > 0.25) {
            frameTime = 0.25;
        }

        currentTime = newTime;
        accumulator += frameTime;

        processInputs();

        while (accumulator >= SECONDS_PER_TICK) {
        	appLayerManager->tick();
            accumulator -= SECONDS_PER_TICK;
        }

        double deltaTime = accumulator / SECONDS_PER_TICK;

        glfwPollEvents();

    	appLayerManager->render(deltaTime);

        glfwSwapBuffers(getWindow().getHandle());
    }
}

void VoxelGame::processInputs() {
    if (glfwGetKey(getWindow().getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(getWindow().getHandle(), true);
    }
}


void VoxelGame::onResize(int width, int height) {
    getWindow().resize(width, height);
    appLayerManager->resize(width, height);
}

Window &VoxelGame::getWindow() {
    return *window;
}

void VoxelGame::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    onResize(width, height);
}


#include "voxel_game.h"
#include "layer/test_layer.h"
#include "window.h"
#include "app_layer.h"
#include "app_layer_manager.h"

#include <GLFW/glfw3.h>


VoxelGame* VoxelGame::instance = nullptr;

VoxelGame::VoxelGame() {
    instance = this;
    this->window = new Window();
    window->init();
    glfwSetFramebufferSizeCallback(window->getHandle(), framebuffer_size_callback);
    this->appLayerManager = new AppLayerManager();
	this->pushAppLayer(new TestLayer());
}

VoxelGame::~VoxelGame() {
    instance = nullptr;
}

void VoxelGame::pushAppLayer(AppLayer *layer) const {
	this->appLayerManager->pushAppLayer(layer);
}

void VoxelGame::popAppLayer() const {
	this->appLayerManager->popAppLayer();
}


void VoxelGame::run() {
    while (!glfwWindowShouldClose(getWindow().getHandle())) {

        this->appLayerManager->update();
		if (this->appLayerManager->isEmpty()) {
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
        	this->appLayerManager->tick(this);
            accumulator -= SECONDS_PER_TICK;
        }

        double deltaTime = accumulator / SECONDS_PER_TICK;

        glfwPollEvents();

    	this->appLayerManager->render(this, deltaTime);

        glfwSwapBuffers(getWindow().getHandle());
    }
}

void VoxelGame::processInputs() {
    if (glfwGetKey(getWindow().getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(getWindow().getHandle(), true);
    }
}


void VoxelGame::onResize(int width, int height) const {
    getWindow().resize(width, height);
    this->appLayerManager->resize(width, height);
}

Window &VoxelGame::getWindow() const {
    return *window;
}

VoxelGame *VoxelGame::getInstance() {
    return instance;
}

void VoxelGame::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    getInstance()->onResize(width, height);
}


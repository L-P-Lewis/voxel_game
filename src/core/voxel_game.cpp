#include "voxel_game.h"
#include "layer/test_layer.h"
#include "window.h"
#include "app_layer.h"

#include <GLFW/glfw3.h>


VoxelGame* VoxelGame::instance = nullptr;

VoxelGame::VoxelGame() {
    instance = this;
    this->window = new Window();
    window->init();
    glfwSetFramebufferSizeCallback(window->getHandle(), framebuffer_size_callback);
	this->app_layers.push_back(new TestLayer());
}

VoxelGame::~VoxelGame() {
    instance = nullptr;
}

void VoxelGame::run() {
    while (!glfwWindowShouldClose(getWindow().getHandle()) & !this->app_layers.empty()) {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;

        if (frameTime > 0.25) {
            frameTime = 0.25;
        }

        currentTime = newTime;
        accumulator += frameTime;

        processInputs();

        while (accumulator >= SECONDS_PER_TICK) {
			for (int i = app_layers.size() - 1; i > 0; i--) {
				if (!app_layers[i]->tick(this)) {
					break;
				}
			}
            accumulator -= SECONDS_PER_TICK;
        }

        double deltaTime = accumulator / SECONDS_PER_TICK;

        glfwPollEvents();
		for (int i = 0; i < app_layers.size(); i++) {
			app_layers[i]->render(this);
		}
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


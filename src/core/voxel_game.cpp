#include "voxel_game.h"
#include "layer/test_layer.h"
#include "window.h"
#include "app_layer.h"

#include <GLFW/glfw3.h>
#include <cstdlib>


VoxelGame* VoxelGame::instance = nullptr;

VoxelGame::VoxelGame() {
    instance = this;
    this->window = new Window();
    window->init();
    glfwSetFramebufferSizeCallback(window->getHandle(), framebuffer_size_callback);
	this->pushAppLayer(new TestLayer());
}

VoxelGame::~VoxelGame() {
    instance = nullptr;
}

void VoxelGame::pushAppLayer(AppLayer *layer)
{
	if (this->layer_operation_count == MAX_APP_LAYER_OPS) {return;}
	this->layer_operation_queue[this->layer_operation_count++] = {
		.type = LayerOpp::LAYER_PUSH,
		.layer = layer
	};
}

void VoxelGame::popAppLayer()
{
	if (this->layer_operation_count == MAX_APP_LAYER_OPS) {return;}
	this->layer_operation_queue[this->layer_operation_count++] = {
		.type = LayerOpp::LAYER_POP
	};
}


void VoxelGame::run() {
    while (!glfwWindowShouldClose(getWindow().getHandle())) {

		for (int i = 0; i < layer_operation_count; i++) {
			LayerOpp new_op = layer_operation_queue[i];
			if (new_op.type == LayerOpp::LAYER_POP) {
				if (app_layers.empty()) continue;
				AppLayer *old = app_layers.back();
				app_layers.pop_back();
				delete old;
			} else if (new_op.type == LayerOpp::LAYER_PUSH) {
				app_layers.push_back(new_op.layer);
			}
		}
		layer_operation_count = 0;
		if (app_layers.empty()) {
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


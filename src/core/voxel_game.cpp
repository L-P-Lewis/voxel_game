#include "voxel_game.h"
#include "renderer.h"
#include "window.h"

#include <GLFW/glfw3.h>


VoxelGame* VoxelGame::instance = nullptr;

VoxelGame::VoxelGame() {
    instance = this;
    this->window = std::make_unique<Window>();
    window->init();
    glfwSetFramebufferSizeCallback(window->getHandle(), framebuffer_size_callback);
    this->gameRenderer = std::make_unique<GameRenderer>(*this);
}

VoxelGame::~VoxelGame() {
    instance = nullptr;
}

void VoxelGame::run() {
    while (!glfwWindowShouldClose(getWindow().getHandle())) {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;

        if (frameTime > 0.25) {
            frameTime = 0.25;
        }

        currentTime = newTime;
        accumulator += frameTime;

        processInputs();

        while (accumulator >= SECONDS_PER_TICK) {
            tick();
            accumulator -= SECONDS_PER_TICK;
        }

        double deltaTime = accumulator / SECONDS_PER_TICK;

        glfwPollEvents();
        render(deltaTime);
        glfwSwapBuffers(getWindow().getHandle());
    }
}

void VoxelGame::processInputs() {
    if (glfwGetKey(getWindow().getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(getWindow().getHandle(), true);
    }
}

void VoxelGame::tick() {
    tickCount++;
}

void VoxelGame::render(float deltaTime) {
    gameRenderer->renderGame(deltaTime);
}

void VoxelGame::onResize(int width, int height) const {
    getWindow().resize(width, height);
}

Window &VoxelGame::getWindow() const {
    return *window;
}

GameRenderer &VoxelGame::getGameRenderer() const {
    return *gameRenderer;
}

VoxelGame *VoxelGame::getInstance() {
    return instance;
}

void VoxelGame::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    getInstance()->onResize(width, height);
}


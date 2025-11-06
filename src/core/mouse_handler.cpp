#include "include/mouse_handler.h"

#include "camera.h"
#include "voxel_game.h"
#include "window.h"
#include "GLFW/glfw3.h"

bool MouseHandler::leftButtonPressed = false;
bool MouseHandler::rightButtonPressed = false;
bool MouseHandler::middleButtonPressed = false;
double MouseHandler::xPos = 0.0;
double MouseHandler::yPos = 0.0;
double MouseHandler::accumulatedDX = 0.0;
double MouseHandler::accumulatedDY = 0.0;

void MouseHandler::setup(const Window &window) {
    glfwSetCursorPosCallback(window.getHandle(), onMouseMove);
    glfwSetMouseButtonCallback(window.getHandle(), onMouseButton);
}

void MouseHandler::onMouseMove(GLFWwindow *window, double xpos, double ypos) {
    if (window == VoxelGame::getWindow().getHandle()) {
        accumulatedDX += xpos - xPos;
        accumulatedDY += ypos - yPos;

        xPos = xpos;
        yPos = ypos;
    }
}

void MouseHandler::onMouseButton(GLFWwindow *window, int button, int action, int mods) {
    //TODO: Implement mouse button handling here :3
}

float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void MouseHandler::turnCamera(Camera &camera) {
    double mouseSensitivity = 0.3;
    double d2 = mouseSensitivity * 0.6 + 0.2;
    double d3 = d2*d2*d2;
    double d4 = d3 * 8.0;

    double x = accumulatedDX * d4;
    double y = accumulatedDY * d4;

    float newPitch =  camera.getPitch() - static_cast<float>(y);
    float newYaw = camera.getYaw() + static_cast<float>(x);
    camera.setPitch(newPitch);
    camera.setYaw(newYaw);
}

void MouseHandler::enableCursor(const bool enable) {
    glfwSetInputMode(VoxelGame::getWindow().getHandle(), GLFW_CURSOR, enable ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

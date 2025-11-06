#pragma once
#include "camera.h"

class Window;
class GLFWwindow;

class MouseHandler {
    static bool leftButtonPressed;
    static bool rightButtonPressed;
    static bool middleButtonPressed;
    static double xPos;
    static double yPos;
    static double accumulatedDX;
    static double accumulatedDY;

public:
    static void setup(const Window &window);
    static void onMouseMove(GLFWwindow *window, double xpos, double ypos);
    static void onMouseButton(GLFWwindow *window, int button, int action, int mods);

    static void turnCamera(Camera &camera);

    static void enableCursor(bool enable);
};

#include <iostream>
#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

Window::Window(int width, int height, bool resizeable, const char *title) {
    this->width = width;
    this->height = height;
    this->resizeable = resizeable;
    this->title = title;
}

void Window::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizeable);
    this->handle = glfwCreateWindow(getWidth(), getHeight(), title, nullptr, nullptr);

    if (getHandle() == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(getHandle());
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, getWidth(), getHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Window::~Window() {
    glfwDestroyWindow(getHandle());
    glfwTerminate();
}

void Window::resize(int width, int height) {
    glViewport(0, 0, width, height);
}

int Window::getWidth() const {
    return this->width;
}

int Window::getHeight() const {
    return this->height;
}

const char* Window::getTitle() const {
    return this->title;
}

GLFWwindow *Window::getHandle() const {
    return this->handle;
}

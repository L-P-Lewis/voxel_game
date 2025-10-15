#pragma once

/// Dummy GLFWindow handle definition
class GLFWwindow;

/// Game Window Class
class Window {
    int width;
    int height;
    bool resizeable;
    const char* title;
    GLFWwindow* handle{};
public:
    explicit Window(int width = 1600, int height = 900, bool resizeable = true, const char* title = "OpenGL Window");
    ~Window();
    void init();
    void resize(int width, int height);
    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;
    [[nodiscard]] const char* getTitle() const;
    [[nodiscard]] GLFWwindow* getHandle() const;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

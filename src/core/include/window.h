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
	float accumulated_mouse_x;
	float accumulated_mouse_y;
	float mpos_x;
	float mpos_y;
public:
    explicit Window(int width = 1600, int height = 900, bool resizeable = true, const char* title = "OpenGL Window");
    ~Window();
    void init();
    void resize(int width, int height);
    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;
    [[nodiscard]] const char* getTitle() const;
    [[nodiscard]] GLFWwindow* getHandle() const;

	float get_mouse_acc_x() const {return accumulated_mouse_x;};
	float get_mouse_acc_y() const {return accumulated_mouse_y;};
	void clear_mouse_acc() {accumulated_mouse_x = 0; accumulated_mouse_y = 0;};

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

	friend void mouse_callback(GLFWwindow *window, double xpos, double ypos);
};

#pragma once

#include <memory>

class Window;
class GLFWwindow;
class GameRenderer;

class VoxelGame {
    static VoxelGame *instance;
    static constexpr float TICKS_PER_SECOND = 20.0;
    static constexpr float SECONDS_PER_TICK = 1.0 / TICKS_PER_SECOND;

    std::unique_ptr<Window> window;
    std::unique_ptr<GameRenderer> gameRenderer;

    double accumulator = 0.0;
    double currentTime = 0.0;
    long tickCount = 0;

public:
    VoxelGame();
    ~VoxelGame();
    void run();
    void processInputs();
    void tick();
    void render(float deltaTime);
    void onResize(int width, int height) const;

    Window& getWindow() const;
    GameRenderer& getGameRenderer() const;

    static VoxelGame* getInstance();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

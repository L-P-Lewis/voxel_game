#pragma once

/// Dummy game class definition
class VoxelGame;

class GameRenderer {
    VoxelGame &game;
public:
    explicit GameRenderer(VoxelGame &game);
    ~GameRenderer();
    void renderGame(float deltaTime);
    void resize(int width, int height) const;
};

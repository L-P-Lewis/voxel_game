#include "GameRenderer.h"

#include <GLFW/glfw3.h>

GameRenderer::GameRenderer(VoxelGame &game) : game(game) {
}

GameRenderer::~GameRenderer() = default;

void GameRenderer::renderGame(float deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0,   1);
    glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(1,  -1);
    glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-1, -1);
    glEnd();
}

void GameRenderer::resize(const int width, const int height) const {
}




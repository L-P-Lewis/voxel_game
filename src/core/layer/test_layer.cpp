#include "layer/test_layer.h"
#include "app_layer.h"
#include "voxel_game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void TestLayer::render(VoxelGame *game) {
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0,   1);
    glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(1,  -1);
    glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-1, -1);
    glEnd();
}

bool TestLayer::tick(VoxelGame *game) {
	return true;
}

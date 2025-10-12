#include "layer/test_layer.h"
#include "app_layer.h"
#include "voxel_game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

float tick_count = 0.0;


void TestLayer::render(VoxelGame *game) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glPushMatrix();
	
	glTranslatef(cos(tick_count), 0.0, 0.0);
	glRotatef(10.0 * tick_count, 0.0f, 1.0f, 0);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0,   1, 0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1,  -1, 0);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1, -1, 0);
    glEnd();

	glPopMatrix();

}

bool TestLayer::tick(VoxelGame *game) {
	tick_count += 0.1;
	std::cout << tick_count << std::endl;
	return true;
}

#include "layer/test_layer.h"
#include "app_layer.h"
#include "voxel_game.h"
//#include <GL/glext.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

float tick_count = 0.0;

const float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

const char* vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragment_shader_source = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}";

unsigned int VBO;
unsigned int shader_program;

TestLayer::TestLayer()
{
	// We are initializing only one buffer, use pointer stuff to cast vbo into an array
	glGenBuffers(1, &VBO);
	// Make it an array buffer

	unsigned int vertex_shader;
	unsigned int fragment_shader;

	// Compile Vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	// Compile Fragment shader
	fragment_shader= glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	// Create the shader program and clean up now useless shaders
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);
}


void TestLayer::render(float deltaTime) {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glLoadIdentity();
	 
	//glPushMatrix();

	
	//glTranslatef(cos(tick_count), 0.0, 0.0);
	//glRotatef(10.0 * tick_count, 0.0f, 1.0f, 0);
	
	//glad_glPushMatrix
	

	//glBegin(GL_TRIANGLES);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(0,   1, 0);
	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(1,  -1, 0);
	//glColor3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(-1, -1, 0);
    //glEnd();

	//glPopMatrix();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  

	glUseProgram(shader_program);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

bool TestLayer::tick() {
	tick_count += 0.1;
	this->camera.setPosition(glm::dvec3(0.0, 0.0, 10.0));
	this->camera.setYaw(tick_count);
	return true;
}

void TestLayer::resize(int width, int height) {

}

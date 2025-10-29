#include "layer/test_layer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "voxel_game.h"
#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

float tick_count = 0.0;

glm::vec3 camera_pos;
float pitch;
float yaw;

const int vert_count = 36;
const float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f
};

const char* vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aCol;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"out vec3 vColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
	"	vColor = aCol;\n"
    "}\0";

const char* fragment_shader_source = "#version 330 core\n"
	"in vec3 vColor;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(vColor, 1.0f);\n"
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


	this->camera.setFov(90);
	this->camera.setNearPlane(0.1f);
	this->camera.setFarPlane(1000.0f);
	this->camera.setPosition(glm::dvec3(0.0, 0.0, -3.0));
	this->camera.setYaw(90);
	this->camera.setRoll(0);
	this->camera.setPitch(0);
	this->camera.setPerspective();
	this->camera.setAspectRatio(1.0);
	this->camera.recalculateProjectionMatrix();
	this->camera.recalculateViewMatrix();
}


void TestLayer::render(float deltaTime) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glm::mat4 model_matrix( 1.0 );
	model_matrix = glm::rotate(model_matrix, tick_count, glm::vec3(0.0, 1.0, 0.0));
	int model_loc = glGetUniformLocation(shader_program, "model");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	int proj_loc = glGetUniformLocation(shader_program, "projection");
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(this->camera.getProjectionMatrix()));



	glm::mat4 view(1.0f);
	//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
  	//	   glm::vec3(0.0f, 0.0f, 0.0f), 
  	//	   glm::vec3(0.0f, 1.0f, 0.0f));
	//
	view = this->camera.getViewMatrix();
	int view_loc = glGetUniformLocation(shader_program, "view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
	//std::cout << view[0][0] << view[0][1] << view[0][2] << view[0][3] << std::endl;
	//std::cout << view[1][0] << view[1][1] << view[1][2] << view[1][3] << std::endl;
	//std::cout << view[2][0] << view[2][1] << view[2][2] << view[2][3] << std::endl;
	//std::cout << view[3][0] << view[3][1] << view[3][2] << view[3][3] << std::endl;
	//std::cout << std::endl;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);  

	glUseProgram(shader_program);
	glDrawArrays(GL_TRIANGLES, 0, vert_count);

}

bool TestLayer::tick() {
	tick_count += 0.1;

	const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_W) == GLFW_PRESS)
        camera_pos += cameraSpeed * camera.getLookVector();
    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_S) == GLFW_PRESS)
        camera_pos -= cameraSpeed * camera.getLookVector();

    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_A) == GLFW_PRESS)
        camera_pos -= glm::normalize(glm::cross(camera.getLookVector(), camera.getUpVector())) * cameraSpeed;
    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_D) == GLFW_PRESS)
        camera_pos += glm::normalize(glm::cross(camera.getLookVector(), camera.getUpVector())) * cameraSpeed;


    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_SPACE) == GLFW_PRESS)
        camera_pos += cameraSpeed * glm::vec3(0.0, 1.0, 0.0);
    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera_pos += cameraSpeed * glm::vec3(0.0, -1.0, 0.0);


    //if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_RIGHT) == GLFW_PRESS)
	//	yaw += 1.0;
   // if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_LEFT) == GLFW_PRESS)
	//	yaw -= 1.0;

  //  if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_UP) == GLFW_PRESS)
//		pitch += 1.0;
//    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_DOWN) == GLFW_PRESS)
		//pitch -= 1.0;

	float mouse_sensitivity = 0.01f;

	pitch -= VoxelGame::getWindow().get_mouse_acc_y();
	yaw += VoxelGame::getWindow().get_mouse_acc_x();
	VoxelGame::getWindow().clear_mouse_acc();

	if (pitch > 89.0) pitch = 89.0;
	if (pitch < -89.0) pitch = -89.0;

	camera.setPosition(camera_pos);
	camera.setYaw(yaw);
	camera.setPitch(pitch);
	return true;
}

void TestLayer::resize(int width, int height) {
	camera.setAspectRatio((float)width / (float)height);

}

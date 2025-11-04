#include "layer/game_layer.h"
#include "chunk.h"

#include "assets/shader.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "voxel_game.h"
#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

Chunk chunk;

glm::vec3 camera_pos = glm::vec3(-3, 0, 0);
float pitch;
float yaw;
std::string vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 normal;\n"
    "layout (location = 2) in vec2 uv;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"out vec3 vColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(position, 1.0);\n"
	"	vColor = vec3(uv, 0);\n"
    "}\0";

std::string fragment_shader_source = "#version 330 core\n"
	"in vec3 vColor;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(vColor, 1.0f);\n"
	"}";

Shader *shader;

GameLayer::GameLayer()
{
	this->registry.RegisterBlock("air", Block::AllSides(0));
	this->registry.RegisterBlock("stone", Block::AllSides(1));
	chunk.Fill(1);
	chunk.RegnerateMesh(&this->registry);

	shader = new Shader(vertex_shader_source, fragment_shader_source);

	this->camera.setFov(45);
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

void GameLayer::render(float deltaTime) 
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model_matrix( 1.0 );
	glm::mat4 projection(1.0f);
	projection = this->camera.getProjectionMatrix();
	glm::mat4 view(1.0f);
	view = this->camera.getViewMatrix();


	shader->set_uniform("projection", projection);
	shader->set_uniform("view", view);


	shader->set_uniform("model", chunk.GetChunkTransform());
	chunk.Draw(*shader);
}

bool GameLayer::tick() 
{
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

	float mouse_sensitivity = 0.0001f;

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

void GameLayer::resize(int width, int height) 
{
	camera.setAspectRatio((float)width / (float)height);
}

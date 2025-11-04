#include "layer/game_layer.h"
#include "assets/texture.h"
#include "camera.h"
#include "chunk.h"

#include "assets/shader.h"
#include "game_world.h"
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



GameLayer GameLayer::New()
{
	BlockRegistry registry;
	Camera camera;
	Texture terrain;

	registry.RegisterBlock("air", Block::AllSides(0));
	registry.RegisterBlock("stone", Block::AllSides(1));


	Shader shader = VoxelGame::getAssetManager().GetShader("chunk_shader");
	terrain = VoxelGame::getAssetManager().GetTexture("terrain");

	camera.setFov(45);
	camera.setNearPlane(0.1f);
	camera.setFarPlane(1000.0f);
	camera.setPosition(glm::dvec3(0.0, 0.0, -3.0));
	camera.setYaw(90);
	camera.setRoll(0);
	camera.setPitch(0);
	camera.setPerspective();
	camera.setAspectRatio(1.0);
	camera.recalculateProjectionMatrix();
	camera.recalculateViewMatrix();

	GameWorld world = GameWorld::New();

	return GameLayer(M{
		.camera = camera,
		.registry = registry,
		.terrain = terrain,
		.chunk_shader = shader,
		.world = world
	});
}

void GameLayer::render(float deltaTime) 
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	m.world.Draw(m.camera, m.chunk_shader, m.terrain);
}

bool GameLayer::tick() 
{
	const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_W) == GLFW_PRESS)
        camera_pos += cameraSpeed * m.camera.getLookVector();
    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_S) == GLFW_PRESS)
        camera_pos -= cameraSpeed * m.camera.getLookVector();
    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_A) == GLFW_PRESS)
        camera_pos -= glm::normalize(glm::cross(m.camera.getLookVector(), m.camera.getUpVector())) * cameraSpeed;
    if (glfwGetKey(VoxelGame::getWindow().getHandle(), GLFW_KEY_D) == GLFW_PRESS)
        camera_pos += glm::normalize(glm::cross(m.camera.getLookVector(), m.camera.getUpVector())) * cameraSpeed;
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

	m.camera.setPosition(camera_pos);
	m.camera.setYaw(yaw);
	m.camera.setPitch(pitch);
	return true;
}

void GameLayer::resize(int width, int height) 
{
	m.camera.setAspectRatio((float)width / (float)height);
}

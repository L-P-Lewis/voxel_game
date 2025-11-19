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
#include "worldgen.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "mouse_handler.h"
#include "render_texture.h"
#include "texture_attachment.h"

Chunk chunk;

glm::vec3 camera_pos = glm::vec3(1, 32, 1);

GameLayer* GameLayer::New()
{
	BlockRegistry registry;
	Camera camera;
	Texture terrain;

	TextureAttachment depth = TextureAttachment::createDepthAttachment();
	TextureAttachment color = TextureAttachment(GL_COLOR_ATTACHMENT0, "color");
	RenderTexture framebuffer = RenderTexture({depth, color});

	registry.RegisterBlock("air", Block::AllSides(0));
	registry.RegisterBlock("stone", Block::AllSides(1));
	registry.RegisterBlock("dirt", Block::AllSides(2));
	registry.RegisterBlock("grass", Block::Barrel(3, 4, 2));
	registry.RegisterBlock("snow", Block::AllSides(5));
	registry.RegisterBlock("gravel", Block::AllSides(6));
	registry.RegisterBlock("sand", Block::AllSides(7));
	registry.RegisterBlock("water", Block::Fluid(8));


	Shader shader = VoxelGame::getAssetManager().GetShader("chunk_shader");
	terrain = VoxelGame::getAssetManager().GetTexture("terrain");

	camera.setFov(45);
	camera.setNearPlane(0.1f);
	camera.setFarPlane(1000.0f);
	camera.setPosition(glm::dvec3(1.0, 1.0, 1.0));
	camera.setYaw(90);
	camera.setRoll(0);
	camera.setPitch(0);
	camera.setPerspective();
	camera.setAspectRatio(VoxelGame::getWindow().getWidth() / static_cast<float>(VoxelGame::getWindow().getHeight()));
	camera.recalculateProjectionMatrix();
	camera.recalculateViewMatrix();

	GameWorld world = GameWorld::New();

	WorldGenerator worldgen;

	return new GameLayer(M{
		.camera = camera,
		.registry = registry,
		.terrain = terrain,
		.chunk_shader = shader,
		.world = world,
		.generator = worldgen,
		.framebuffer = framebuffer
	});
}

void GameLayer::render(float deltaTime) 
{
	MouseHandler::turnCamera(m.camera);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	m.world.Draw(m.camera, m.chunk_shader, m.terrain);
}

bool GameLayer::tick() 
{
	const float cameraSpeed = 0.8f; // adjust accordingly
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

	m.camera.setPosition(camera_pos);
	m.world.UpdateActiveChunks(WorldPosition(camera_pos.x, camera_pos.y, camera_pos.z), &m.registry, &m.generator);
	return true;
}

void GameLayer::resize(int width, int height) 
{
	m.camera.setAspectRatio((float)width / (float)height);
}

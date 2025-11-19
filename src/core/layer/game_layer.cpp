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
#include <ctime>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

#include "mouse_handler.h"
#include "render_texture.h"
#include "texture_attachment.h"

Chunk chunk;

glm::vec3 camera_pos = glm::vec3(1, 32, 1);

void GenerateFaceSky(glm::vec3 dl, glm::vec3 ul, glm::vec3 dr, glm::vec3 ur, int texture, glm::vec3 normal, unsigned int &index, std::vector<ChunkVertex> &vertices, std::vector<unsigned int> &indices)
{
	float tex_uv_size = 1.0 / 4.0;
	int dtx = texture % 4;
	int dty = texture / 4;
	glm::vec2 base_uv(dtx * tex_uv_size, dty * tex_uv_size);

	glm::vec2 tl_uv = base_uv;
	glm::vec2 br_uv = base_uv + glm::vec2(tex_uv_size, tex_uv_size);
	glm::vec2 bl_uv = base_uv + glm::vec2(0.0, tex_uv_size);
	glm::vec2 tr_uv = base_uv + glm::vec2(tex_uv_size, 0.0);

	vertices.push_back(ChunkVertex(dl, normal, br_uv));
	unsigned int down_1 = index++;
	vertices.push_back(ChunkVertex(ul, normal, tr_uv)); 
	unsigned int down_2 = index++;
	vertices.push_back(ChunkVertex(dr, normal, bl_uv));
	unsigned int down_3 = index++;
	vertices.push_back(ChunkVertex(ur, normal, tl_uv));
	unsigned int down_4 = index++;

	indices.push_back(down_3);
	indices.push_back(down_2);
	indices.push_back(down_1);

	indices.push_back(down_3);
	indices.push_back(down_4);
	indices.push_back(down_2);
}

ChunkMesh GenerateSkybox()
{
	std::vector<ChunkVertex> verts;
	std::vector<unsigned int> inds;
	unsigned int index = 0;

	// topface
	glm::vec3 dsw(-1, -1, -1);
	glm::vec3 dnw(-1, -1, 1);
	glm::vec3 dse(1, -1, -1);
	glm::vec3 dne(1, -1, 1);
	glm::vec3 usw(-1, 1, -1);
	glm::vec3 unw(-1, 1, 1);
	glm::vec3 use(1, 1, -1);
	glm::vec3 une(1, 1, 1);

	GenerateFaceSky(use, usw, une, unw, 5, glm::vec3(0, -1, 0), index, verts, inds);
	GenerateFaceSky(dnw, dsw, dne, dse, 13, glm::vec3(0, -1, 0), index, verts, inds);
	GenerateFaceSky(dne, une, dnw, unw, 8, glm::vec3(0, 0, 1), index, verts, inds);
	GenerateFaceSky(dsw, usw, dse, use, 10, glm::vec3(0, 0, -1), index, verts, inds);
	GenerateFaceSky(dse, use, dne, une, 9, glm::vec3(1, 0, 0), index, verts, inds);
	GenerateFaceSky(dnw, unw, dsw, usw, 11, glm::vec3(-1, 0, 0), index, verts, inds);
	return ChunkMesh(verts, inds);
}

GameLayer* GameLayer::New()
{
	BlockRegistry registry;
	Camera camera;
	Texture terrain;
	Texture skybox;

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
	Shader sky_shader = VoxelGame::getAssetManager().GetShader("sky_shader");
	terrain = VoxelGame::getAssetManager().GetTexture("terrain");
	skybox = VoxelGame::getAssetManager().GetTexture("skybox");




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

	WorldGenerator worldgen(time(NULL));

	return new GameLayer(M{
		.camera = camera,
		.registry = registry,
		.terrain = terrain,
		.skybox = skybox,
		.skybox_mesh = GenerateSkybox(),
		.chunk_shader = shader,
		.sky_shader = sky_shader,
		.world = world,
		.generator = worldgen,
		.framebuffer = framebuffer
	});
}

void GameLayer::render(float deltaTime) 
{
	MouseHandler::turnCamera(m.camera);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 projection(1.0f);
	projection = m.camera.getProjectionMatrix();
	glm::mat4 view(1.0f);
	view = m.camera.getViewMatrix();

	m.sky_shader.use();
	m.sky_shader.set_uniform("projection", projection);
	m.sky_shader.set_uniform("view", view);
	glm::vec3 campow = m.camera.getPosition();
	//ampow += glm::vec3(0, 2, 0);
	m.sky_shader.set_uniform("model", glm::translate(glm::mat4(1), campow));
	m.skybox.Use();
	m.skybox_mesh.Draw(m.sky_shader);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m.chunk_shader.use();
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

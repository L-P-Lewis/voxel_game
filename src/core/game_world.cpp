#include "game_world.h"
#include "assets/shader.h"
#include "assets/texture.h"
#include "camera.h"
#include <vector>
#include "glad/glad.h"


GameWorld GameWorld::New() 
{
	std::vector<Chunk> chunks;

	return GameWorld(M {
			.chunks = chunks
	});
}


void GameWorld::Draw(Camera &camera, Shader &shader, Texture &terrain)
{

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model_matrix( 1.0 );
	glm::mat4 projection(1.0f);
	projection = camera.getProjectionMatrix();
	glm::mat4 view(1.0f);
	view = camera.getViewMatrix();

	shader.set_uniform("projection", projection);
	shader.set_uniform("view", view);

	terrain.Use();

	for (Chunk chunk : m.chunks) {
		shader.set_uniform("model", chunk.GetChunkTransform());
		chunk.Draw(shader);
	}
}


void GameWorld::UpdateActiveChunks(WorldPosition player_pos)
{
	
}



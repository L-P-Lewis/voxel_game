#include "game_world.h"
#include "assets/shader.h"
#include "assets/texture.h"
#include "camera.h"
#include <climits>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "glad/glad.h"

#define H_RENDER 7
#define V_RENDER 4
#define MAX_CHUNKS ((H_RENDER + H_RENDER + 1) * (H_RENDER + H_RENDER + 1) * (V_RENDER + V_RENDER + 1)) 

GameWorld GameWorld::New() 
{
	std::vector<Chunk> chunks;
	std::map<ChunkPosition, ChunkID> active_chunks;
	std::vector<ChunkID> free_chunk_ids;
	chunks.reserve(MAX_CHUNKS);
	for (int i = 0; i < MAX_CHUNKS; i++) {
		free_chunk_ids.push_back(i);
		chunks.push_back(Chunk());
	}

	return GameWorld(M {
			.chunks = chunks,
			.free_chunk_ids = free_chunk_ids,
			.active_chunks = active_chunks,
	});
}


void GameWorld::ActivateChunk(ChunkPosition pos)
{
	if (IsChunkActive(pos)) return;
	ChunkID new_id = m.free_chunk_ids.back();
	m.free_chunk_ids.pop_back();

	if (m.active_chunks.contains(pos)) {
	}
	m.active_chunks[pos] = new_id;
	m.chunks[new_id].Init(pos);
	//for (int x = 0; x < 8; x++) {
	//	m.chunks[new_id].SetBlock(x, 0, 0, 1);
	//	m.chunks[new_id].SetBlock(0, x, 0, 1);
	//	m.chunks[new_id].SetBlock(0, 0, x, 1);
	//}
}


ChunkID GameWorld::GetChunkID(ChunkPosition pos)
{
	if (!m.active_chunks.contains(pos)) {
		return INT_MAX;
	}
	return m.active_chunks[pos];
}


void GameWorld::DeactivateChunk(ChunkPosition pos)
{
	if (!IsChunkActive(pos)) return;
	ChunkID rem_id = m.active_chunks[pos];
	m.active_chunks.erase(pos);
	m.free_chunk_ids.push_back(rem_id);

}


bool GameWorld::IsChunkActive(ChunkPosition pos)
{
	return m.active_chunks.contains(pos);
}


void GameWorld::Draw(Camera &camera, Shader &shader, Texture &terrain)
{

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection(1.0f);
	projection = camera.getProjectionMatrix();
	glm::mat4 view(1.0f);
	view = camera.getViewMatrix();

	shader.set_uniform("projection", projection);
	shader.set_uniform("view", view);

	terrain.Use();

	for (auto const& [pos, id] : m.active_chunks) {
		shader.set_uniform("model", m.chunks[id].GetChunkTransform());

		m.chunks[id].Draw(shader);
	}
}


void GameWorld::UpdateActiveChunks(WorldPosition player_pos, BlockRegistry *registry, WorldGenerator *generator)
{
	//std::cout << "Chunk : ";
	//std::cout << player_pos.chunk_x << " " << player_pos.chunk_y << " " << player_pos.chunk_z << std::endl;
	// Calculate set of all chunks that should be active right now
	std::set<ChunkPosition> needed_chunk_set;

	for (int x = -H_RENDER; x <= H_RENDER; x++) {
		for (int y = -V_RENDER; y <= V_RENDER; y++) {
			for (int z = -H_RENDER; z <= H_RENDER; z++) {
				needed_chunk_set.insert({
					player_pos.chunk_x + x,
					player_pos.chunk_y + y,
					player_pos.chunk_z + z
					});
			}
		}
	}
	
	// Remove from that set all chunks that are active
	// Keep track of set of active chunks that should be inactive
	std::set<ChunkPosition> remove_chunk_set;
	for (auto const& [pos, id] : m.active_chunks) {
		if (needed_chunk_set.contains(pos)) {
			needed_chunk_set.erase(pos);
		} else {
			remove_chunk_set.insert(pos);
		}
	}
	
	// Deactivate all chunks that should be inactive
	for (ChunkPosition pos : remove_chunk_set) {
		DeactivateChunk(pos);
	}

	for (ChunkPosition pos : needed_chunk_set) {
		ActivateChunk(pos);
		ChunkID id = GetChunkID(pos);
		generator->PopulateChunk(&m.chunks[id]);
		m.chunks[id].RegnerateMesh(registry);
	}
}



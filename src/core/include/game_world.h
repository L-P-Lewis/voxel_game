#pragma once


// Struct for storing canonical world positions in terms of chunk position and offset
#include "assets/shader.h"
#include "assets/texture.h"
#include "camera.h"
#include "chunk.h"
#include "worldgen.h"
#include <cmath>
#include <map>
#include <utility>
#include <vector>

struct WorldPosition {
	int chunk_x, chunk_y, chunk_z;
	int block_x, block_y, block_z;
	WorldPosition(int x, int y, int z) :
		chunk_x(0),
		chunk_y(0),
		chunk_z(0),
		block_x(x),
		block_y(y),
		block_z(z) {Canonize();};
	void Canonize() {
		int g_x = block_x + chunk_x * 16;
		int g_y = block_y + chunk_y * 16;
		int g_z = block_z + chunk_z * 16;

		chunk_x = floor((double)g_x / 16.0);
		chunk_y = floor((double)g_y / 16.0);
		chunk_z = floor((double)g_z / 16.0);
	};
};

typedef unsigned int ChunkID;

class GameWorld {
	private:
		struct M {
			std::vector<Chunk> chunks;
			std::vector<ChunkID> free_chunk_ids;
			std::map<ChunkPosition, ChunkID> active_chunks;
		} m;
		explicit GameWorld(M m) : m(std::move(m)) {};
		void ActivateChunk(ChunkPosition pos);
		void DeactivateChunk(ChunkPosition pos);
		ChunkID GetChunkID(ChunkPosition pos);
		bool IsChunkActive(ChunkPosition pos);

	public:
		static GameWorld New();
		void Draw(Camera &camera, Shader &shader, Texture &terrain);
		void UpdateActiveChunks(WorldPosition player_pos, BlockRegistry *registry, WorldGenerator *generator);

};

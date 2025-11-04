#pragma once


// Struct for storing canonical world positions in terms of chunk position and offset
#include "assets/shader.h"
#include "assets/texture.h"
#include "camera.h"
#include "chunk.h"
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
		block_z(x) {Canonize();};
	void Canonize() {
		block_x += chunk_x * 16;
		block_y += chunk_y * 16;
		block_z += chunk_z * 16;
		chunk_x = block_x / 16;
		chunk_y = block_y / 16;
		chunk_z = block_z / 16;
		block_x = block_x % 16;
		block_x = block_x % 16;
		block_y = block_y % 16;
		block_z = block_z % 16;
	};
};


class GameWorld {
	private:
		struct M {
			std::vector<Chunk> chunks;
		} m;
		explicit GameWorld(M m) : m(std::move(m)) {};
	public:
		static GameWorld New();
		void Draw(Camera &camera, Shader &shader, Texture &terrain);
		void UpdateActiveChunks(WorldPosition player_pos);
};

#pragma once


#include "assets/shader.h"
#include "block.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <vector>


#define CHUNK_SIZE 16


struct ChunkVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	ChunkVertex(glm::vec3 p, glm::vec3 n, glm::vec2 u) : position(p), normal(n), uv(u) {};
};


class ChunkMesh {
	private:
		std::vector<ChunkVertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int VAO, VBO, EBO;
		void SetupMesh();
	public:
		ChunkMesh(std::vector<ChunkVertex> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {this->SetupMesh();};
		void Draw(Shader &shader);

		
};

struct ChunkPosition {
	int x, y, z;
};


inline bool operator< (const ChunkPosition& lhs, const ChunkPosition& rhs) { 
	if (lhs.x < rhs.x) return true;
	if (lhs.x > rhs.x) return false;
	if (lhs.y < rhs.y) return true;
	if (lhs.y > rhs.y) return false;
	if (lhs.z < rhs.z) return true;
	if (lhs.z > rhs.z) return false;
	return false;
}
inline bool operator> (const ChunkPosition& lhs, const ChunkPosition& rhs) { return rhs < lhs; }
inline bool operator<=(const ChunkPosition& lhs, const ChunkPosition& rhs) { return !(lhs > rhs); }
inline bool operator>=(const ChunkPosition& lhs, const ChunkPosition& rhs) { return !(lhs < rhs); }
inline bool operator==(const ChunkPosition& lhs, const ChunkPosition& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
inline bool operator!=(const ChunkPosition& lhs, const ChunkPosition& rhs) { return !(lhs == rhs); }



class Chunk {
	private:
		BlockHandle blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
		ChunkPosition pos;
		ChunkMesh *mesh;
	public:
		void Init(ChunkPosition position) {
			pos = position;
			Fill(0);
		};
		void Fill(BlockHandle handle);
		void RegnerateMesh(BlockRegistry *block_registry);
		void Draw(Shader &shader);
		glm::mat4 GetChunkTransform() const;
		ChunkPosition GetPosition() const {return pos;};
		BlockHandle GetBlock(int x, int y, int z) {
			if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
				return 0;
			}
			return blocks[x][y][z];
		}
		void SetBlock(int x, int y, int z, BlockHandle target) {
			if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
				return;
			}
			blocks[x][y][z] = target;
		}

};

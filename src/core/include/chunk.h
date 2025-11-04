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


class Chunk {
	private:
		BlockHandle blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
		int chunk_x, chunk_y, chunk_z;
		ChunkMesh *mesh;
	public:
		void Fill(BlockHandle handle);
		void RegnerateMesh(BlockRegistry *block_registry);
		void Draw(Shader &shader);
		glm::mat4 GetChunkTransform() const;
};

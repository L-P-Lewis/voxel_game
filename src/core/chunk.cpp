#include "chunk.h"
#include "block.h"
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include <vector>


void ChunkMesh::SetupMesh() {
	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ChunkVertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (void*)offsetof(ChunkVertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (void*)offsetof(ChunkVertex, uv));

    glBindVertexArray(0);
}

void ChunkMesh::Draw(Shader &shader)
{
	glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void Chunk::Fill(BlockHandle handle)
{
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				this->blocks[x][y][z] = handle;
			}
		}
	}
	this->RegnerateMesh();
}


void Chunk::RegnerateMesh()
{
	if (this->mesh != nullptr) {
		delete this->mesh;
	}

	std::vector<ChunkVertex> vertices;
	std::vector<unsigned int> indices;
	
	unsigned int index = 0;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				// Test function, just generate a single triangle
				glm::vec3 base(x, y, z);
				glm::vec3 up(0.0, 1.0, 0.0);
				glm::vec2 uv(0.0, 0.0);

				ChunkVertex v1 = (ChunkVertex){
					base, up, uv
				};
				ChunkVertex v2 = (ChunkVertex){
					base + glm::vec3(1.0, 0.0, 0.0), up, uv
				};
				ChunkVertex v3 = (ChunkVertex){
					base + glm::vec3(0.0, 0.0, 1.0), up, uv
				};
				vertices.push_back(v1);
				vertices.push_back(v2);
				vertices.push_back(v3);
				indices.push_back(index++);
				indices.push_back(index++);
				indices.push_back(index++);
			}
		}
	}

	this->mesh = new ChunkMesh(vertices, indices);
}

void Chunk::Draw(Shader &shader)
{
	if (mesh != nullptr) mesh->Draw(shader);
}

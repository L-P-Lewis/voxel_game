#include "chunk.h"
#include "block.h"
#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"
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
	shader.use();
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
}


void Chunk::RegnerateMesh(BlockRegistry *block_registry)
{
	if (this->mesh != nullptr) {
		delete this->mesh;
	}

	std::vector<ChunkVertex> vertices;
	std::vector<unsigned int> indices;

	float texture_size = 1.0;
	
	unsigned int index = 0;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				Block block = block_registry->GetBlock(blocks[x][y][z]);
				glm::vec3 dsw(x, y, z);
				glm::vec3 dnw(x, y, z + 1);
				glm::vec3 dse(x + 1, y, z);
				glm::vec3 dne(x + 1, y, z + 1);

				
				int dtx = block.down_texture % 8;
				int dty = block.down_texture / 8;
				glm::vec2 down_uv(0, 0);
				glm::vec3 down_normal = glm::vec3(0, -1, 0);

				vertices.push_back(ChunkVertex(dsw, down_normal, glm::vec2(0.0, 0.0)));
				unsigned int down_1 = index++;
				vertices.push_back(ChunkVertex(dnw, down_normal, glm::vec2(0.0, 1.0))); 
				unsigned int down_2 = index++;
				vertices.push_back(ChunkVertex(dse, down_normal, glm::vec2(1.0, 0.0)));
				unsigned int down_3 = index++;
				vertices.push_back(ChunkVertex(dne, down_normal, glm::vec2(1.0, 1.0)));
				unsigned int down_4 = index++;

				indices.push_back(down_1);
				indices.push_back(down_2);
				indices.push_back(down_3);

				indices.push_back(down_2);
				indices.push_back(down_4);
				indices.push_back(down_3);


				
			}
		}
	}

	this->mesh = new ChunkMesh(vertices, indices);
}

void Chunk::Draw(Shader &shader)
{
	if (mesh != nullptr) mesh->Draw(shader);
}

glm::mat4 Chunk::GetChunkTransform() const {

	return glm::translate(glm::mat4(1), glm::vec3(chunk_x * CHUNK_SIZE, chunk_y * CHUNK_SIZE, chunk_z * CHUNK_SIZE));
}

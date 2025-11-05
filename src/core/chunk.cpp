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


void GenerateFace(glm::vec3 dl, glm::vec3 ul, glm::vec3 dr, glm::vec3 ur, int texture, glm::vec3 normal, unsigned int &index, std::vector<ChunkVertex> &vertices, std::vector<unsigned int> &indices)
{
	float tex_uv_size = 1.0 / 16.0;
	int dtx = texture % 16;
	int dty = texture / 16;
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

	indices.push_back(down_1);
	indices.push_back(down_2);
	indices.push_back(down_3);

	indices.push_back(down_2);
	indices.push_back(down_4);
	indices.push_back(down_3);
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
				if (blocks[x][y][z] == 0) continue;
				Block block = block_registry->GetBlock(blocks[x][y][z]);

				glm::vec3 dsw(x, y, z);
				glm::vec3 dnw(x, y, z + 1);
				glm::vec3 dse(x + 1, y, z);
				glm::vec3 dne(x + 1, y, z + 1);
				glm::vec3 usw(x, y + 1, z);
				glm::vec3 unw(x, y + 1, z + 1);
				glm::vec3 use(x + 1, y + 1, z);
				glm::vec3 une(x + 1, y + 1, z + 1);

				// UD Faces (Y Axis)
				if (y == CHUNK_SIZE - 1 || blocks[x][y + 1][z] == 0) {
					GenerateFace(usw, unw, use, une, block.up_texture, glm::vec3(0, 1, 0), index, vertices, indices); 
				}
				if (y == 0 || blocks[x][y - 1][z] == 0) {
					GenerateFace(dnw, dsw, dne, dse, block.down_texture, glm::vec3(0, -1, 0), index, vertices, indices);
				}

				// NS Faces (Z Axis)
				if (z == CHUNK_SIZE - 1 || blocks[x][y][z + 1] == 0) {
					GenerateFace(dne, une, dnw, unw, block.north_texture, glm::vec3(0, 0, 1), index, vertices, indices);
				}
				if (z == 0 || blocks[x][y][z - 1] == 0) {
					GenerateFace(dsw, usw, dse, use, block.south_texture, glm::vec3(0, 0, -1), index, vertices, indices);
				}

				// EW Faces (X Axis)
				if (x == CHUNK_SIZE - 1 || blocks[x+1][y][z] == 0) {
					GenerateFace(dse, use, dne, une, block.west_texture, glm::vec3(1, 0, 0), index, vertices, indices);
				}
				if (x == 0  || blocks[x-1][y][z] == 0) {
					GenerateFace(dnw, unw, dsw, usw, block.west_texture, glm::vec3(-1, 0, 0), index, vertices, indices);
				}

				
			}
		}
	}
	if (indices.empty()) {
		this->mesh = nullptr;
		return;
	}

	this->mesh = new ChunkMesh(vertices, indices);
}

void Chunk::Draw(Shader &shader)
{
	if (mesh != nullptr) mesh->Draw(shader);
}

glm::mat4 Chunk::GetChunkTransform() const {

	return glm::translate(glm::mat4(1), glm::vec3(pos.x * CHUNK_SIZE, pos.y* CHUNK_SIZE, pos.z * CHUNK_SIZE));
}

#include "worldgen.h"
#include "chunk.h"
#include <cmath>


void GenerateColumn(Chunk *chunk, int cx, int cz, int floor_height)
{
	int chunk_floor = chunk->GetPosition().y * 16;
	int relative_floor = floor_height - chunk_floor;
	if (relative_floor < 0) return;
	if (relative_floor > 16) relative_floor = 16;
	for (int y = 0; y < relative_floor; y++) {
		chunk->SetBlock(cx, y, cz, 1);
	}
}


void WorldGenerator::PopulateChunk(Chunk *chunk)
{
	ChunkPosition cpos = chunk->GetPosition();
	for (int cx = 0; cx < 16; cx++) {
		for (int cz = 0; cz < 16; cz++) {
			int x = cpos.x * 16 + cx;
			int z = cpos.z * 16 + cz;

			float w = x + z;

			float fh = 8 * std::sin(w * 0.1);


			GenerateColumn(chunk, cx, cz, fh);
		}
	}
}

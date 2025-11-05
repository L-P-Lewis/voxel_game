#include "worldgen.h"
#include "FastNoise.h"
#include "chunk.h"
#include <cmath>


WorldGenerator::WorldGenerator()
{
	noise.SetNoiseType(FastNoise::SimplexFractal);
	noise.SetFractalOctaves(8);
	noise.SetFrequency(0.005);
}


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


			float fh = 64 * noise.GetNoise((float)x, (float)z);


			GenerateColumn(chunk, cx, cz, fh);
		}
	}
}

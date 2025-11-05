#include "worldgen.h"
#include "FastNoise.h"
#include "chunk.h"
#include <cmath>
#include <cstdlib>


WorldGenerator::WorldGenerator()
{
	noise.SetNoiseType(FastNoise::SimplexFractal);
	noise.SetFractalOctaves(8);
	noise.SetFrequency(0.005);
}


void GenerateColumn(Chunk *chunk, int cx, int cz, int floor_height, int toplayer_width)
{
	int chunk_floor = chunk->GetPosition().y * 16;
	int relative_floor = floor_height - chunk_floor;
	if (relative_floor < 0) return;
	if (relative_floor > 16) relative_floor = 16;
	for (int y = 0; y < relative_floor; y++) {
		int gy = chunk_floor + y;
		int depth = floor_height - gy;
		int block = 1;
		if (depth <= toplayer_width) {
			block = 2;
			if (depth == 1) {
				block = 3;
			}
			if (floor_height > 16) {
				block = 4;
			}
		}
		chunk->SetBlock(cx, y, cz, block);
	}
}


void WorldGenerator::PopulateChunk(Chunk *chunk)
{
	ChunkPosition cpos = chunk->GetPosition();
	for (int cx = 0; cx < 16; cx++) {
		for (int cz = 0; cz < 16; cz++) {
			int x = cpos.x * 16 + cx;
			int z = cpos.z * 16 + cz;


			float fh = noise.GetNoise((float)x, (float)z);

			float s1 = noise.GetNoise((float)x + 0.5, (float)z);
			float s2 = noise.GetNoise((float)x, (float)z + 0.5);

			float slope = std::abs(fh - s1) + std::abs(fh - s2);

			// Slope will range form 0-~0.1

			int dirt_size = 5.0 - floor(slope * 200);


			GenerateColumn(chunk, cx, cz,  64 * fh, dirt_size);
		}
	}
}

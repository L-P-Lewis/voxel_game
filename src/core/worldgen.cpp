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
	continentalness.SetNoiseType(FastNoise::SimplexFractal);
	continentalness.SetFractalOctaves(1);
	continentalness.SetFrequency(0.00001);
}


void GenerateColumn(Chunk *chunk, int cx, int cz, int floor_height, int toplayer_width, int snow_height)
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
			if (floor_height > snow_height) {
				block = 4;
			}
		}
		chunk->SetBlock(cx, y, cz, block);
	}
}


float SampleFloorHeight(FastNoise &base, FastNoise &continentalness, float x, float z)
{

	float fh = (base.GetNoise((float)x, (float)z) + 1.0) / 2.0;
	float ct = (base.GetNoise((float)x, (float)z) + 1.0) /2.0;

	ct = ct * ct * ct;
	float map_ct = (3 * ct * ct) - (2 * ct * ct * ct);
	//map_ct = map_ct < 0.5 ? 0.0 : 1.0;
	float min_height = 0.0 + 25.0 * map_ct;
	float max_height = 3.0 + 128.0 * map_ct;
	return min_height + fh * (max_height - min_height);
}


void WorldGenerator::PopulateChunk(Chunk *chunk)
{
	ChunkPosition cpos = chunk->GetPosition();
	for (int cx = 0; cx < 16; cx++) {
		for (int cz = 0; cz < 16; cz++) {

			int x = cpos.x * 16 + cx;
			int z = cpos.z * 16 + cz;

			float fh = SampleFloorHeight(noise, continentalness,(float)x, (float)z);


			float s1 = SampleFloorHeight(noise, continentalness, (float)x + 0.5, (float)z);
			float s2 = SampleFloorHeight(noise, continentalness, (float)x, (float)z + 0.5);

			float slope = std::abs(fh - s1) + std::abs(fh - s2);

			// Slope will range form 0-~0.1

			int dirt_size = 5.0 - floor(slope * 2);

			float sh = noise.GetNoise(z, x) * 16 + 32; 

			GenerateColumn(chunk, cx, cz, fh, dirt_size, sh);
		}
	}
}

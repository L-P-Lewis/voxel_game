#include "worldgen.h"
#include "FastNoise.h"
#include "chunk.h"
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>


WorldGenerator::WorldGenerator()
{
	noise.SetNoiseType(FastNoise::SimplexFractal);
	noise.SetFractalOctaves(8);
	noise.SetFrequency(0.005);
	continentalness.SetNoiseType(FastNoise::SimplexFractal);
	continentalness.SetFractalOctaves(1);
	continentalness.SetFrequency(0.1);
	cave_noise.SetNoiseType(FastNoise::Simplex);
	cave_noise.SetFrequency(0.05);

	// Ocean Biome
	world_biomes.push_back({
		.min_cont = -1.0,
		.max_cont = -0.5,
		.terain_min = -40,
		.terain_max = -10,
		.upper_block = 5
	});
	// Beach
	world_biomes.push_back({
		.min_cont = -0.5,
		.max_cont = -0.1,
		.terain_min = 0,
		.terain_max = 5,
		.upper_block = 6
	});
	// Plains
	world_biomes.push_back({
		.min_cont = -0.1,
		.max_cont = 0.7,
		.terain_min = 5.0,
		.terain_max = 15.0,
		.upper_block = 3,
		.toper_block = 2
	});
	// Mountains
	world_biomes.push_back({
		.min_cont = 0.7,
		.max_cont = 1.0,
		.terain_min = 40.0,
		.terain_max = 60.0,
		.upper_block = 1
	});
}


BiomeDef WorldGenerator::GetBiomeRaw(ChunkPosition map_position)
{
	if (world_biome_cache.contains(map_position)) {
		return world_biome_cache.at(map_position);
	}
	float cont = continentalness.GetNoise(map_position.x, map_position.z);
	BiomeDef best_fit = world_biomes.front();
	bool found_biome_fit = false;;
	for (BiomeDef biome : world_biomes) {
		if (biome.CheckFit(cont)) {
			best_fit = biome;
			found_biome_fit = true;
		}
	}
	world_biome_cache.emplace(map_position, best_fit);
	return best_fit;
}



BiomeDef WorldGenerator::GetBiome(ChunkPosition map_position)
{
	int test_x = floor(map_position.x / 8.0);
	int test_z = floor(map_position.z / 8.0);
	float blend_ud = (map_position.z - (test_z * 8)) / 8.0f;
	float blend_rl = (map_position.x - (test_x * 8)) / 8.0f;

	blend_rl = 1.0 - blend_rl;
	blend_ud = 1.0 - blend_ud;

	ChunkPosition tl_pos = {test_x, 0, test_z};
	BiomeDef tl_biome = GetBiomeRaw(tl_pos);
	ChunkPosition bl_pos = {test_x, 0, test_z + 1};
	BiomeDef bl_biome = GetBiomeRaw(bl_pos);

	ChunkPosition tr_pos = {test_x + 1, 0, test_z};
	BiomeDef tr_biome = GetBiomeRaw(tr_pos);
	ChunkPosition br_pos = {test_x + 1, 0, test_z + 1};
	BiomeDef br_biome = GetBiomeRaw(br_pos);


	return BiomeDef::blend(BiomeDef::blend(tl_biome, bl_biome, blend_ud), BiomeDef::blend(tr_biome, br_biome, blend_ud), blend_rl);
}




float WorldGenerator::GetHeightmapValue(ChunkPosition map_position)
{
	if (world_height_cache.contains(map_position)) {
		return world_height_cache.at(map_position);
	}
	float pos_sample = noise.GetNoise(map_position.x, map_position.z);
	pos_sample = (pos_sample + 1.0) / 2.0;
	world_height_cache.emplace(map_position, pos_sample);
	return pos_sample;
}


void CarveCaves(Chunk *chunk, int cx, int cz, int cave_height, FastNoise &cave_noise) 
{
	
	int chunk_floor = chunk->GetPosition().y * CHUNK_SIZE;
	int x = chunk->GetPosition().x * CHUNK_SIZE + cx;
	int z = chunk->GetPosition().z * CHUNK_SIZE + cz;
	for (int cy = 0; cy < CHUNK_SIZE; cy++) {
		int y = chunk_floor + cy;
		float base_cave_power = (cave_noise.GetNoise(x, y, z) + 1.0) / 2.0;

		base_cave_power *= glm::clamp(0.1 * (cave_height - y), 0.0, 1.0);

		if (base_cave_power > 0.5) {
			chunk->SetBlock(cx, cy, cz, 0);
		}
	}
}

void WorldGenerator::ClearCache()
{
	world_biome_cache.clear();
	world_height_cache.clear();
}

void GenerateColumn(Chunk *chunk, int cx, int cz, int floor_height, int toplayer_width, int upper_block, int toper_block)
{
	int chunk_floor = chunk->GetPosition().y * CHUNK_SIZE;
	int relative_floor = floor_height - chunk_floor;
	if (relative_floor < 0) return;
	if (relative_floor > CHUNK_SIZE) relative_floor = CHUNK_SIZE;
	for (int y = 0; y < relative_floor; y++) {
		int gy = chunk_floor + y;
		int depth = floor_height - gy;
		int block = 1;
		if (depth <= toplayer_width) {
			block = upper_block;
			if (depth == 1) {
				block = toper_block;
			}
		}
		chunk->SetBlock(cx, y, cz, block);
	}
}




void WorldGenerator::PopulateChunk(Chunk *chunk)
{
	ChunkPosition cpos = chunk->GetPosition();
	for (int cx = 0; cx < CHUNK_SIZE; cx++) {
		for (int cz = 0; cz < CHUNK_SIZE; cz++) {

			int x = cpos.x * CHUNK_SIZE + cx;
			int z = cpos.z * CHUNK_SIZE + cz;
			BiomeDef biome = GetBiome({x, 0, z});

			float base_heightmap = GetHeightmapValue({x, 0, z});

			float fh = biome.MapHeight(base_heightmap);
			float s1 = biome.MapHeight(GetHeightmapValue({x + 1, 0 ,z}));
			float s2 = biome.MapHeight(GetHeightmapValue({x, 0 ,z + 1}));

			float slope = std::abs(fh - s1) + std::abs(fh - s2);

			int upper_block = biome.upper_block;
			int toper_block = biome.toper_block;
			if (toper_block == -1) {
				toper_block = upper_block;
			}

			// Slope will range form 0-~0.1

			int dirt_size = 5.0 - floor(slope * 2);


			GenerateColumn(chunk, cx, cz, fh, dirt_size, toper_block, upper_block);

			//float cave_height = fh + cave_noise.GetNoise(x, 100, z) * 16 - 4.0;
			//CarveCaves(chunk, cx, cz, cave_height, cave_noise);
		}
	}
}

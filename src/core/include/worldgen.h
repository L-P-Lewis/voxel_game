#pragma once


#include "FastNoise.h"
#include "chunk.h"
#include "glm/fwd.hpp"
#include <map>
#include <vector>


#define LERP(from, to, blend) ((float)from * blend) + ((float)to * (1.0f - blend))
#define CUTOFF(from, to, blend) (blend > 0.5 ? from : to)

struct BiomeDef 
{
	float min_cont = -1.0;
	float max_cont = 1.0;
	float terain_min = 0.0;
	float terain_max = 0.0;
	int upper_block = 1;
	int toper_block = -1;
	static BiomeDef blend(BiomeDef from, BiomeDef to, float blend) {
		return BiomeDef {
				.min_cont = LERP(from.max_cont, to.max_cont, blend),
				.max_cont = LERP(from.min_cont, to.min_cont, blend),
				.terain_min = LERP(from.terain_min, to.terain_min, blend),
				.terain_max = LERP(from.terain_max, to.terain_max, blend),
				.upper_block = CUTOFF(from.upper_block, to.upper_block, blend),
				.toper_block = CUTOFF(from.toper_block, to.toper_block, blend)
		};
	};
	bool CheckFit(float continentalness)
	{
		if (continentalness > max_cont) {return false;}
		if (continentalness < min_cont) {return false;}
		return true;
	}
	float MapHeight(float height)
	{
		return height * (terain_max - terain_min) + terain_min;
	}
};

class WorldGenerator {
	private:
		FastNoise noise;
		FastNoise continentalness;
		FastNoise cave_noise;
		FastNoise biome_blend_noise;
		std::map<ChunkPosition, float> world_height_cache;
		std::map<ChunkPosition, BiomeDef> world_biome_cache;
		std::vector<BiomeDef> world_biomes;
		BiomeDef GetBiomeRaw(ChunkPosition map_position);
	public:
		WorldGenerator();
		void ClearCache();
		void PopulateChunk(Chunk *chunk);
		float GetHeightmapValue(ChunkPosition map_position);
		BiomeDef GetBiome(ChunkPosition map_position);
};

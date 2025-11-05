#pragma once


#include "FastNoise.h"
#include "chunk.h"

class WorldGenerator {
	private:
		FastNoise noise;
		FastNoise continentalness;
		FastNoise cave_noise;
	public:
		WorldGenerator();
		void PopulateChunk(Chunk *chunk);
};

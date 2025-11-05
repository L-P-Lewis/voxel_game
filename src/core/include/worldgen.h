#pragma once


#include "FastNoise.h"
#include "chunk.h"

class WorldGenerator {
	private:
		FastNoise noise;
	public:
		WorldGenerator();
		void PopulateChunk(Chunk *chunk);
};

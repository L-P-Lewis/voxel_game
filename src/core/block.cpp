#include "block.h"


void BlockRegistry::RegisterBlock(std::string id, Block block)
{
	BlockHandle handle = this->blocks.size();
	this->blocks.push_back(block);
	this->index_map[id] = handle;
}

BlockHandle BlockRegistry::LookupBlock(std::string &id) const
{
	return this->index_map.at(id);
}

Block BlockRegistry::GetBlock(BlockHandle handle) const
{
	if (handle >= this->blocks.size()) return this->blocks.front();
	return this->blocks[handle];
}


Block Block::AllSides(uint8_t tex) {
	return {
		tex, tex, tex, tex, tex, tex
	};
}

Block Block::Barrel(uint8_t top, uint8_t sides, uint8_t bottom)
{
	return {
			.up_texture = top,
			.down_texture = bottom,
			.north_texture = sides,
			.south_texture = sides,
			.east_texture = sides,
			.west_texture = sides
	};
}

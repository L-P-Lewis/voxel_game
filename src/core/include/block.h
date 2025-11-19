#pragma once

/**
 * Represents a block type
 */
#include <cstdint>
#include <map>
#include <string>
#include <vector>
struct Block {
	uint8_t up_texture;
	uint8_t down_texture;
	uint8_t north_texture;
	uint8_t south_texture;
	uint8_t east_texture;
	uint8_t west_texture;
	uint8_t type = 0;
	static Block AllSides(uint8_t tex);
	static Block Barrel(uint8_t top, uint8_t sides, uint8_t bottom);
	static Block Fluid(uint8_t type) {Block ret = Block::AllSides(type); ret.type = 1; return ret;};
	bool occludes() {return type == 0 && up_texture != 0;}
};


typedef uint8_t BlockHandle;


class BlockRegistry {
	private:
		std::vector<Block> blocks;
		std::map<std::string, BlockHandle> index_map;
	public:
		void RegisterBlock(std::string id, Block block);
		BlockHandle LookupBlock(std::string &id) const;
		Block GetBlock(BlockHandle handle) const;
};

#pragma once

/**
 * Represents a block type
 */
#include <cstddef>
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
	static Block AllSides(uint8_t tex);
};


typedef size_t BlockHandle;


class BlockRegistry {
	private:
		std::vector<Block> blocks;
		std::map<std::string, BlockHandle> index_map;
	public:
		void RegisterBlock(std::string id, Block block);
		BlockHandle LookupBlock(std::string &id) const;
		Block GetBlock(BlockHandle handle) const;
};

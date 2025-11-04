#pragma once


#include "assets/shader.h"
#include <filesystem>
#include <string>


class AssetManager {
	private:
		std::filesystem::path base_path;
	public:
		AssetManager(std::filesystem::path base_path) : base_path(base_path) {};
		Shader GetShader(std::string shader_id);
};

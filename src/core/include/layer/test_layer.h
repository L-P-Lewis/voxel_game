#pragma once
#ifndef VOXEL_CORE_LAYER_TEST_LAYER_H_
#define VOXEL_CORE_LAYER_TEST_LAYER_H_

#include "voxel_game.h"
#include "app_layer.h"


// Test layer for initial milestone
class TestLayer : public AppLayer {
	public:
		void render(VoxelGame &game, float deltaTime) override;
		bool tick(VoxelGame &game) override;
		void resize(VoxelGame &game, int width, int height) override;
};


#endif

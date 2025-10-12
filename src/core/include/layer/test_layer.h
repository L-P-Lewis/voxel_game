#pragma once
#ifndef VOXEL_CORE_LAYER_TEST_LAYER_H_
#define VOXEL_CORE_LAYER_TEST_LAYER_H_

#include "voxel_game.h"
#include "app_layer.h"


// Test layer for initial milestone
class TestLayer : public AppLayer {
	public:
		void render(VoxelGame *game) override;
		bool tick(VoxelGame *game) override;
};


#endif

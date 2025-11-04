#pragma once
#ifndef VOXEL_CORE_LAYER_TEST_LAYER_H_
#define VOXEL_CORE_LAYER_TEST_LAYER_H_


#include "camera.h"
#include "voxel_game.h"
#include "app_layer.h"


// Test layer for initial milestone
class GameLayer : public AppLayer {
	private:
		Camera camera;
	public:
		GameLayer();
		void render(float deltaTime) override;
		bool tick() override;
		void resize(int width, int height) override;
};


#endif

#pragma once
#ifndef VOXEL_CORE_LAYER_TEST_LAYER_H_
#define VOXEL_CORE_LAYER_TEST_LAYER_H_


#include "camera.h"
#include "voxel_game.h"
#include "app_layer.h"


// Test layer for initial milestone
class TestLayer : public AppLayer {
	private:
		Camera camera;
	public:
		TestLayer();
		void render(float deltaTime) override;
		bool tick() override;
		void resize(int width, int height) override;
};


#endif

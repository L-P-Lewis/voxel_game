#pragma once
#include "assets/shader.h"
#include "assets/texture.h"
#include "block.h"
#include "game_world.h"
#include "worldgen.h"
#ifndef VOXEL_CORE_LAYER_TEST_LAYER_H_
#define VOXEL_CORE_LAYER_TEST_LAYER_H_


#include "camera.h"
#include "voxel_game.h"
#include "app_layer.h"


// Test layer for initial milestone
class GameLayer : public AppLayer {
	private:
		struct M {
			Camera camera;
			BlockRegistry registry;
			Texture terrain;
			Shader chunk_shader;
			GameWorld world;
			WorldGenerator generator;
		} m;
		explicit GameLayer(M m) : m(std::move(m)) {};
	public:
		static GameLayer* New();
		void render(float deltaTime) override;
		bool tick() override;
		void resize(int width, int height) override;
};


#endif

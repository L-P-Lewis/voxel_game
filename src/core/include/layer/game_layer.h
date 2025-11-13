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
#include "render_texture.h"
#include "window.h"


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
			RenderTexture framebuffer;
		} m;
		explicit GameLayer(M m) : m(std::move(m)) {
			m.framebuffer.createBuffers(VoxelGame::getWindow().getWidth(), VoxelGame::getWindow().getHeight());
			m.framebuffer.unbindWrite();
		};
	public:
		static GameLayer* New();
		void render(float deltaTime) override;
		bool tick() override;
		void resize(int width, int height) override;
};


#endif

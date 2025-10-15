#pragma once
#ifndef VOXEL_GAME_APP_LAYER_H_
#define VOXEL_GAME_APP_LAYER_H_

// Dummy declaration of Voxel Game
class VoxelGame;

/// Virtual class representing a "layer" of the application
/// Layers handle their own data, and act as a stack
class AppLayer {
	public:
	virtual ~AppLayer() = default;
	/// Runs the tick update for the current layer. Returns true if "lower" layers should also tick
	virtual bool tick() {return true;};
	/// Renders the layer
	virtual void render(float deltaTime) {};
	/// Called when the window is resized
	virtual void resize(int width, int height) {};
};


#endif

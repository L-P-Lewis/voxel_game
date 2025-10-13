#pragma once
#ifndef VOXEL_GAME_APP_LAYER_H_
#define VOXEL_GAME_APP_LAYER_H_

// Dummy declaration of Voxel Game
class VoxelGame;

/// Virtual class representing a "layer" of the application
/// Layers handle their own data, and act as a stack
class AppLayer {
	public:
	/// Runs the tick update for the current layer. Returns true if "lower" layers should also tick
	virtual bool tick(VoxelGame &game) {return true;};
	/// Renders the layer
	virtual void render(VoxelGame &game, float deltaTime) {};
	/// Called when the window is resized
	virtual void resize(VoxelGame &game, int width, int height) {};
};


#endif

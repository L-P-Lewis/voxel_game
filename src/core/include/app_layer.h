#pragma once
#include "VoxelGame.h"
#ifndef VOXEL_GAME_APP_LAYER_H_
#define VOXEL_GAME_APP_LAYER_H_


/// Virtual class representing a "layer" of the application
/// Layers handle their own data, and act as a stack
class AppLayer {
	/// Runs the tick update for the current layer. Returns true if "lower" layers should also tick
	virtual bool tick(VoxelGame &game);
	/// Renders the layer
	virtual void render(VoxelGame &game);
};


#endif

#include "layer/game_layer.h"
#include "voxel_game.h"

int main() {
	VoxelGame::init();
	
	GameLayer game_layer = GameLayer::New();
	VoxelGame::pushAppLayer(&game_layer);
	VoxelGame::run();
	VoxelGame::destroy();
	return 0;
}

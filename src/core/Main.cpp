#include "game/VoxelGame.h"

int main() {
	const auto game = new VoxelGame;
	game->run();
	delete game;
	return 0;
}

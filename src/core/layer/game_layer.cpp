#include "layer/game_layer.h"

GameLayer::GameLayer()
{
	this->registry.RegisterBlock("air", Block::AllSides(0));
	this->registry.RegisterBlock("stone", Block::AllSides(1));
}

void GameLayer::render(float deltaTime) 
{}

bool GameLayer::tick() 
{return true;}

void GameLayer::resize(int width, int height) 
{}

#include "GameTime.h"

GameTime::GameTime() 
{
	currentTotalTime = 0;
	gameTime = 0;
}

GameTime::~GameTime() {}

void GameTime::Update(DWORD dt)
{
	//Cu 1000ms thi +gametime len 1 lan
	if (currentTotalTime + dt < 1000)	
	{
		currentTotalTime += dt;
	}
	else
	{
		currentTotalTime = (currentTotalTime + dt) % 1000;
		gameTime++;
	}
}
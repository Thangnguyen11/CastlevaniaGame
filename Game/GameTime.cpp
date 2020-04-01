#include "GameTime.h"

GameTime::GameTime() 
{
	accumulationTime = 0;
	gameTime = 0;
}

GameTime::~GameTime() {}

void GameTime::Update(DWORD dt)
{
	//Cu 1000ms thi +gametime len 1 lan
	if (accumulationTime + dt < 1000)	
	{
		accumulationTime += dt;
	}
	else
	{
		accumulationTime = (accumulationTime + dt) % 1000;
		gameTime++;
	}
}
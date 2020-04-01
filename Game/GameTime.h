#pragma once
#include "Game.h"

class GameTime
{
	DWORD accumulationTime;	//Thoi gian da tich luy duoc (+ dt)
	int gameTime;
public:
	GameTime();
	~GameTime();

	void Update(DWORD dt);

	void SetTime(int time) { gameTime = time; }
	int GetTime() { return gameTime; }
};


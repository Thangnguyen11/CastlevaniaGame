#pragma once
#include "Game.h"

class GameTime
{
	static GameTime* __instance;
	DWORD currentTotalTime;	//Thoi gian da tich luy duoc (+ dt)
	int gameTime;

	GameTime();
	~GameTime();
public:
	static GameTime* GetInstance();

	void Update(DWORD dt);

	void SetTime(int time) { gameTime = time; }
	int GetTime() { return gameTime; }
	void ResetGameTime() { gameTime = 0; }
};


#pragma once
#include "Weapon.h"

#define DAGGER_SPEED_X		0.4f

class Dagger : public Weapon
{
	float ownerPosX;
public:
	Dagger();
	~Dagger();

	void Update(DWORD dt, vector<LPGAMEENTITY> *coObjects = NULL);
	void Render();

	void Attack(float posX, float posY, int direction);

	bool CheckIsOutCamera(float posX);
};


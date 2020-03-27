#pragma once
#include "Weapon.h"

#define MORNINGSTAR_ATTACKING_DELAY		130 //equal to PLAYER_ATTACKING_DELAY

#define MORNINGSTAR_ANI_BEGIN			0
#define MORNINGSTAR_ANI_END				3

class MorningStar : public Weapon
{
public:
	MorningStar();
	~MorningStar();

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY> *coObjects = NULL);
	void Render();

	void Attack(float posX, float posY, int direction);
	void ArticulatedPlayerPos(bool isSitting = false);

	bool IsCollidingObject(Entity* Obj);
};


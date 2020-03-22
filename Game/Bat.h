#pragma once
#include "Entity.h"
#include <math.h>

#define BAT_FLYING_SPEED		0.15f

#define BAT_STATE_FLYING		100
#define BAT_STATE_DIE			200

#define BAT_ANI_FLYING_BEGIN	1
#define BAT_ANI_FLYING_END		3

#define BAT_FLAPPING_SPEED		130 

class Bat : public Entity
{
	bool isDead;
public:
	Bat();
	~Bat();
	//	a fake update for testing
	virtual void Update(DWORD dt, vector<LPGAMEENTITY> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);

	//	some AI func
	float GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target);
	D3DXVECTOR2 RadialMovement(D3DXVECTOR2 focus, D3DXVECTOR2 pos, float speed);
};


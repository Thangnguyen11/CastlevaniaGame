#pragma once
#include "Entity.h"

#define TORCH_BBOX_WIDTH		32
#define TORCH_BBOX_HEIGHT		64

#define TORCH_ANI_BEGIN			0
#define TORCH_ANI_END			1

#define TORCH_BURN_SPEED		100

class Torch : public Entity
{
public:
	Torch(float posX, float posY);
	~Torch();

	void Update(DWORD dt, vector<LPGAMEENTITY> *coObjects = 0);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};


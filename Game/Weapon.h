#pragma once
#include "Entity.h"

class Weapon : public Entity
{
protected:
	bool isDone;
public:
	Weapon();
	~Weapon();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();

	virtual void Attack(float posX, float posY, int direction);
	virtual void ArticulatedPlayerPos() = 0;	//Fix pos to fit player base on each weapon

	bool GetIsDone() { return isDone; }
	void SetIsDone(bool isdone) { isDone = isdone; }
};


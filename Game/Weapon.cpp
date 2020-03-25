#include "Weapon.h"

Weapon::Weapon() 
{
	isDone = true;
}

Weapon::~Weapon(){}

void Weapon::Attack(float posX, float posY, int direction)
{
	this->posX = posX;
	this->posY = posY;
	this->direction = direction;
	isDone = false;
}

void Weapon::Render() 
{
	if (isDone)
		return;
	if (direction == 1) {
		sprite->DrawFlipVertical(posX, posY, 255);
	}
	else {
		sprite->Draw(posX, posY, 255);
	}
	
	//RenderBoundingBox();
}

void Weapon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = posX;
	top = posY;
	right = posX + texture->getFrameWidth();
	bottom = posY + texture->getFrameHeight();
}

#include "Dagger.h"

Dagger::Dagger()
{
	texture = Texture2dManager::GetInstance()->GetTexture(EntityType::DAGGER);
	sprite = new Sprite(texture, MaxFrameRate);
	tag = EntityType::DAGGER;
	ownerPosX = 0;
}

Dagger::~Dagger(){}

void Dagger::Update(DWORD dt, vector<LPGAMEENTITY> *coObjects)
{
	if (CheckIsOutCamera(ownerPosX))
	{
		isDone = true;
		return;
	}
	Weapon::Update(dt);
	posX += dx;
}

void Dagger::Attack(float posX, float posY, int direction)
{
	Weapon::Attack(posX, posY, direction);
	ownerPosX = posX;
	this->posY -= 8;	//Fit Simon Hand
	vX = DAGGER_SPEED_X * this->direction;
}

void Dagger::Render()
{
	if (isDone)
		return;
	if (direction == -1) //Right direction
	{
		sprite->DrawFlipVertical(posX, posY);
	}
	else 
	{
		sprite->Draw(posX, posY);
	}

	RenderBoundingBox();
	
}

bool Dagger::CheckIsOutCamera(float posX)
{
	if (vX > 0)	//Walking Right
	{
		if (this->posX - posX >= (SCREEN_WIDTH / 2))
			return true;
	}
	else if (vX < 0)
	{
		if (posX - this->posX >= (SCREEN_WIDTH / 2))
			return true;
	}
	return false;
}

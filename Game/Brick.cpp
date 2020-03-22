#include "Brick.h"

Brick::Brick() 
{
	this->texture = Texture2dManager::GetInstance()->GetTexture(EntityType::BRICK);
	this->sprite = new Sprite(texture, MaxFrameRate);
	tag = EntityType::BRICK;
}

Brick::~Brick() {}

void Brick::Update(DWORD dt, vector<LPGAMEENTITY> *coObjects)
{
	sprite->SelectFrame(BRICK_NORMAL);
}

void Brick::Render()
{
	sprite->Draw(posX, posY);
}

void Brick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
}

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
	if (brickType == 2)
		sprite->SelectFrame(BRICK_TYPE2);
	else
		sprite->SelectFrame(BRICK_TYPE1);
}

void Brick::Render()
{
	sprite->Draw(posX, posY);
	RenderBoundingBox();
}

void Brick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = posX;
	t = posY;
	r = posX + BRICK_BBOX_WIDTH;
	b = posY + BRICK_BBOX_HEIGHT;
}

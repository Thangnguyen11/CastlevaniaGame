#include "Brick.h"

Brick::Brick() 
{
	this->texture = new Texture2d("Resources/Ground.png", 4, 2, 8);
	this->sprite = new Sprite(texture, 100);
	tag = EntityType::BRICK;
}

Brick::~Brick() {}

void Brick::Update(float dt) 
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

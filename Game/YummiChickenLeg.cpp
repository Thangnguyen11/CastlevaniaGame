#include "YummiChickenLeg.h"

YummiChickenLeg::YummiChickenLeg(float posX, float posY)
{
	this->texture = Texture2dManager::GetInstance()->GetTexture(EntityType::YUMMICHICKENLEG);
	this->sprite = new Sprite(texture, MaxFrameRate);
	tag = EntityType::YUMMICHICKENLEG;

	this->posX = posX;
	this->posY = posY;

	vY = CHICKEN_GRAVITY;
	timeDisplayed = 0;
	timeDisplayMax = CHICKEN_TIMEDISPLAYMAX;
}

YummiChickenLeg::~YummiChickenLeg() {}
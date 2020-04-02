#include "Fire.h"

Fire::Fire(float posX, float posY)
{
	this->texture = Texture2dManager::GetInstance()->GetTexture(EntityType::FIREEFFECT);
	this->sprite = new Sprite(texture, MaxFrameRate);
	this->posX = posX;
	this->posY = posY;
	isDone = false;
}

Fire::~Fire() {}

void Fire::Update(DWORD dt)
{
	if (isDone)	//Khong bo vao Effect::Update do khong return duoc Update o day
	{
		return;
	}

	int currentFrame = sprite->GetCurrentFrame();
	if (currentFrame < FIRE_ANI_BEGIN) {
		sprite->SelectFrame(FIRE_ANI_BEGIN);
		sprite->currentTotalTime = dt;
	}
	else {
		sprite->currentTotalTime += dt;
		if (sprite->currentTotalTime >= FIRE_TIME_OF_PER_EFFECT) {
			sprite->currentTotalTime -= FIRE_TIME_OF_PER_EFFECT;
			sprite->SelectFrame(sprite->GetCurrentFrame() + 1);
		}

		if (sprite->GetCurrentFrame() > FIRE_ANI_END) {
			isDone = true;
		}
	}
}

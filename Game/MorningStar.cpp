#include "MorningStar.h"

MorningStar::MorningStar()
{
	texture = Texture2dManager::GetInstance()->GetTexture(EntityType::MORNINGSTAR);
	sprite = new Sprite(texture, MORNINGSTAR_ATTACKING_DELAY);
	tag = EntityType::MORNINGSTAR;
	level = 1;
}

MorningStar::~MorningStar(){}

void MorningStar::Update(DWORD dt, vector<LPGAMEENTITY> *coObjects)
{
	Weapon::Update(dt);

	if (level == 1 && sprite->GetCurrentFrame() == MORNINGSTAR_LV1_ANI_END)
		isDone = true;
	else if (level == 2 && sprite->GetCurrentFrame() == MORNINGSTAR_LV2_ANI_END)
		isDone = true;
	else if (level == 3 && sprite->GetCurrentFrame() == MORNINGSTAR_LV3_ANI_END)
		isDone = true;


	int currentFrame = sprite->GetCurrentFrame();
	if (level == 1) 
	{
		if (currentFrame >= MORNINGSTAR_LV1_ANI_BEGIN && currentFrame < MORNINGSTAR_LV1_ANI_END)
			sprite->Update(dt);
		else
			sprite->SelectFrame(MORNINGSTAR_LV1_ANI_BEGIN);
	}
	else if (level == 2) 
	{
		if (currentFrame >= MORNINGSTAR_LV2_ANI_BEGIN && currentFrame < MORNINGSTAR_LV2_ANI_END)
			sprite->Update(dt);
		else
			sprite->SelectFrame(MORNINGSTAR_LV2_ANI_BEGIN);
	}
	else if (level == 3) 
	{
		if (currentFrame >= MORNINGSTAR_LV3_ANI_BEGIN && currentFrame < MORNINGSTAR_LV3_ANI_END)
			sprite->Update(dt);
		else
			sprite->SelectFrame(MORNINGSTAR_LV3_ANI_BEGIN);
	}
}

void MorningStar::Render()
{
	Weapon::Render();
}

void MorningStar::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	//Weapon::GetBoundingBox(left, top, right, bottom);
	if (level == 1 || level == 2)
	{
		left = posX + 10;
		top = posY;
		right = posX + texture->getFrameWidth() - 30;
		bottom = posY + texture->getFrameHeight();
	}
	else if (level == 3)
	{
		left = posX - 18;
		top = posY;
		right = posX + texture->getFrameWidth() + 5;
		bottom = posY + texture->getFrameHeight();
	}
}

void MorningStar::Attack(float posX, float posY, int direction)
{
	Weapon::Attack(posX, posY, direction);
	if (level == 1)
		sprite->SelectFrame(MORNINGSTAR_LV1_ANI_BEGIN);
	else if (level == 2)
		sprite->SelectFrame(MORNINGSTAR_LV2_ANI_BEGIN);
	else if (level == 3)
		sprite->SelectFrame(MORNINGSTAR_LV3_ANI_BEGIN);
}

void MorningStar::ArticulatedPlayerPos(bool isSitting)
{
	/*if (direction == 1)
		posX += 25;
	else
		posX -= 25;*/

	posX += 25 * direction;

	if (isSitting)
		posY += 12;
}

bool MorningStar::IsCollidingObject(Entity* Obj)
{
	//Khong tinh 3 frame dau
	if (level == 1) 
	{
		if (sprite->GetCurrentFrame() <= MORNINGSTAR_LV1_ANI_BEGIN + 2)
			return false;
	}
	else if (level == 2)
	{
		if (sprite->GetCurrentFrame() <= MORNINGSTAR_LV2_ANI_BEGIN + 2)
			return false;
	}
	else if (level == 3)
	{
		if (sprite->GetCurrentFrame() <= MORNINGSTAR_LV3_ANI_BEGIN + 2)
			return false;
	}

	return Weapon::IsCollidingObject(Obj);
}

void MorningStar::UpLevel()
{
	if (level == 3)
		return;

	level++;
}
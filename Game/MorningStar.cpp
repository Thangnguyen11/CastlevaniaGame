#include "MorningStar.h"

MorningStar::MorningStar()
{
	texture = Texture2dManager::GetInstance()->GetTexture(EntityType::MORNINGSTAR);
	sprite = new Sprite(texture, MORNINGSTAR_ATTACKING_DELAY);
	tag = EntityType::MORNINGSTAR;
}

MorningStar::~MorningStar(){}

void MorningStar::Update(DWORD dt, vector<LPGAMEENTITY> *coObjects)
{
	Weapon::Update(dt);

	if (sprite->GetCurrentFrame() == MORNINGSTAR_ANI_END)
		isDone = true;

	int currentFrame = sprite->GetCurrentFrame();
	if (currentFrame >= MORNINGSTAR_ANI_BEGIN && currentFrame < MORNINGSTAR_ANI_END)
		sprite->Update(dt);
	else
		sprite->SelectFrame(MORNINGSTAR_ANI_BEGIN);
}

void MorningStar::Render()
{
	Weapon::Render();
}

void MorningStar::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	Weapon::GetBoundingBox(left, top, right, bottom);
}

void MorningStar::Attack(float posX, float posY, int direction)
{
	Weapon::Attack(posX, posY, direction);
	sprite->SelectFrame(MORNINGSTAR_ANI_BEGIN);
}

void MorningStar::ArticulatedPlayerPos()
{
	/*if (direction == 1)
		posX += 25;
	else
		posX -= 25;*/

	posX += 25 * direction;
}

bool MorningStar::IsCollidingObject(Entity* Obj)
{
	//Khong tinh 2 frame dau
	if (sprite->GetCurrentFrame() <= MORNINGSTAR_ANI_BEGIN + 1)
		return false;

	return Weapon::IsCollidingObject(Obj);
}
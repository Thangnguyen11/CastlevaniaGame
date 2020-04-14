#include "Knight.h"

Knight::Knight(float posX, float posY, int directionX)
{
	this->texture = Texture2dManager::GetInstance()->GetTexture(EntityType::KNIGHT);
	this->sprite = new Sprite(texture, MaxFrameRate);
	tag = EntityType::KNIGHT;

	this->posX = posX;
	this->posY = posY;
	this->direction = directionX;
	startX = posX + 1;

	SetState(KNIGHT_STATE_WALKING);

	health = 2;
	isDead = false;
}

Knight::~Knight(){}

void Knight::Update(DWORD dt, vector<LPGAMEENTITY> *coObjects)
{
	if (health <= 0)
	{
		SetState(KNIGHT_STATE_DIE);
		return;
	}

	int currentFrame = sprite->GetCurrentFrame();
	if (!isDead) {
		if (currentFrame < KNIGHT_ANI_WALKING_BEGIN)
		{
			sprite->SelectFrame(KNIGHT_ANI_WALKING_BEGIN);
			sprite->currentTotalTime = dt;
		}
		else {
			sprite->currentTotalTime += dt;
			if (sprite->currentTotalTime >= KNIGHT_SWAPLEG_SPEED)
			{
				sprite->currentTotalTime -= KNIGHT_SWAPLEG_SPEED;
				sprite->SelectFrame(currentFrame + 1);
			}

			if (sprite->GetCurrentFrame() > KNIGHT_ANI_WALKING_END) {
				sprite->SelectFrame(KNIGHT_ANI_WALKING_BEGIN);
			}
		}
	}
	Entity::Update(dt);
	vY += KNIGHT_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> bricks;
	//Knight only collision with bricks
	coEvents.clear();
	bricks.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == EntityType::BRICK)
			bricks.push_back(coObjects->at(i));

	// turn off collision when die 
	if (state != KNIGHT_STATE_DIE)
		CalcPotentialCollisions(&bricks, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		posX += dx;
		posY += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		posX += min_tx * dx + nx * 0.1f;
		posY += min_ty * dy + ny * 0.1f;

		if (nx != 0 && ny == 0)
		{
			direction *= -1;
			vX *= -1;
		}
		else 
			if (ny == -1)
			vY = 0;
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

	if (posX >= startX)
	{
		direction *= -1;
		vX *= -1;
	}
}

void Knight::Render()
{
	if (isDead)
		return;

	if (direction == 1) {
		sprite->DrawFlipVertical(posX, posY, 255);
	}
	else {
		sprite->Draw(posX, posY, 255);
	}
	RenderBoundingBox();
}

void Knight::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case KNIGHT_STATE_DIE:
		vX = 0;
		vY = 0;
		health = 0;
		isDead = true;
		break;
	case KNIGHT_STATE_WALKING:
		vX = KNIGHT_WALKING_SPEED * direction;
		break;
	}
}

void Knight::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	//not clean
	if (!isDead) 
	{
		l = posX - 15;
		t = posY;
		r = posX + KNIGHT_BBOX_WIDTH;
		b = posY + KNIGHT_BBOX_HEIGHT;
	}
}

#include "Bat.h"

Bat::Bat() 
{
	this->texture = Texture2dManager::GetInstance()->GetTexture(EntityType::BAT);
	this->sprite = new Sprite(texture, MaxFrameRate);
	tag = EntityType::BAT;

	direction = 1;

	this->SetState(BAT_STATE_FLYING);

	isDead = false;
}

Bat::~Bat(){}

void Bat::Update(float dt, Entity *target) 
{
	int currentFrame = sprite->GetCurrentFrame();
	if (!isDead) {
		if (currentFrame < BAT_ANI_FLYING_BEGIN) {
			sprite->SelectFrame(BAT_ANI_FLYING_BEGIN);
			sprite->currentTotalTime = dt;
		}
		else {
			sprite->currentTotalTime += dt;
			if (sprite->currentTotalTime >= BAT_FLAPPING_SPEED) {
				sprite->currentTotalTime -= BAT_FLAPPING_SPEED;
				sprite->SelectFrame(sprite->GetCurrentFrame() + 1);			
			}

			if (sprite->GetCurrentFrame() > BAT_ANI_FLYING_END) {
				sprite->SelectFrame(BAT_ANI_FLYING_BEGIN);
			}
		}
	}

	//AI (testing - not neccesary)
	D3DXVECTOR2 pos = D3DXVECTOR2(posX, posY);
	//speed nay dc define cho vX (pos += vX * dt) nen khi muon dung cho pos phai nhan voi 1 so tuong duong dt
	pos += RadialMovement(D3DXVECTOR2(target->GetPosX(), target->GetPosY()), pos, BAT_FLYING_SPEED*10);	
	posX = pos.x;
	posY = pos.y;

	Entity::Update(dt);


	if (vX < 0 && posX < 0) {
		posX = 0; vX = -vX;
	}

	if (vX > 0 && posX > SCREEN_WIDTH) {
		posX = SCREEN_WIDTH; vX = -vX;
	}
}

void Bat::Render() 
{
	if (isDead)
		return;

	if (direction == 1) {
		sprite->DrawFlipVertical(posX, posY, 255);
	}
	else {
		sprite->Draw(posX, posY, 255);
	}
}

void Bat::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case BAT_STATE_DIE:
		vX = 0;
		vY = 0;
		isDead = true;
		break;
	case BAT_STATE_FLYING:
		//vX = -BAT_FLYING_SPEED;
		break;
	}
}

void Bat::GetBoundingBox(float &l, float &t, float &r, float &b) 
{

}

float Bat::GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target)
{
	return (float)sqrt(pow(pos.x - target.x, 2) + pow(pos.y - target.y, 2));
}

D3DXVECTOR2 Bat::RadialMovement(D3DXVECTOR2 focus, D3DXVECTOR2 pos, float speed)
{
	float dist = GetDistance(pos, focus);
	if (dist < speed)
		return focus - pos;
	else
		return (focus - pos) * speed / dist;
}
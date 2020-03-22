#include "Player.h"

Player::Player() 
{
	this->texture = Texture2dManager::GetInstance()->GetTexture(EntityType::PLAYER);
	this->sprite = new Sprite(texture, MaxFrameRate);
	tag = EntityType::PLAYER;
	direction = 1;

	this->SetState(PLAYER_STATE_IDLE);

	isWalking = false;
	isJumping = false;
	isAttacking = false;
}

Player::~Player(){}

void Player::Update(DWORD dt, vector<LPGAMEENTITY> *coObjects)
{	
	//Not clean
	if (posY == 375)
		isJumping = false;
	else
		isJumping = true;

	if (!isWalking && !isJumping)	//Attack tren mat dat thi dung yen, attack khi dang jump thi di chuyen duoc
	{
		vX = 0;
		//explain: state_walking: jump false already -> state_attack: walking false too -> vX = 0
		//		  state_jumping: jump true -> state_attack: jump false but posY != 375 -> jump true -> vX != 0
	}

	int currentFrame = sprite->GetCurrentFrame();
	if (isSitting) {
		if (isAttacking) {
			if (currentFrame < PLAYER_ANI_SITTING_ATTACK_BEGIN) {
				sprite->SelectFrame(PLAYER_ANI_SITTING_ATTACK_BEGIN);
				sprite->currentTotalTime = dt;
			}
			else {
				sprite->currentTotalTime += dt;
				if (sprite->currentTotalTime >= PLAYER_ATTACKING_DELAY) {
					sprite->currentTotalTime -= PLAYER_ATTACKING_DELAY;
					sprite->SelectFrame(sprite->GetCurrentFrame() + 1);			//Wait 100 and trans to next frame
				}

				if (sprite->GetCurrentFrame() > PLAYER_ANI_SITTING_ATTACK_END) {
					sprite->SelectFrame(PLAYER_ANI_IDLE);
					isAttacking = false;
				}
			}
		}
		else {
			sprite->SelectFrame(PLAYER_ANI_SITTING);
		}
	}
	else if (isAttacking && !isSitting) {
		if (currentFrame < PLAYER_ANI_ATTACK_BEGIN) {
			sprite->SelectFrame(PLAYER_ANI_ATTACK_BEGIN);
			sprite->currentTotalTime = dt;
		}
		else {
			sprite->currentTotalTime += dt;
			if (sprite->currentTotalTime >= PLAYER_ATTACKING_DELAY) {
				sprite->currentTotalTime -= PLAYER_ATTACKING_DELAY;
				sprite->SelectFrame(sprite->GetCurrentFrame() + 1);			//Wait 100ms and trans to next frame
			}

			if (sprite->GetCurrentFrame() > PLAYER_ANI_ATTACK_END) {
				sprite->SelectFrame(PLAYER_ANI_IDLE);
				isAttacking = false;
			}
		}
	}
	else 
		if (isWalking) {
			if (!isJumping) {
				if (currentFrame < PLAYER_ANI_WALKING_BEGIN || currentFrame >= PLAYER_ANI_WALKING_END)
					sprite->SelectFrame(PLAYER_ANI_WALKING_BEGIN);

				sprite->Update(dt);
			}
			else {
				sprite->SelectFrame(PLAYER_ANI_JUMPING);
			}
		}
		else
			if (isJumping) {		//Nhay tai cho
				sprite->SelectFrame(PLAYER_ANI_JUMPING);
			}
			else {
				sprite->SelectFrame(PLAYER_ANI_IDLE);
			}


	Entity::Update(dt);
	posX += dx;
	posY += dy;
	// simple fall down
	vY += PLAYER_GRAVITY;
	if (posY > 375)
	{
		vY = 0; posY = 375;
	}



	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();

	//// turn off collision when die 
	//if (state != PLAYER_STATE_DIE)
	//	CalcPotentialCollisions(coObjects, coEvents);


	//// No collision occured, proceed normally
	//if (coEvents.size() == 0)
	//{
	//	posX += dx;
	//	posY += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;

	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	//	// block 
	//	posX += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
	//	posY += min_ty * dy + ny * 0.4f;

	//	if (nx != 0) vX = 0;
	//	if (ny != 0) vY = 0;

	//}



	//// clean up collision events
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void Player::Render()
{
	if (direction == 1) {
		sprite->DrawFlipVertical(posX, posY, 255);
	}
	else {
		sprite->Draw(posX, posY, 255);
	}

	RenderBoundingBox();
}

void Player::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case PLAYER_STATE_WALKING_RIGHT:
		isWalking = true;
		isSitting = false;
		vX = PLAYER_WALKING_SPEED;
		direction = 1;
		break;
	case PLAYER_STATE_WALKING_LEFT:
		isWalking = true;
		isSitting = false;
		vX = -PLAYER_WALKING_SPEED;
		direction = -1;
		break;
	case PLAYER_STATE_JUMP:
		isJumping = true;
		isSitting = false;
		if (posY == 375)
			vY = -PLAYER_JUMP_SPEED_Y;
		break;
	case PLAYER_STATE_IDLE:
		//vX = 0;
		isWalking = false;
		isSitting = false;
		break;
	case PLAYER_STATE_ATTACK:
		//vX = 0;		//While attacking cant walking until the attack is done
		isAttacking = true;
		isWalking = false;
		isJumping = false;
		break;
	case PLAYER_STATE_SITTING:
		//vX = 0;		//Vi phim return khi dang ngoi nen set vX = 0 de khi moving -> sit se khong bi truot
		isSitting = true;
		isWalking = false;
		break;
	}
}

void Player::GetBoundingBox(float &left, float &top, float &right, float &bottom) 
{
	left = posX;
	top = posY;
	right = posX + PLAYER_BBOX_WIDTH;
	bottom = posY + PLAYER_BBOX_HEIGHT;
}
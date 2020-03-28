#include "Player.h"
#include "Bat.h"
#include "Zombie.h"

Player::Player() 
{
	this->texture = Texture2dManager::GetInstance()->GetTexture(EntityType::PLAYER);
	this->sprite = new Sprite(texture, MaxFrameRate);
	tag = EntityType::PLAYER;
	direction = 1;

	this->SetState(PLAYER_STATE_IDLE);

	health = PLAYER_MAXHEALTH;
	isDead = false;
	isWalking = false;
	isJumping = false;
	isAllowJump = true;
	isAttacking = false;

	currentWeapon = new MorningStar();
}

Player::~Player(){}

void Player::Update(DWORD dt, vector<LPGAMEENTITY> *coObjects)
{	
	//Not clean
	/*if (posY == 375)
		isJumping = false;
	else
		isJumping = true;*/

	if (!isWalking && !isJumping)	//Attack tren mat dat thi dung yen, attack khi dang jump thi di chuyen duoc
	{
		vX = 0;
		//old explain: state_walking: jump false already, transfer to state_attack: walking false too -> vX = 0
		//		  state_jumping: jump true, transfer to state_attack: jump false but posY != 375 -> jump true -> vX != 0
		//update explain: state_walking: jump false (collision with brick), transfer to state_attack: walking false too -> vX = 0
		//				state_jumping: jump true (until collision with brick) -> vX != 0
	}

	int currentFrame = sprite->GetCurrentFrame();
	if (isDead)
		sprite->SelectFrame(PLAYER_ANI_DIE);
	else 
		if (isSitting) {
			if (isAttacking) {
				if (currentFrame != PLAYER_ANI_SITTING_ATTACK_BEGIN && currentFrame != PLAYER_ANI_SITTING_ATTACK_BEGIN + 1 && currentFrame != PLAYER_ANI_SITTING_ATTACK_BEGIN + 2) {
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
						sprite->SelectFrame(PLAYER_ANI_SITTING);
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

	// simple fall down
	vY += PLAYER_GRAVITY * dt;

	if (posX <= 15)
		posX = 15;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	//if (state != PLAYER_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

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
		posX += min_tx * dx + nx * 0.1f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		posY += min_ty * dy + ny * 0.1f;

		if (ny == -1)
		{
			vY = 0.1f;
			dy = vY * dt;

			if (isJumping)
			{
				isJumping = false;
				isAllowJump = true;
			}
		}

		if (nx != 0) vX = 0;
		if (ny != 0) vY = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->GetType() == EntityType::BAT) 
			{
				Bat *bat = dynamic_cast<Bat *>(e->obj);

				//No jump on head
				/*if (e->ny < 0)
				{
					if (bat->GetState() != BAT_STATE_DIE)
					{
						bat->SetState(BAT_STATE_DIE);
						vY = -PLAYER_DEFLECT_SPEED_Y;
					}
				}
				else */
					if (e->nx != 0 || e->ny != 0)
				{
					if (bat->GetState() != BAT_STATE_DIE)
					{
						AddHealth(-2);
						vX = -PLAYER_DEFLECT_SPEED_X;
						vY = -PLAYER_DEFLECT_SPEED_Y;
						bat->AddHealth(-1);
					}
				}
			}
			if (e->obj->GetType() == EntityType::ZOMBIE)
			{
				Zombie *zombie = dynamic_cast<Zombie *>(e->obj);
				if (zombie->GetState() != ZOMBIE_STATE_DIE) 
				{
					if (e->nx != 0 || e->ny != 0)
					{
						vX = -PLAYER_DEFLECT_SPEED_X;
						vY = -PLAYER_DEFLECT_SPEED_Y;
						AddHealth(-2);
						//zombie->SetState(ZOMBIE_STATE_DIE);
					}
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (!currentWeapon->GetIsDone())
	{
		currentWeapon->SetPosition(posX, posY);	//Update pos per player::update
		currentWeapon->SetSpeed(vX, vY);		//Collision
		currentWeapon->ArticulatedPlayerPos(isSitting);	//Fixing weapon pos
		currentWeapon->Update(dt, coObjects);
	}
}

void Player::Render()
{
	if (direction == 1) {
		sprite->DrawFlipVertical(posX, posY, 255);
	}
	else {
		sprite->Draw(posX, posY, 255);
	}

	if (!currentWeapon->GetIsDone())
	{
		currentWeapon->Render();
	}

	RenderBoundingBox();
}

void Player::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case PLAYER_STATE_DIE:
		isDead = true;
		vX = 0;
		vY = 0;
		break;
	case PLAYER_STATE_WALKING_RIGHT:
		direction = 1;
		isWalking = true;
		//isSitting = false;
		vX = PLAYER_WALKING_SPEED * direction;
		break;
	case PLAYER_STATE_WALKING_LEFT:
		direction = -1;
		isWalking = true;
		//isSitting = false;
		vX = PLAYER_WALKING_SPEED * direction;
		break;
	case PLAYER_STATE_JUMP:
		if (!isAllowJump)
			return;
		isJumping = true;
		isAllowJump = false;
		//isSitting = false;
		vY = -PLAYER_JUMP_SPEED_Y;
		break;
	case PLAYER_STATE_IDLE:
		vX = 0;
		isWalking = false;
		isSitting = false;
		break;
	case PLAYER_STATE_ATTACK:
		Attack(EntityType::MORNINGSTAR);
		//vX = 0;		//While attacking cant walking until the attack is done
		isWalking = false;
		//isJumping = false;
		//testing
		break;
	case PLAYER_STATE_SITTING:
		vX = 0;		//Vi phim return khi dang ngoi nen set vX = 0 de khi moving -> sit se khong bi truot
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

void Player::Attack(EntityType weaponType)
{
	if (isAttacking)
		return;
	
	if (currentWeapon->GetIsDone())
	{
		isAttacking = true;
		currentWeapon->Attack(posX, posY, direction);
	}
}
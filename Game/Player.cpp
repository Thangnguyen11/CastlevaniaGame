#include "Player.h"
#include "Bat.h"
#include "Zombie.h"

Player::Player(float posX, float posY) 
{
	this->texture = Texture2dManager::GetInstance()->GetTexture(EntityType::PLAYER);
	this->sprite = new Sprite(texture, MaxFrameRate);
	tag = EntityType::PLAYER;
	this->posX = posX;
	this->posY = posY;
	direction = 1;

	this->SetState(PLAYER_STATE_IDLE);

	health = PLAYER_MAXHEALTH;
	mana = 5;
	score = 0;
	live = 3;
	isDead = false;
	isWalking = false;
	isJumping = false;
	isAllowJump = true;
	isAttacking = false;
	isSitting = false;
	isHurting = false;
	isImmortaling = false;
	isPassingStage = false;
	isRespawning = false;

	mainWeapon = new MorningStar();		//Simon's main/basic weapon is MorningStar
	supWeapon = NULL;
	currentSupWeaponType = EntityType::NONE;		//	non ((:
}

Player::~Player(){}

void Player::Update(DWORD dt, vector<LPGAMEENTITY> *coObjects)
{	
	if (health <= 0 && !isRespawning)
	{
		respawningTimer->Start();
		isRespawning = true;
		immortalTimer->Start();
		isImmortaling = true;
		live -= 1;
		if (live <= 0)
		{
			isDead = true;
		}
	}

	if (!isWalking && !isJumping)	//Attack tren mat dat thi dung yen, attack khi dang jump thi di chuyen duoc
	{
		vX = 0;
		//old explain: state_walking: jump false already, transfer to state_attack: walking false too -> vX = 0
		//		  state_jumping: jump true, transfer to state_attack: jump false but posY != 375 -> jump true -> vX != 0
		//update explain: state_walking: jump false (collision with brick), transfer to state_attack: walking false too -> vX = 0
		//				state_jumping: jump true (until collision with brick) -> vX != 0
	}

#pragma region Timer
	if (isHurting && hurtingTimer->IsTimeUp()) 
	{
		SetState(PLAYER_STATE_IDLE);
		isHurting = false;
		hurtingTimer->Reset();
	}
	if (isImmortaling && immortalTimer->IsTimeUp()) 
	{
		isImmortaling = false;
		immortalTimer->Reset();
	}
	if (isUpgrading && upgradeTimer->IsTimeUp()) 
	{
		SetState(PLAYER_STATE_IDLE);
		isUpgrading = false;
		upgradeTimer->Reset();
	}
	if (isRespawning && respawningTimer->IsTimeUp())
	{
		Respawn();
		isRespawning = false;
		respawningTimer->Reset();
	}
#pragma endregion

#pragma region Update Sprite
	int currentFrame = sprite->GetCurrentFrame();
	if (isDead || isRespawning)
	{
		sprite->SelectFrame(PLAYER_ANI_DIE);
	}
	else
		if (isUpgrading)
		{
			if (currentFrame < PLAYER_ANI_UPGRADING_BEGIN) //Can be bug here
			{
				sprite->SelectFrame(PLAYER_ANI_UPGRADING_BEGIN);
				sprite->currentTotalTime = dt;
			}
			else 
			{
				sprite->currentTotalTime += dt;
				if (sprite->currentTotalTime >= PLAYER_UPGRADING_DELAY) 
				{
					sprite->currentTotalTime -= PLAYER_UPGRADING_DELAY;
					sprite->SelectFrame(sprite->GetCurrentFrame() + 1);			//Wait 100ms and trans to next frame
				}

				if (sprite->GetCurrentFrame() > PLAYER_ANI_UPGRADING_END) 
				{
					sprite->SelectFrame(PLAYER_ANI_IDLE);
				}
			}
		}
	else 
		if (isHurting)
		{
			sprite->SelectFrame(PLAYER_ANI_HURTING);
		}
		else
			if (isSitting) 
			{
				if (isAttacking) 
				{
					if (currentFrame != PLAYER_ANI_SITTING_ATTACK_BEGIN && currentFrame != PLAYER_ANI_SITTING_ATTACK_BEGIN + 1 && currentFrame != PLAYER_ANI_SITTING_ATTACK_BEGIN + 2) 
					{
						sprite->SelectFrame(PLAYER_ANI_SITTING_ATTACK_BEGIN);
						sprite->currentTotalTime = dt;
					}
					else 
					{
						sprite->currentTotalTime += dt;
						if (sprite->currentTotalTime >= PLAYER_ATTACKING_DELAY) 
						{
							sprite->currentTotalTime -= PLAYER_ATTACKING_DELAY;
							sprite->SelectFrame(sprite->GetCurrentFrame() + 1);			//Wait 100 and trans to next frame
						}

						if (sprite->GetCurrentFrame() > PLAYER_ANI_SITTING_ATTACK_END) 
						{
							sprite->SelectFrame(PLAYER_ANI_SITTING);
							isAttacking = false;
						}
					}
				}
				else 
				{
					sprite->SelectFrame(PLAYER_ANI_SITTING);
				}
			}
			else if (isAttacking && !isSitting) 
			{
				if (currentFrame < PLAYER_ANI_ATTACK_BEGIN) 
				{
					sprite->SelectFrame(PLAYER_ANI_ATTACK_BEGIN);
					sprite->currentTotalTime = dt;
				}
				else 
				{
					sprite->currentTotalTime += dt;
					if (sprite->currentTotalTime >= PLAYER_ATTACKING_DELAY) 
					{
						sprite->currentTotalTime -= PLAYER_ATTACKING_DELAY;
						sprite->SelectFrame(sprite->GetCurrentFrame() + 1);			//Wait 100ms and trans to next frame
					}

					if (sprite->GetCurrentFrame() > PLAYER_ANI_ATTACK_END) 
					{
						sprite->SelectFrame(PLAYER_ANI_IDLE);
						isAttacking = false;
					}
				}
			}
			else
				if (isWalking) 
				{
					if (!isJumping) 
					{
						if (currentFrame < PLAYER_ANI_WALKING_BEGIN || currentFrame >= PLAYER_ANI_WALKING_END)
							sprite->SelectFrame(PLAYER_ANI_WALKING_BEGIN);

						sprite->Update(dt);
					}
					else 
					{
						sprite->SelectFrame(PLAYER_ANI_JUMPING);
					}
				}
				else
					if (isJumping) 
					{		//Nhay tai cho
						sprite->SelectFrame(PLAYER_ANI_JUMPING);
					}
					else 
					{
						sprite->SelectFrame(PLAYER_ANI_IDLE);
					}
#pragma endregion

	Entity::Update(dt);

	// simple fall down
	vY += PLAYER_GRAVITY * dt;

	if (posX <= 15)	//Not go out
		posX = 15;

#pragma region Collide Logic
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> listObjMayCollide;			//Khong xet va cham voi torch
	coEvents.clear();
	listObjMayCollide.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() != EntityType::TORCH)
			listObjMayCollide.push_back(coObjects->at(i));

	// turn off collision when die 
	//if (state != PLAYER_STATE_DIE)
		CalcPotentialCollisions(&listObjMayCollide, coEvents);

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
				isHurting = false;
			}
		}

		if (nx != 0) vX = 0;
		if (ny != 0) vY = 0;
		if (nx != 0 && ny != 0)
			isHurting = false;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (!isImmortaling) {
				if (e->obj->GetType() == EntityType::BAT)
				{
					Bat *bat = dynamic_cast<Bat *>(e->obj);

					if (e->nx != 0 || e->ny != 0)
					{
						if (bat->GetState() != BAT_STATE_DIE)
						{
							this->AddHealth(-2);
							hurtingTimer->Start();
							immortalTimer->Start();
							isImmortaling = true;
							SetState(PLAYER_STATE_HURTING);
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
							this->AddHealth(-2);
							hurtingTimer->Start();
							immortalTimer->Start();
							isImmortaling = true;
							SetState(PLAYER_STATE_HURTING);
						}
					}
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion

	if (!mainWeapon->GetIsDone())
	{
		mainWeapon->SetPosition(posX, posY);				//Update pos per player::update
		//mainWeapon->SetSpeed(vX, vY);					//Collision
		mainWeapon->ArticulatedPlayerPos(isSitting);		//Fixing weapon pos
		mainWeapon->Update(dt, coObjects);
	}
	if (supWeapon != NULL)
	{
		if (!supWeapon->GetIsDone())
		{
			supWeapon->Update(dt, coObjects);
		}
	}
}

void Player::Render()
{
	int alpha = 255;
	if (isImmortaling)
		alpha = 150;
	if (direction == 1) {
		sprite->DrawFlipVertical(posX, posY, alpha);
	}
	else {
		sprite->Draw(posX, posY, alpha);
	}

	if (!mainWeapon->GetIsDone())
	{
		mainWeapon->Render();
	}
	if (supWeapon != NULL)
	{
		if (!supWeapon->GetIsDone())
		{
			supWeapon->Render();
		}
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
		isAttacking = false;
		isAllowJump = true;
		isPassingStage = false;		//set simon not blocking
		break;
	case PLAYER_STATE_SUPWEAPON_ATTACK:
		Attack(currentSupWeaponType);
		isWalking = false;
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
	case PLAYER_STATE_HURTING:
		if (isHurting == true)
			return;
		isHurting = true;
		isWalking = false;
		isJumping = false;
		isAttacking = false;
		if (isSitting)	isSitting = false;
		vX = PLAYER_DEFLECT_SPEED_X * direction;
		vY = -PLAYER_DEFLECT_SPEED_Y;
		break;
	case PLAYER_STATE_UPGRADING:
		isHurting = false;
		isWalking = false;
		isJumping = false;
		isAttacking = false;
		if (isSitting)	isSitting = false;
		vX = 0;
		vY = 0;
		break;
	case PLAYER_STATE_PASSING_STAGE:
		direction = 1;
		isWalking = true;
		//isSitting = false;
		vX = PLAYER_PASSING_STAGE_SPEED * direction; 
		posX += dx;	//Khi trong state nay tuc la da va cham voi gate, o ngoai update cua simon khi co va cham kh update posX
		isPassingStage = true;	//block control simon
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
	if (isAttacking || isUpgrading)	//Co 1 bug khi dang danh ma rot item thi van danh tiep
		return;
	
	switch (weaponType)
	{
	case EntityType::MORNINGSTAR:
	{
		if (mainWeapon->GetIsDone())
		{
			isAttacking = true;
			mainWeapon->Attack(posX, posY, direction);
		}
		break; 
	}
	case EntityType::DAGGER:
	{
		if (mana > 0)
		{
			if (supWeapon->GetIsDone())
			{
				AddMana(-1);
				isAttacking = true;
				supWeapon->Attack(posX, posY, direction);
			}
		}
	}
	default:
		break;
	}
	
}

void Player::UpgradingMorningStar()
{
	MorningStar* morningStar = dynamic_cast<MorningStar*>(mainWeapon);
	if (morningStar->GetLevel() >= 3)
		return;

	upgradeTimer->Start();
	isUpgrading = true;
	SetState(PLAYER_STATE_UPGRADING);
}

void Player::SetPlayerSupWeaponType(EntityType supWeaponType)
{
	switch (supWeaponType)
	{
	case EntityType::DAGGER:
		currentSupWeaponType = EntityType::DAGGER;
		supWeapon = new Dagger();
		break;
	default:
		break;
	}
}

void Player::Respawn()
{
	posX = 100;
	posY = 280;
	vX = 0;
	isJumping = false;
	isHurting = false;
	isImmortaling = false;
	SetState(PLAYER_STATE_IDLE);
	health = PLAYER_MAXHEALTH;
	immortalTimer->Start();
	isImmortaling = true;
}
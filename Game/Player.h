#pragma once

#include "Entity.h"
#include "MorningStar.h"
#include <map>

#define PLAYER_WALKING_SPEED				0.20f	//0.25
#define PLAYER_JUMP_SPEED_Y					0.5f	//0.8
#define PLAYER_GRAVITY						0.002f	//0.025
#define PLAYER_DEFLECT_SPEED_X				0.8f
#define PLAYER_DEFLECT_SPEED_Y				0.3f
#define PLAYER_MAXHEALTH					16

#define PLAYER_BBOX_WIDTH					45		//60
#define PLAYER_BBOX_HEIGHT					63		//66

#define PLAYER_STATE_DIE					-1
#define PLAYER_STATE_IDLE					0
#define PLAYER_STATE_WALKING_RIGHT			100
#define PLAYER_STATE_WALKING_LEFT			200
#define PLAYER_STATE_JUMP					300
#define PLAYER_STATE_ATTACK					400
#define PLAYER_STATE_SITTING				500

#define PLAYER_ANI_DIE						28
#define PLAYER_ANI_IDLE						0
#define PLAYER_ANI_WALKING_BEGIN			1
#define PLAYER_ANI_WALKING_END				3
#define PLAYER_ANI_JUMPING					4
#define PLAYER_ANI_ATTACK_BEGIN				5
#define PLAYER_ANI_ATTACK_END				7
#define PLAYER_ANI_SITTING					29
#define PLAYER_ANI_SITTING_ATTACK_BEGIN		15
#define PLAYER_ANI_SITTING_ATTACK_END		17

#define PLAYER_ATTACKING_DELAY		110

class Player : public Entity
{
	
public:
	bool isWalking,
		isJumping,
		isAllowJump,
		isAttacking,
		isSitting;

	//testing
	//std::unordered_map<EntityType, Weapon*> weapons;
	Weapon* currentWeapon;

	Player();
	~Player();

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY> *colliable_objects = NULL);
	void Render();
	void SetState(int state);

	void Attack(EntityType weaponType);	//Ve sau xet weapon type
};


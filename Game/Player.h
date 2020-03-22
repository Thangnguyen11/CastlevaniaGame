#pragma once

#include "Entity.h"

#define PLAYER_WALKING_SPEED				0.25f
#define PLAYER_JUMP_SPEED_Y					1.f
#define PLAYER_GRAVITY						0.05f

#define PLAYER_BBOX_WIDTH					60
#define PLAYER_BBOX_HEIGHT					66

#define PLAYER_STATE_DIE					-1
#define PLAYER_STATE_IDLE					0
#define PLAYER_STATE_WALKING_RIGHT			100
#define PLAYER_STATE_WALKING_LEFT			200
#define PLAYER_STATE_JUMP					300
#define PLAYER_STATE_ATTACK					400
#define PLAYER_STATE_SITTING				500

#define PLAYER_ANI_IDLE						0
#define PLAYER_ANI_WALKING_BEGIN			1
#define PLAYER_ANI_WALKING_END				3
#define PLAYER_ANI_JUMPING					4
#define PLAYER_ANI_ATTACK_BEGIN				5
#define PLAYER_ANI_ATTACK_END				7
#define PLAYER_ANI_SITTING					4
#define PLAYER_ANI_SITTING_ATTACK_BEGIN		15
#define PLAYER_ANI_SITTING_ATTACK_END		17

#define PLAYER_ATTACKING_DELAY		150

class Player : public Entity
{
	
public:
	bool isWalking,
		isJumping,
		isAttacking,
		isSitting;


	Player();
	~Player();

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY> *colliable_objects = NULL);
	void Render();
	void SetState(int state);
};


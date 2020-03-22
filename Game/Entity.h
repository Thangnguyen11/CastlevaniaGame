#pragma once
#include <vector>

#include "debug.h"
#include "define.h"
#include "Sprite.h"
#include "Texture2dManager.h"

using namespace std;

class Entity;
typedef Entity * LPGAMEENTITY;


struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEENTITY obj;
	float t, nx, ny;
	CCollisionEvent(float t, float nx, float ny, LPGAMEENTITY obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class Entity
{
protected:
	float posX, posY;
	float vX, vY;
	float dx, dy;	// v * dt
	DWORD dt;
	int direction;	//-1 left && 1 right
	EntityType tag;
	int health;
	Texture2d* texture;
	Sprite* sprite;

	int state;

public:
	Entity();
	~Entity();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEENTITY> *coObjects = NULL);
	virtual void Render() = 0;

	void RenderBoundingBox();

	void SetState(int STATE) { this->state = STATE; }
	int GetState() { return state; }

	int GetHealth() { return health; }
	void AddHealth(int BonusHealth) { health += BonusHealth; }
	void SetDirection(int d) { direction = d; }
	int GetDirection() { return direction; }

	void SetPosition(float X, float Y) { posX = X; posY = Y; }
	float GetPosX() { return posX; }
	float GetPosY() { return posY; }
	void ReceivePos(float &x, float &y) { x = this->posX; y = this->posY; }
	void SetPosX(float X) { posX = X; }
	void SetPosY(float Y) { posY = Y; }
	void AddPos(float X, float Y) { this->SetPosition(this->GetPosX() + X, this->GetPosY() + Y); }

	//void GetSpeed(float &vx, float &vy);
	void SetSpeed(float VX, float VY) { vX = VX; vY = VY; }
	float GetVx() { return vX; }
	float GetVy() { return vY; }
	void ReceiveSpeed(float &vx, float &vy) { vx = this->vX; vy = this->vY; }
	void SetVx(float VX) { vX = VX; }
	void SetVy(float VY) { vY = VY; }
	void AddVx(float VX) { this->SetVx(this->GetVx() + VX); }
	void AddVy(float VY) { this->SetVy(this->GetVy() + VY); }

	void SetTexture(Texture2d * Texture);
	Sprite* GetSprite();
	int GetWidth() { return this->texture->getFrameWidth(); }
	int GetHeight() { return this->texture->getFrameHeight(); }

	EntityType GetType() { return tag; }


	LPCOLLISIONEVENT SweptAABBEx(LPGAMEENTITY coO);
	void CalcPotentialCollisions(vector<LPGAMEENTITY> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents,
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny);

};


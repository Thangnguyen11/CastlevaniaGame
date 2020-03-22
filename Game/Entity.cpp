#include "Entity.h"

Entity::Entity()
{
	posX = posY = 0;
	vX = vY = 0;
	direction = 1;

	health = 1;
}

Entity::~Entity() {
	//SAFE_DELETE(sprite);
}

void Entity::Update(DWORD dt, vector<LPGAMEENTITY> *coObjects)
{
	this->dt = dt;
	dx = vX * dt;
	dy = vY * dt;
}

void Entity::SetTexture(Texture2d * Texture)
{
	texture = Texture;
	sprite->texture = Texture;
}

Sprite * Entity::GetSprite()
{
	return sprite;
}

void Entity::RenderBoundingBox() 
{
	Texture2d* bbox = Texture2dManager::GetInstance()->GetTexture(EntityType::BBOX);
	D3DXVECTOR3 origin((float)texture->getFrameWidth() / 2, (float)texture->getFrameHeight() / 2, 0);

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	Game::GetInstance()->Draw(posX, posY, bbox->Texture, rect.left, rect.top, rect.right, rect.bottom, origin, 20);
}


/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT Entity::SweptAABBEx(LPGAMEENTITY coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->ReceiveSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	Game::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent * e = new CCollisionEvent(t, nx, ny, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void Entity::CalcPotentialCollisions(
	vector<LPGAMEENTITY> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void Entity::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty,
	float &nx, float &ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


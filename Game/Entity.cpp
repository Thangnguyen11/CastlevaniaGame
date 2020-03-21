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

void Entity::Update(float dt)
{
	//velocity = pixel / s
	posX += vX * dt;
	posY += vY * dt;
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

	Game::GetInstance()->Draw(posX, posY, bbox->Texture, rect.left, rect.top, rect.right, rect.bottom, origin, 200);
}
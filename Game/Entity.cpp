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

RECT Entity::GetBound() {
	RECT bound;

	bound.left = posX - this->texture->getFrameWidth() / 2;
	bound.right = posX + this->texture->getFrameWidth() / 2;
	bound.top = posY - this->texture->getFrameHeight() / 2;
	bound.bottom = posY + this->texture->getFrameHeight() / 2;

	return bound;
}

void Entity::Update(float dt)
{
	//velocity = pixel / s
	posX += vX * dt;
	posY += vY * dt;
}

void Entity::Render()
{
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

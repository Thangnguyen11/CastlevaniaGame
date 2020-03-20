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

}
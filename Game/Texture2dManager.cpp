#include "Texture2dManager.h"

Texture2dManager* Texture2dManager::__instance = NULL;

Texture2dManager* Texture2dManager::GetInstance() 
{
	if (__instance == NULL) __instance = new Texture2dManager();
	return __instance;
}

Texture2dManager::Texture2dManager()
{
	LoadContent();
}

Texture2dManager::~Texture2dManager(){}

Texture2d* Texture2dManager::GetTexture(EntityType tag)
{
	return Textures[tag];
}

void Texture2dManager::AddTexture(EntityType tag, Texture2d* texture)
{
	Textures[tag] = texture;
}

void Texture2dManager::LoadContent()
{
	AddTexture(EntityType::PLAYER, new Texture2d("Resources/Simon.png", 8, 7, 56));
	AddTexture(EntityType::BRICK, new Texture2d("Resources/Ground.png", 2, 1, 2));
	AddTexture(EntityType::BAT, new Texture2d("Resources/VampireBat.png", 4, 1, 4));
	AddTexture(EntityType::BBOX, new Texture2d("Resources/bbox.png", 1, 1, 1));
}
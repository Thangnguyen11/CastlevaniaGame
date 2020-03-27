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
	//BBox
	AddTexture(EntityType::BBOX, new Texture2d("Resources/bbox.png", 1, 1, 1));

	//Player
	AddTexture(EntityType::PLAYER, new Texture2d("Resources/Simon.png", 8, 7, 56, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Static Obj
	AddTexture(EntityType::BRICK, new Texture2d("Resources/Ground.png", 2, 1, 2));

	//Mobs and Bosses
	AddTexture(EntityType::BAT, new Texture2d("Resources/VampireBat.png", 4, 1, 4, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::ZOMBIE, new Texture2d("Resources/Zombie.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Weapons
	AddTexture(EntityType::MORNINGSTAR, new Texture2d("Resources/MorningStar.png", 4, 3, 12, D3DCOLOR_ARGB(255, 255, 0, 255)));
}
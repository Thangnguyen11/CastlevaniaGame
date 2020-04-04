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
	AddTexture(EntityType::TORCH, new Texture2d("Resources/Torch.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::CANDLE, new Texture2d("Resources/Candle.png", 4, 1, 4, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//UI
	AddTexture(EntityType::BLACKBOARD, new Texture2d("Resources/BlackBoard.png", 1, 1, 1));
	AddTexture(EntityType::PLAYERHP, new Texture2d("Resources/HP.png", 3, 1, 3));
	AddTexture(EntityType::TEXTS, new Texture2d("Resources/Texts.png", 13, 3, 36, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Effective
	AddTexture(EntityType::FIREEFFECT, new Texture2d("Resources/Fire.png", 4, 1, 4, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::HITEFFECT, new Texture2d("Resources/Hit.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::BUBBLESEFFECT, new Texture2d("Resources/Bubbles.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::BRICKEXPLODEEFFECT, new Texture2d("Resources/BrickExplode.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::ADDSCOREEFFECT, new Texture2d("Resources/Score.png", 8, 1, 8, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Mobs and Bosses
	AddTexture(EntityType::BAT, new Texture2d("Resources/VampireBat.png", 4, 1, 4, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::ZOMBIE, new Texture2d("Resources/Zombie.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Weapons
	AddTexture(EntityType::MORNINGSTAR, new Texture2d("Resources/MorningStar.png", 4, 3, 12, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::DAGGER, new Texture2d("Resources/Dagger.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Items
	AddTexture(EntityType::SMALLHEART, new Texture2d("Resources/SmallHeart.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::BIGHEART, new Texture2d("Resources/BigHeart.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::YUMMICHICKENLEG, new Texture2d("Resources/YummiChickenLeg.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::MONEYBAGS, new Texture2d("Resources/MoneyBags.png", 3, 1, 3, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::UPGRADEMORNINGSTAR, new Texture2d("Resources/UpgradeMS.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::CRYSTALBALL, new Texture2d("Resources/CrystalBall.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::INVIPOTION, new Texture2d("Resources/InviPotion.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::DAGGER, new Texture2d("Resources/Dagger.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::STOPWATCH, new Texture2d("Resources/StopWatch.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::CROSS, new Texture2d("Resources/Cross.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::AXE, new Texture2d("Resources/Axe.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::BOOMERANG, new Texture2d("Resources/Boomerang.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::WATERPOTION, new Texture2d("Resources/WaterPotion.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::ITEMDAGGER, new Texture2d("Resources/Dagger.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));

}
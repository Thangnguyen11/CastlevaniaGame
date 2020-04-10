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

	//Scene
	AddTexture(EntityType::TITLESCENE, new Texture2d("Resources/Scene/TitleScene.png"));
	AddTexture(EntityType::TITLEBAT, new Texture2d("Resources/Scene/TitleBat.png", 5, 3, 15));
	AddTexture(EntityType::STARTTEXT, new Texture2d("Resources/Scene/StartText.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::INTROSCENE, new Texture2d("Resources/Scene/IntroScene.png"));
	AddTexture(EntityType::INTROSIMON, new Texture2d("Resources/Simon.png", 8, 7, 56, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Map
	AddTexture(EntityType::MAPSTAGE1, new Texture2d("Resources/TileMap/TilesetStage1.png", 8, 4, 32, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::MAPSTAGE2, new Texture2d("Resources/TileMap/TilesetStage2.png", 22, 4, 88, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Static Obj
	AddTexture(EntityType::BRICK, new Texture2d("Resources/Objects/Ground.png", 2, 1, 2));
	AddTexture(EntityType::TORCH, new Texture2d("Resources/Objects/Torch.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::CANDLE, new Texture2d("Resources/Objects/Candle.png", 4, 1, 4, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::GATE, new Texture2d("Resources/Objects/Gate.png", 4, 1, 4, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::STAIRS, new Texture2d("Resources/Objects/Stair.png", 2, 1, 2));

	//UI
	AddTexture(EntityType::BLACKBOARD, new Texture2d("Resources/UI/BlackBoard.png", 1, 1, 1));
	AddTexture(EntityType::PLAYERHP, new Texture2d("Resources/UI/HP.png", 3, 1, 3));
	AddTexture(EntityType::TEXTS, new Texture2d("Resources/UI/Texts.png", 13, 3, 36, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Effective
	AddTexture(EntityType::FIREEFFECT, new Texture2d("Resources/Effects/Fire.png", 4, 1, 4, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::HITEFFECT, new Texture2d("Resources/Effects/Hit.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::BUBBLESEFFECT, new Texture2d("Resources/Effects/Bubbles.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::BRICKEXPLODEEFFECT, new Texture2d("Resources/Effects/BrickExplode.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::ADDSCOREEFFECT, new Texture2d("Resources/Effects/Score.png", 8, 1, 8, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Mobs and Bosses
	AddTexture(EntityType::BAT, new Texture2d("Resources/Enemies/VampireBat.png", 4, 1, 4, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::ZOMBIE, new Texture2d("Resources/Enemies/Zombie.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Weapons
	AddTexture(EntityType::MORNINGSTAR, new Texture2d("Resources/Weapons/MorningStar.png", 4, 3, 12, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::DAGGER, new Texture2d("Resources/Weapons/Dagger.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));

	//Items
	AddTexture(EntityType::SMALLHEART, new Texture2d("Resources/Items/SmallHeart.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::BIGHEART, new Texture2d("Resources/Items/BigHeart.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::YUMMICHICKENLEG, new Texture2d("Resources/Items/YummiChickenLeg.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::MONEYBAGS, new Texture2d("Resources/Items/MoneyBags.png", 3, 1, 3, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::UPGRADEMORNINGSTAR, new Texture2d("Resources/Items/UpgradeMS.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::CRYSTALBALL, new Texture2d("Resources/Items/CrystalBall.png", 2, 1, 2, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::INVIPOTION, new Texture2d("Resources/Items/InviPotion.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::STOPWATCH, new Texture2d("Resources/Items/StopWatch.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::CROSS, new Texture2d("Resources/Items/Cross.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::AXE, new Texture2d("Resources/Items/Axe.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::BOOMERANG, new Texture2d("Resources/Items/Boomerang.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::WATERPOTION, new Texture2d("Resources/Items/WaterPotion.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));
	AddTexture(EntityType::ITEMDAGGER, new Texture2d("Resources/Items/Dagger.png", 1, 1, 1, D3DCOLOR_ARGB(255, 255, 0, 255)));

}
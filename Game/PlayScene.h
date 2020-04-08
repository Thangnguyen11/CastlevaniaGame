#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "Scene.h"
#include "Game.h"
#include "GameTime.h"
#include "Camera.h"
#include "Timer.h"
#include "UI.h"
#include "Map.h"
#include "SmallHeart.h"
#include "BigHeart.h"
#include "MoneyBags.h"
#include "YummiChickenLeg.h"
#include "UpgradeMorningStar.h"
#include "ItemDagger.h"
#include "Hit.h"
#include "Fire.h"
#include "Score.h"

#include "Player.h"
#include "Brick.h"
#include "Bat.h"
#include "Zombie.h"
#include "Torch.h"
#include "Gate.h"

#define SPAWNING_ZOMBIE_DELAY				3000
#define SPAWNING_DELAY_BETWEEN_2_ZOMBIE		360	
#define SPAWNING_BAT_DELAY					3000

#define HIT_EFFECT_CUSTOMIZED_POS			8

class PlayScene : public Scene
{
protected:
	Player* player;
	std::vector<LPGAMEENTITY> listObjects;
	std::vector<LPGAMEEFFECT> listEffects;
	std::vector<LPGAMEITEM> listItems;
	UI* gameUI;
	GameTime* gameTime;		
	Camera* camera;
	Map* map;

	int idStage;

	int counterZombie;
	bool isTimeToSpawnZombie;
	bool triggerSpawnZombie;
	Timer* spawningZombieTimer = new Timer(SPAWNING_ZOMBIE_DELAY);
	Timer* delaySpawningZombieTimer = new Timer(SPAWNING_DELAY_BETWEEN_2_ZOMBIE);
	bool isTimeToSpawnBat;
	bool triggerSpawnBat;
	Timer* spawningBatTimer = new Timer(SPAWNING_BAT_DELAY);
	//testing Scan
	//bool isScanned;
	//Timer* scanningGameTimer = new Timer(SCANING_GAME_DELAY);
	void _ParseSection_OBJECTS(string line);

public:
	PlayScene();

	void LoadBaseObjects();
	void ChooseMap(int whatStage);	//Co the thay bien int thanh EntityType
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	Effect* CreateEffect(EntityType createrType, EntityType effectType, float posX, float posY);
	Item* DropItem(EntityType createrType, float posX, float posY);
	void WeaponInteractObj(UINT i);
	void WeaponCollision();
	void PlayerCollideItem();
	void CheckObjAlive();
	void SetSubWeaponDone(UINT i);
	void PlayerGotGate();

	friend class PlayScenceKeyHandler;
};


class PlayScenceKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlayScenceKeyHandler(Scene *s) :ScenceKeyHandler(s) {};
};



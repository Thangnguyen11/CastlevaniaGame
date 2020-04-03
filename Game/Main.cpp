#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "debug.h"
#include "define.h"
#include "Game.h"
#include "GameTime.h"
#include "Timer.h"
#include "UI.h"
#include "SmallHeart.h"
#include "BigHeart.h"
#include "MoneyBags.h"
#include "YummiChickenLeg.h"
#include "UpgradeMorningStar.h"
#include "Hit.h"
#include "Fire.h"
#include "Score.h"

#include "Player.h"
#include "Brick.h"
#include "Bat.h"
#include "Zombie.h"
#include "Torch.h"

#define SPAWNING_ZOMBIE_DELAY				3000
#define SPAWNING_DELAY_BETWEEN_2_ZOMBIE		360	
#define SPAWNING_BAT_DELAY					3000

#define HIT_EFFECT_CUSTOMIZED_POS			8

Game* game;
Player* player;
std::vector<LPGAMEENTITY> objects;
std::vector<LPGAMEEFFECT> effects;
std::vector<LPGAMEITEM> items;
UI* gameUI;
GameTime* gameTime;
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

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HWND InitWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WIN_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WIN_NAME,
			WIN_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

void LoadContent()
{
	//testing Scan
	//isScanned = false;
	//scanningGameTimer->Start();
#pragma region Creat Player
	player = new Player(100, SCREEN_HEIGHT - 150);
	objects.push_back(player);
#pragma endregion
#pragma region Creat Ground
	for (int i = 0; i < 50; i++)
		objects.push_back(new Brick(i * 32.0f, SCREEN_HEIGHT - 55));
	
	for (int i = 0; i < 3; i++)
	{
		objects.push_back(new Brick(1000 + i * 200, SCREEN_HEIGHT - 110 - i * 50, 2));
		objects.push_back(new Brick(1032 + i * 200, SCREEN_HEIGHT - 110 - i * 50, 2));
		objects.push_back(new Brick(1064 + i * 200, SCREEN_HEIGHT - 110 - i * 50, 2));
	}

	for (int i = 0; i < 5; i++)
	{
		objects.push_back(new Torch(200 + i * 250, SCREEN_HEIGHT - 105));
	}
#pragma endregion
	gameTime = new GameTime();
	gameUI = new UI(player->GetHealth(), 16);

	counterZombie = 0;
	isTimeToSpawnZombie = true;		//vua vao spawn luon
	triggerSpawnZombie = false;

	isTimeToSpawnBat = true;
	triggerSpawnBat = true;

}

Effect* CreateEffect(EntityType createrType, EntityType effectType, float posX, float posY)
{
	if (effectType == EntityType::HITEFFECT)
		return new Hit(posX - HIT_EFFECT_CUSTOMIZED_POS, posY - HIT_EFFECT_CUSTOMIZED_POS);
	else if (effectType == EntityType::FIREEFFECT)
		return new Fire(posX, posY);
	else if (effectType == EntityType::ADDSCOREEFFECT)
		return new Score(posX, posY, createrType);
	else
		return new Hit(posX - HIT_EFFECT_CUSTOMIZED_POS, posY - HIT_EFFECT_CUSTOMIZED_POS);
}

Item* DropItem(EntityType createrType, float posX, float posY)
{
	int bagrandom = rand() % 100;
	if (createrType == EntityType::ZOMBIE || createrType == EntityType::BAT || createrType == EntityType::TORCH)
	{
		int random = rand() % 1000;
		if (random <= 200)
			return new SmallHeart(posX, posY);
		else if (200 < random && random <= 400)
			return new BigHeart(posX, posY);
		else if (400 < random && random <= 600)
			return new YummiChickenLeg(posX, posY);
		else if (600 < random && random <= 800)
			return new UpgradeMorningStar(posX, posY);
		else
		{
			if (bagrandom <= 33) 
				return new MoneyBags(posX, posY, EntityType::MONEYBAGRED);
			else if (33 < bagrandom && bagrandom <= 66)
				return new MoneyBags(posX, posY, EntityType::MONEYBAGWHITE);
			else
				return new MoneyBags(posX, posY, EntityType::MONEYBAGBLUE);
		}
	}
	else
		return new SmallHeart(posX, posY);
}

void WeaponCollision()
{
	if (!player->GetPlayerCurrentWeapon()->GetIsDone())
	{
		for (UINT i = 0; i < objects.size(); i++)
		{
			if (player->GetPlayerCurrentWeapon()->IsCollidingObject(objects[i]))	//weapon va cham voi obj
			{
				switch (objects[i]->GetType())
				{
				case EntityType::BAT:
					objects[i]->AddHealth(-1);
					player->AddScore(500);
					effects.push_back(CreateEffect(objects[i]->GetType(), EntityType::HITEFFECT, objects[i]->GetPosX(), objects[i]->GetPosY()));
					effects.push_back(CreateEffect(objects[i]->GetType(), EntityType::FIREEFFECT, objects[i]->GetPosX(), objects[i]->GetPosY()));
					items.push_back(DropItem(objects[i]->GetType(), objects[i]->GetPosX(), objects[i]->GetPosY()));
					break;
				case EntityType::ZOMBIE:
					objects[i]->AddHealth(-1);
					player->AddScore(100);
					effects.push_back(CreateEffect(objects[i]->GetType(), EntityType::HITEFFECT, objects[i]->GetPosX(), objects[i]->GetPosY()));
					effects.push_back(CreateEffect(objects[i]->GetType(), EntityType::FIREEFFECT, objects[i]->GetPosX(), objects[i]->GetPosY()));
					items.push_back(DropItem(objects[i]->GetType(), objects[i]->GetPosX(), objects[i]->GetPosY()));
					counterZombie--;
					if (counterZombie == 0)
					{
						spawningZombieTimer->Start();
						triggerSpawnZombie = true;
						isTimeToSpawnZombie = false;
					}
					break;
				case EntityType::TORCH:
					objects[i]->AddHealth(-1);
					effects.push_back(CreateEffect(objects[i]->GetType(), EntityType::HITEFFECT, objects[i]->GetPosX(), objects[i]->GetPosY()));
					effects.push_back(CreateEffect(objects[i]->GetType(), EntityType::FIREEFFECT, objects[i]->GetPosX(), objects[i]->GetPosY()));
					items.push_back(DropItem(objects[i]->GetType(), objects[i]->GetPosX(), objects[i]->GetPosY()));
				default:
					break;
				}
			}
		}
	}
}

void PlayerCollideItem()
{
	for (UINT i = 0; i < items.size(); i++)
	{
		if (!items[i]->GetIsDone())
		{
			if (player->IsCollidingObject(items[i]))
			{
				switch (items[i]->GetType())
				{
				case EntityType::MONEYBAGRED:
					//Vi cai nay ma phai dem CollideItem tu Player ra ngoai -.-
					//Ton biet bao nhieu suc
					player->AddScore(100);
					items[i]->SetIsDone(true);
					effects.push_back(CreateEffect(EntityType::MONEYBAGRED, EntityType::ADDSCOREEFFECT, items[i]->GetPosX(), items[i]->GetPosY() - PLAYER_BBOX_HEIGHT));
					break;
				case EntityType::MONEYBAGWHITE:
					player->AddScore(400);
					items[i]->SetIsDone(true);
					effects.push_back(CreateEffect(EntityType::MONEYBAGWHITE, EntityType::ADDSCOREEFFECT, items[i]->GetPosX(), items[i]->GetPosY() - PLAYER_BBOX_HEIGHT));
					break;
				case EntityType::MONEYBAGBLUE:
					player->AddScore(700);
					items[i]->SetIsDone(true);
					effects.push_back(CreateEffect(EntityType::MONEYBAGBLUE, EntityType::ADDSCOREEFFECT, items[i]->GetPosX(), items[i]->GetPosY() - PLAYER_BBOX_HEIGHT));
					break;
				case EntityType::SMALLHEART:
					player->AddMana(1);
					items[i]->SetIsDone(true);
					break;
				case EntityType::BIGHEART:
					player->AddMana(5);
					items[i]->SetIsDone(true);
					break;
				case EntityType::YUMMICHICKENLEG:
					player->AddScore(1000);
					items[i]->SetIsDone(true);
					break;
				case EntityType::UPGRADEMORNINGSTAR:
				{
					//nang cap ms
					//Dung yen simon 1 ty
					MorningStar* morningStarWeapon = dynamic_cast<MorningStar*>(player->GetPlayerCurrentWeapon());
					player->UpgradingMorningStar();
					morningStarWeapon->UpLevel();
					items[i]->SetIsDone(true);
					break;
				}
				default:
					break;
				}
			}
		}
	}
}

void Update(DWORD dt)
{
#pragma region Scan Game Periodically
	//Nen co 1 lan quet toan bo object cua game moi 100 hoac 1000 lan update
	/*if (!isScanned && scanningGameTimer->IsTimeUp())
	{
		isScanned = true;
		if (isScanned) 
		{
			ScanEntitiesPeriodically(objects);
			isScanned = false;
		}
		scanningGameTimer->Reset();
		scanningGameTimer->Start();
	}*/
#pragma endregion
#pragma region Checking alive object is in screen
	for (UINT i = 0; i < objects.size(); i++)
	{
		LPGAMEENTITY coO = objects[i];	
		if(coO->GetHealth() > 0)	//Xet object con song	//object health < 0 con xu li o trong zombie.cpp update
			switch (coO->GetType())
			{
			case EntityType::BAT:
				if (coO->GetPosX() < 0 || coO->GetPosX() > SCREEN_WIDTH * 2)
				{
					coO->SetState(BAT_STATE_DIE);
				}
				break;
			case EntityType::ZOMBIE:
				if (coO->GetPosY() > SCREEN_HEIGHT + 200)
				{
					coO->SetState(ZOMBIE_STATE_DIE);
					counterZombie--;
					if (counterZombie == 0)
					{
						spawningZombieTimer->Start();
						triggerSpawnZombie = true;
						isTimeToSpawnZombie = false;	//De khong vao if voi
					}
				}
				break;
			default:
				break;
			}
	}
#pragma endregion
#pragma region Spawning Zombie Logic
#pragma region Failed Operation Spawning Each Zombie
	////auto if = true neu counter chua len 3 do spawningTimer kh thay doi
	//if (isTimeToSpawnZombie) {
	//	if (counterZombie <= 3 && spawningZombieTimer->IsTimeUp())
	//	{
	//		if (delaySpawningZombieTimer->IsTimeUp())	//IsTimeUp nay vao duoc do co Start o dieu kien duoi
	//		{
	//			objects.push_back(new Zombie(SCREEN_WIDTH / 2 + 400, SCREEN_HEIGHT - 150, -1));
	//			counterZombie++;

	//			delaySpawningZombieTimer->Reset();		//Reset va start su dung nhu 1 vong lap (ap dung ham Update)
	//			delaySpawningZombieTimer->Start();
	//		}
	//		if (counterZombie == 3)	//counter len 3 thay doi spawningTimer de thoat khoi if thu nhat
	//		{
	//			spawningZombieTimer->Reset();
	//			counterZombie = 0;
	//			isTimeToSpawnZombie = false;
	//		}
	//	}
	//	if (triggerSpawnZombie)	//STAR: Dat Dieu kien Start sau counter++
	//	{
	//		spawningZombieTimer->Start();
	//		//start dau tien cua delay de co the vao IsTimeUp cua no
	//		delaySpawningZombieTimer->Start();
	//		triggerSpawnZombie = false;	//Bien trigger giup cho spawning zombie chi thuc hien khi dang co 0 hoac 3 zombie
	//	}
	//}
#pragma endregion
	if (isTimeToSpawnZombie)
	{
		if (delaySpawningZombieTimer->IsTimeUp()) 
		{
			if (counterZombie < 3)
			{
				objects.push_back(new Zombie(SCREEN_WIDTH / 2 + 400, SCREEN_HEIGHT - 150, -1));
				counterZombie++;
				if (counterZombie >= 3)
				{
					isTimeToSpawnZombie = false;	//out ra khoi if nay
					triggerSpawnZombie = false;		//out ra ca if sau
				}
				delaySpawningZombieTimer->Start();
			}
		}
	}
	else
	{
		if (triggerSpawnZombie)
		{
			if(spawningZombieTimer->IsTimeUp())
			{
				isTimeToSpawnZombie = true;
			}
		}
	}
#pragma endregion
#pragma region Spawning Bat Logic
	if (isTimeToSpawnBat) {
		if (spawningBatTimer->IsTimeUp())
		{
			objects.push_back(new Bat(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT - 150, 1));
			spawningBatTimer->Reset(SPAWNING_BAT_DELAY + (rand() % 3000));
			triggerSpawnBat = true;	//Dung 1 cho thi se spawn bat mai mai, cho den khi di den khu vuc ma trigger o do = false
		}
		if (triggerSpawnBat)
		{
			spawningBatTimer->Start();
			triggerSpawnBat = false;
		}
	}
#pragma endregion
#pragma region Objects Updates
	std::vector<LPGAMEENTITY> coObjects;
	for (int i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	for (int i = 0; i < effects.size(); i++)
	{
		effects[i]->Update(dt);
	}
	for (int i = 0; i < items.size(); i++)
	{
		items[i]->Update(dt, &objects);
	}
#pragma endregion
	WeaponCollision();
	PlayerCollideItem();
#pragma region Camera
	float cx, cy;
	player->ReceivePos(cx, cy);

	if (player->GetPosX() < SCREEN_WIDTH / 2)
		cx -= SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 2 - player->GetPosX());
	else
		cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;

	Game::GetInstance()->SetCamPos(cx, 0.0f);//cy khi muon camera move theo y player //castlevania chua can
#pragma endregion
	gameTime->Update(dt);
	gameUI->Update(cx + 260, 35, player->GetHealth(), 16);	//move posX follow camera
	//test
	if (player->GetPosX() > SCREEN_WIDTH + 200)
		isTimeToSpawnBat = false;
	else
		isTimeToSpawnBat = true;
}

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	
	if (d3ddv->BeginScene())
	{
		d3ddv->ColorFill(bb, NULL, BGColor);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();
		for (int i = 0; i < effects.size(); i++)
			effects[i]->Render();
		for (int i = 0; i < items.size(); i++)
			items[i]->Render();
		gameUI->Render(1, SCENEGAME_GAMETIMEMAX - gameTime->GetTime(), player);

		//End draw
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

int GameLoop()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MaxFrameRate;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			//DebugOut(L"dt = %d , tickPerFrame = %d \n", dt, tickPerFrame);

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}



class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_ESCAPE:
		DestroyWindow(Game::GetInstance()->GetWindowHandle());
	case DIK_R:
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->GetBBARGB() == 0)
				objects[i]->SetBBARGB(200);
			else
				objects[i]->SetBBARGB(0);
		}
		break;
	case DIK_C:
		if (player->IsDeadYet() || player->IsAttacking() || player->IsSitting() || player->IsHurting() || player->IsUpgrading())
			return;
		player->SetState(PLAYER_STATE_JUMP);
		break;
	case DIK_X:
		if (player->IsDeadYet() || player->IsHurting() || player->IsUpgrading())
			return;
		player->SetState(PLAYER_STATE_ATTACK);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	if (player->IsDeadYet() || player->IsAttacking() || player->IsJumping() || player->IsHurting() || player->IsUpgrading()) {	
		return;
	}

	if (game->IsKeyDown(DIK_DOWN)) {
		player->SetState(PLAYER_STATE_SITTING);
		if (game->IsKeyDown(DIK_RIGHT))
			player->SetDirection(1);
		else if (game->IsKeyDown(DIK_LEFT))
			player->SetDirection(-1);
		return;				//Important return //Dont change state while sitting
	}
	else player->SetState(PLAYER_STATE_IDLE);

	if (game->IsKeyDown(DIK_RIGHT))
		player->SetState(PLAYER_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		player->SetState(PLAYER_STATE_WALKING_LEFT);
	else player->SetState(PLAYER_STATE_IDLE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = InitWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = Game::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);

	LoadContent();
	GameLoop();

	return 0;
}
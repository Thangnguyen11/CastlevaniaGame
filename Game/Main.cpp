#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "debug.h"
#include "define.h"
#include "Game.h"
#include "Timer.h"
#include "HealthBar.h"

#include "Player.h"
#include "Brick.h"
#include "Bat.h"
#include "Zombie.h"

#define SPAWNING_ZOMBIE_DELAY			20

Game* game;
Player* player;
std::vector<LPGAMEENTITY> objects;
HealthBar* playerHB;
HealthBar* enemyHB;
int counterZombie;
Timer* spawningZombieTimer = new Timer(SPAWNING_ZOMBIE_DELAY);

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
#pragma region Creat Player
	player = new Player(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 150);
	objects.push_back(player);

	playerHB = new HealthBar(player->GetHealth(), true);
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
#pragma endregion
	objects.push_back(new Bat(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT - 150));

	counterZombie = 0;
	//objects.push_back(new Zombie(SCREEN_WIDTH / 2 + 400, SCREEN_HEIGHT - 150, -1));
	//objects.push_back(new Zombie(SCREEN_WIDTH / 2 + 450, SCREEN_HEIGHT - 150, -1));
	//objects.push_back(new Zombie(SCREEN_WIDTH / 2 + 500, SCREEN_HEIGHT - 150, -1));

	enemyHB = new HealthBar(16, false);
}

void WeaponCollision(vector<LPGAMEENTITY> coObjects)
{
	if (!player->GetPlayerCurrentWeapon()->GetIsDone())
	{
		for (UINT i = 0; i < coObjects.size(); i++)
			if (player->GetPlayerCurrentWeapon()->IsCollidingObject(coObjects[i]))	//weapon va cham voi obj
			{
				LPGAMEENTITY coO = coObjects[i];
				switch (coO->GetType())
				{
				case EntityType::BAT:
					coO->AddHealth(-1);
					break;
				case EntityType::ZOMBIE:
					coO->AddHealth(-1);
					counterZombie -= 1;
					break;
				default:
					break;
				}
			}
	}
}

void Update(DWORD dt)
{
	//Bugging
	if (counterZombie <= 3 && spawningZombieTimer->IsTimeUp())
	{
		objects.push_back(new Zombie(SCREEN_WIDTH / 2 + 400, SCREEN_HEIGHT - 150, -1));
		counterZombie++; 
		if (counterZombie == 3)
		{
			spawningZombieTimer->Reset();
			counterZombie = 0;
		}
	}
	if (counterZombie == 0)	//Dat Dieu kien Start sau counter++
	{
		spawningZombieTimer->Start();
	}
#pragma region Object Updates
	std::vector<LPGAMEENTITY> coObjects;
	for (int i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
#pragma endregion
	WeaponCollision(objects);
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
	playerHB->Update(player->GetHealth(), cx + 175, 80);	//move posX follow camera
	enemyHB->Update(16, playerHB->GetPosX(), playerHB->GetPosY() + 20);
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
		playerHB->Render();
		enemyHB->Render();

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
		if (player->IsAttacking() || player->IsSitting() || player->IsHurting())
			return;
		player->SetState(PLAYER_STATE_JUMP);
		break;
	case DIK_X:
		if (player->IsHurting())
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
	if (player->IsDeadYet() || player->IsAttacking() || player->IsJumping() || player->IsHurting()) {	
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
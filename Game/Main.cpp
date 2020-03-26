#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "debug.h"
#include "define.h"
#include "Game.h"

#include "Player.h"
#include "Brick.h"
#include "Bat.h"

Game* game;
Player* player;
std::vector<LPGAMEENTITY> objects;

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
	player = new Player();
	player->SetPosition(SCREEN_WIDTH / 2, 600);
	objects.push_back(player);

	for (int i = 0; i < 50; i++)
	{
		Brick *brick = new Brick();
		brick->SetPosition(i * 32.0f, 695);
		objects.push_back(brick);
	}
	for (int i = 0; i < 40; i++)
	{
		Brick *brick = new Brick();
		brick->brickType = 2;
		brick->SetPosition(0.0f, 695 - 32 * i);
		objects.push_back(brick);
	}
	for (int i = 0; i < 4; i++)
	{
		Brick *brick = new Brick();
		brick->brickType = 2;
		brick->SetPosition(200 + i * 275, 550 - i * 150);
		objects.push_back(brick);

		brick = new Brick();
		brick->brickType = 2;
		brick->SetPosition(232 + i * 275, 550 - i * 150);
		objects.push_back(brick);

		brick = new Brick();
		brick->brickType = 2;
		brick->SetPosition(264 + i * 275, 550 - i * 150);
		objects.push_back(brick);
	}

	Bat *bat = new Bat(SCREEN_WIDTH / 2 + 300, 600);
	objects.push_back(bat);
}

void WeaponCollision(vector<LPGAMEENTITY> coObjects)
{
	if (!player->currentWeapon->GetIsDone())
	{
		for (UINT i = 0; i < coObjects.size(); i++)
			if (player->currentWeapon->IsCollidingObject(coObjects[i]))	//weapon va cham voi obj
			{
				LPGAMEENTITY coO = coObjects[i];
				switch (coO->GetType())
				{
				case EntityType::BAT:
					coO->AddHealth(-1);
					break;
				default:
					break;
				}
			}
	}
}

void Update(DWORD dt)
{
	std::vector<LPGAMEENTITY> coObjects;
	for (int i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	WeaponCollision(objects);

	float cx, cy;
	player->ReceivePos(cx, cy);

	cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;

	Game::GetInstance()->SetCamPos(cx, 0.0f);
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
	case DIK_C:
		if (player->isAttacking || player->isSitting)
			return;
		player->SetState(PLAYER_STATE_JUMP);
		break;
	case DIK_X:
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
	if (player->IsDeadYet() || player->isAttacking || player->isJumping) {	
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
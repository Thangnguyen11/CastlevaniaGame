#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "Game.h"

Game* game;

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

void Update(DWORD dt)
{
	Game::GetInstance()->GetCurrentScene()->Update(dt);
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

		Game::GetInstance()->GetCurrentScene()->Render();

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

//class CSampleKeyHander : public KeyboardHandler
//{
//	virtual void KeyState(BYTE *states);
//	virtual void OnKeyDown(int KeyCode);
//	virtual void OnKeyUp(int KeyCode);
//};
//
//CSampleKeyHander * keyHandler;
//
//void CSampleKeyHander::OnKeyDown(int KeyCode)
//{
//	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
//	switch (KeyCode)
//	{
//	case DIK_ESCAPE:
//		DestroyWindow(Game::GetInstance()->GetWindowHandle());
//	case DIK_R:
//		for (int i = 0; i < listObjects.size(); i++)
//		{
//			if (listObjects[i]->GetBBARGB() == 0)
//				listObjects[i]->SetBBARGB(200);
//			else
//				listObjects[i]->SetBBARGB(0);
//		}
//		break;
//	case DIK_C:
//		if (player->IsDeadYet() || player->IsAttacking() || player->IsSitting() || player->IsHurting() || player->IsUpgrading())
//			return;
//		player->SetState(PLAYER_STATE_JUMP);
//		break;
//	case DIK_X:
//		if (player->IsDeadYet() || player->IsHurting() || player->IsUpgrading() || game->IsKeyDown(DIK_UP))	//Up + X khong Whip duoc nua
//			return;
//		player->SetState(PLAYER_STATE_ATTACK);
//		break;
//	}
//}
//
//void CSampleKeyHander::OnKeyUp(int KeyCode)
//{
//	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
//}
//
//void CSampleKeyHander::KeyState(BYTE *states)
//{
//	if (player->IsDeadYet() || player->IsAttacking() || player->IsJumping() || player->IsHurting() || player->IsUpgrading()) {	
//		return;
//	}
//
//	if (game->IsKeyDown(DIK_UP) && game->IsKeyDown(DIK_X) && !player->IsAttacking())
//	{
//		if (player->GetPlayerSupWeaponType() != EntityType::NONE)	//Neu chua nhat duoc vu khi phu thi khong attack
//		{
//			player->SetState(PLAYER_STATE_SUPWEAPON_ATTACK);
//		}
//	}
//
//	if (game->IsKeyDown(DIK_DOWN)) {
//		player->SetState(PLAYER_STATE_SITTING);
//		if (game->IsKeyDown(DIK_RIGHT))
//			player->SetDirection(1);
//		else if (game->IsKeyDown(DIK_LEFT))
//			player->SetDirection(-1);
//		return;				//Important return //Dont change state while sitting
//	}
//	else player->SetState(PLAYER_STATE_IDLE);
//
//	if (game->IsKeyDown(DIK_RIGHT))
//		player->SetState(PLAYER_STATE_WALKING_RIGHT);
//	else if (game->IsKeyDown(DIK_LEFT))
//		player->SetState(PLAYER_STATE_WALKING_LEFT);
//	else player->SetState(PLAYER_STATE_IDLE);
//}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = InitWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = Game::GetInstance();
	game->Init(hWnd);
	game->InitKeyboard();

	game->Load(L"Resources/Scene/Castlevania-scenemanager.txt");
	GameLoop();

	return 0;
}
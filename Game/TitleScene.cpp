#include "TitleScene.h"
#include "SceneManager.h"
#include "IntroScene.h"

TitleScene::TitleScene() : Scene()
{
	keyHandler = new TitleScenceKeyHandler(this);
	Game::GetInstance()->SetKeyHandler(this->GetKeyEventHandler());

	screenTexture = Texture2dManager::GetInstance()->GetTexture(EntityType::TITLESCENE);
	screenSprite = new Sprite(screenTexture, MaxFrameRate);

	batTexture = Texture2dManager::GetInstance()->GetTexture(EntityType::TITLEBAT);
	batSprite = new Sprite(batTexture, MaxFrameRate);

	startTexture = Texture2dManager::GetInstance()->GetTexture(EntityType::STARTTEXT);
	startSprite = new Sprite(startTexture, MaxFrameRate);

	isAllowToStart = false;
}

void TitleScene::Load()
{

}

void TitleScene::Unload()
{

}

void TitleScene::Update(DWORD dt)
{
	int currentFrame = batSprite->GetCurrentFrame();
	if (currentFrame < BAT_ANI_BEGIN)
	{
		batSprite->SelectFrame(BAT_ANI_BEGIN);
		batSprite->currentTotalTime = dt;
	}
	else
	{
		batSprite->currentTotalTime += dt;
		if (batSprite->currentTotalTime >= BAT_FLAPPING_SPEED)
		{
			batSprite->currentTotalTime -= BAT_FLAPPING_SPEED;
			batSprite->SelectFrame(currentFrame + 1);
		}

		if (currentFrame == BAT_ANI_END) 
		{
			batSprite->SelectFrame(BAT_ANI_START_LOOP);
			isAllowToStart = true;
		}
	}

	if (isAllowToStart)
	{
		int currentStartFrame = startSprite->GetCurrentFrame();
		if (currentStartFrame < STARTTEXT_BEGIN)
		{
			startSprite->SelectFrame(STARTTEXT_BEGIN);
			startSprite->currentTotalTime = dt;
		}
		else
		{
			startSprite->currentTotalTime += dt;
			if (startSprite->currentTotalTime >= STARTTEXT_TRANS_SPEED)
			{
				startSprite->currentTotalTime -= STARTTEXT_TRANS_SPEED;
				startSprite->SelectFrame(currentFrame + 1);
			}

			if (currentStartFrame == STARTTEXT_END)
			{
				startSprite->SelectFrame(STARTTEXT_BEGIN);
			}
		}
	}
}

void TitleScene::Render()
{
	screenSprite->Draw(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	batSprite->Draw(SCREEN_WIDTH / 2 + 185, SCREEN_HEIGHT / 2 + 40);
	startSprite->Draw(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 41);
}


void TitleScenceKeyHandler::OnKeyDown(int KeyCode)
{
	bool canStart = ((TitleScene*)scence)->isAllowToStart;

	switch (KeyCode)
	{
	case DIK_ESCAPE:
		DestroyWindow(Game::GetInstance()->GetWindowHandle());
	case DIK_I:
		if (canStart)
		{
			((TitleScene*)scence)->Unload();
			SceneManager::GetInstance()->SetScene(new IntroScene());
		}

		break;
	}
}

void TitleScenceKeyHandler::OnKeyUp(int KeyCode)
{
}

void TitleScenceKeyHandler::KeyState(BYTE *states)
{
}

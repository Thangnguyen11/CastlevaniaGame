#pragma once
#include "Texture2d.h"

class Sprite
{
	int currentFrame, totalFrame;
	float timePerFrame;
	LPD3DXSPRITE spriteHandler;

public:
	float currentTotalTime;
	Texture2d* texture;
	Sprite(Texture2d* TEXTURE, float TIMEPERFRAME = 0.1f);	//Khong chinh timeperframe cho moi ani duoc
	~Sprite();

	void Update(float dt);
	void Draw(float posX, float posY, int alpha = 255, int R = 255, int G = 255, int B = 255);
	void DrawFlipVertical(float posX, float posY, int alpha = 255, int R = 255, int G = 255, int B = 255);

	RECT GetRectFrame(int CURRENTFRAME);

	int GetCurrentFrame() { return currentFrame; }
	void SelectFrame(int idFrame) { currentFrame = idFrame; }
};


#pragma once

#include "Game.h"

class Texture2d
{
	int column, row, frameWidth, frameHeight;

public:
	int totalFrame;
	LPDIRECT3DTEXTURE9 Texture;

	Texture2d(char* filePath, int COLUMN = 1, int ROW = 1, int TOTALFRAME = 1, D3DCOLOR colorKey = NULL);
	~Texture2d();

	int getColumn() { return column; }
	int getRow() { return row; }
	int getFrameWidth() { return frameWidth; }
	int getFrameHeight() { return frameHeight; }
};


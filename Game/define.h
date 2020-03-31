#pragma once

#include <Windows.h>  
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include "debug.h" 

#include <fstream> 
#include <string>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <iostream>

#define WIN_NAME L"NewbieGame"
#define WIN_TITLE L"NewbieGame"

#define BGColor D3DCOLOR_XRGB(0,0,0)
#define SCREEN_WIDTH 780
#define SCREEN_HEIGHT 660

#define MaxFrameRate 100

enum EntityType
{
	BBOX = 0,

	PLAYER = 1,

	BRICK = 2,

	BAT = 3,

	ZOMBIE = 4,

	MORNINGSTAR = 5,

	PLAYERHP = 6,

	SMALLHEART = 7,

	BIGHEART = 8,

	YUMMICHICKENLEG = 9,

	MONEYBAGS = 10,

	MONEYBAGRED = 11,

	MONEYBAGWHITE = 12,

	MONEYBAGBLUE = 13,

	UPGRADEMORNINGSTAR = 14,

	CRYSTALBALL = 15,

	INVIPOTION = 16,

	DAGGER = 17,

	STOPWATCH = 18,

	CROSS = 19,

	AXE = 20,

	BOOMERANG = 21,

	WATERPOTION = 22,

	TORCH = 23,

	CANDLE = 24
};

class define
{
};


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

	LARGEHEART = 8,

	YUMMICHICKENLEG = 9,

	MONEYBAGS = 10,

	UPGRADEMORNINGSTAR = 11,

	CRYSTALBALL = 12,

	INVIPOTION = 13,

	DAGGER = 14,

	STOPWATCH = 15,

	CROSS = 16,

	AXE = 17,

	BOOMERANG = 18,

	WATERPOTION = 19
};

class define
{
};


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

#define BGColor D3DCOLOR_XRGB(255,255,255)
#define SCREEN_WIDTH 1365
#define SCREEN_HEIGHT 750

#define MaxFrameRate 100

enum EntityType
{
	PLAYER = 01,

	BRICK = 02,

	BAT = 03
};

class define
{
};


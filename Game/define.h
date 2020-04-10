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
#define SCREEN_WIDTH 530
#define SCREEN_HEIGHT 480

#define MaxFrameRate 100	//default Frame Rate is the truth

#define SCENEGAME_GAMETIMEMAX		300

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024


#define MAX_SCENE_LINE 1024

//Nen define thanh nhieu lop type
//Vd Zombie thuoc EntityType, EnemyType
enum EntityType
{
	NONE = -1,

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

	ITEMDAGGER = 17,

	STOPWATCH = 18,

	CROSS = 19,

	AXE = 20,

	BOOMERANG = 21,

	WATERPOTION = 22,

	TORCH = 23,

	CANDLE = 24,

	TEXTS = 25,

	BLACKBOARD = 26,

	FIREEFFECT = 27,

	HITEFFECT = 28,

	BUBBLESEFFECT = 29,

	BRICKEXPLODEEFFECT = 30,

	ADDSCOREEFFECT = 31,

	DAGGER = 32,

	GATE = 33,

	TITLESCENE = 34,

	TITLEBAT = 35,

	STARTTEXT = 36,
	
	INTROSCENE = 37,

	INTROSIMON = 38,

	PLAYSCENE = 39,

	MAPSTAGE1 = 40,

	MAPSTAGE2 = 41,

	STAIRS = 42
};

class define
{
};


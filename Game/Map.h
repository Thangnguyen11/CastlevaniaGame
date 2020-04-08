#pragma once
#include "Texture2dManager.h"
#include "Sprite.h"
#include "Camera.h"

#define STAGE1_ROWMAP			6
#define STAGE1_COLUMNMAP		24
#define STAGE1_BOTTOMDIST		48

#define STAGE2_ROWMAP			12
#define STAGE2_COLUMNMAP		88
#define STAGE2_BOTTOMDIST		48

//Co the chia class ra thanh MapStage1, MapStage2
class Map
{
	Texture2d* tileMapTexture;	//Tileset
	Sprite* tileMapSprite;

	LPCWSTR mapFilePath;

	int ColumnMap, RowMap, BottomDist;	//So cot, hang cua map, Khoang cach voi day screen cua map

	int ColumnTexture, RowTexture, TotalFrameTexture, FrameWidthTexture, FrameHeightTexture;

	int TileMap[100][100];
	//So cot va hang toi da phai lon hon Screen Resolution chia cho do cao/dai cua frame tile
	//So cot va hang toi da cua Map (thay doi tuy y, Castlevania khong qua lon nen 100 is ok)
public:
	Map();
	~Map();

	void LoadMap(EntityType typeMap);
	void Load();
	//void Update(DWORD dt); co the se co map dung update
	void Draw();
};


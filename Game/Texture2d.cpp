#include "Texture2d.h"

Texture2d::Texture2d(char* filePath, int COLUMN, int ROW, int TOTALFRAME, D3DCOLOR colorKey)
{
	this->column = COLUMN;
	this->row = ROW;
	this->totalFrame = TOTALFRAME;

	D3DXIMAGE_INFO info;
	this->frameWidth = info.Width / column;
	this->frameHeight = info.Height / row;

	HRESULT result = D3DXGetImageInfoFromFileA(filePath, &info);

	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = Game::GetInstance()->GetDirect3DDevice();
	result = D3DXCreateTextureFromFileExA(
		d3ddv,								// Pointer to Direct3D device object
		filePath,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		colorKey,
		&info,
		NULL,
		&Texture);							// Created textTexture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}
}

Texture2d::~Texture2d() 
{
	if (this->Texture != NULL)
		this->Texture->Release();
}

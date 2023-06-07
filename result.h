//=============================================================================
//
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct STAFF
{
	D3DXVECTOR3		pos;		// ポリゴンの座標(スクロール前)
	D3DXVECTOR3		pos2;		// ポリゴンの座標(スクロール後)
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
	float			offset;		// テクスチャの移動量
	int				wait;
};

struct RESULTBG
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
};

struct RESULT
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
	float			alpha;		// 使用しているテクスチャ番号
	int				bgmuse;		// SEを鳴らすフラグ
	BOOL			use;
	int				wait;
};

struct KEY
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

void SetGAMECLEAR(void);
void SetGAMEOVER(void);
RESULT *GetGameClear(void);
RESULT *GetGameOver(void);


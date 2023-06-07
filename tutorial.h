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
#define WINDOW_MAX				(1)		// ウィンドウの数
#define DUMMY_MAX				(2)		// ダミーの数

struct TUTORIAL
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	D3DXVECTOR3		rot;		// ポリゴンの回転量
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
	BOOL			use;

	int				countAnim;	// アニメーションカウント
	int				patternAnim;// アニメーションパターンナンバー
};

struct WINDOW
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	D3DXVECTOR3		rot;		// ポリゴンの回転量
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
	BOOL			use;

	int				countAnim;	// アニメーションカウント
	int				patternAnim;// アニメーションパターンナンバー
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

TUTORIAL *GetDummy(void);
TUTORIAL *GetMark(void);


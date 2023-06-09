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

struct ENEMUI
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
	BOOL			use;

	float			pw;			// ポリゴンの幅

	int				countAnim;	// アニメーションカウント
	int				patternAnim;// アニメーションパターンナンバー
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemUI(void);
void UninitEnemUI(void);
void UpdateEnemUI(void);
void DrawEnemUI(void);


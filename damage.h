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
#define DAMAGE_MAX			(100)		// スコアの最大値 / 使えるダメージの数
#define DAMAGE_DIGIT		(2)			// 桁数

#define	DAMAGE_DX			(600.0f)	// 表示位置
#define	DAMAGE_DY			(20.0f)		// 
#define	DAMAGE_DW			(30.0f)		// 
#define	DAMAGE_DH			(50.0f)		// 

struct DAMAGE
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	D3DXVECTOR3		rot;		// ポリゴンの回転率
	int				texNo;		// 使用しているテクスチャ番号

	BOOL			use;		// ダメージ表示フラグ
	int				damage;		// ダメージ

	float			move;		// ダメージの移動量
	float			alpha;		// ダメージの不透明度
	int				time;		// アニメ用
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDamage(void);
void UninitDamage(void);
void UpdateDamage(void);
void DrawDamage(void);

int GetDamage(void);
void AddDamage(int add, int no);
void SetDamage(D3DXVECTOR3 pos, int damage);



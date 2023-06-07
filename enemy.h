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
#define ENEMY_MAX		(17)		// エネミーのMax人数

enum
{
	ENEM_STD_DOWN,
	ENEM_STD_LEFT,
	ENEM_STD_RIGHT,
	ENEM_STD_UP,
	ENEM_DIR_DOWN,
	ENEM_DIR_LEFT,
	ENEM_DIR_RIGHT,
	ENEM_DIR_UP,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct	LINEAR_INTERPOLATION		// 線形補間データの構造体
{
	D3DXVECTOR3		pos;			// 座標
	D3DXVECTOR3		rot;			// 回転量
	D3DXVECTOR3		scl;			// 拡大率
	float			time;			// 移動時間
};


struct ENEMY
{
	D3DXVECTOR3		pos;			// ポリゴンの座標
	D3DXVECTOR3		pos_old;		// 移動前のポリゴンの座標
	D3DXVECTOR3		rot;			// ポリゴンの回転量
	D3DXVECTOR3		scl;			// ポリゴンの拡大率
	BOOL			use;			// true:使っている  false:未使用
	float			w, h;			// 幅と高さ
	int				countAnim;		// アニメーションカウント
	int				patternAnim;	// アニメーションパターンナンバー
	int				texNo;			// テクスチャ番号
	D3DXVECTOR3		move;			// 移動速度
	float			pop;			// 登場させるタイミング
	BOOL			homing;			// true:ホーミングする  false:しない

	int				dir;			// 向き（0:下 1:左 2:右 3:上 4:下 5:左 6:右 7:上）
	BOOL			moving;			// 移動中フラグ
	int				shadowtexNo;	// 影のテクスチャ番号
	
	float			time;			// 線形補間用
	int				moveTblNo;		// 行動データのテーブル番号
	int				tblMax;			// そのテーブルのデータ数

	int				HP, HP_MAX;		// HP,HP_MAX
	int				atk;			// 攻撃力

	D3DXVECTOR3		skillpos;		// スキルAの座標
	float			skillmove;		// スキルAの移動量
	int				PerSecond;		// 単位時間当たりに行動させる用

	D3DXVECTOR3		damagepos;		// ダメージの座標
	float			damagemove;		// ダメージの移動量

	BOOL			hit;			// TRUE:当たり判定を行う  FALSE:行わない

	float			alpha;			// 不透明度
	BOOL			hitted;			// 攻撃されるフラグ
	int				hittedtime;		// アニメーションを描画時用
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);




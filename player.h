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
#define PLAYER_MAX			(1)		// プレイヤーのMax人数

#define	PLAYER_OFFSET_CNT	(10)	// 16分身

enum
{
	CHAR_STD_DOWN,
	CHAR_STD_LEFT,
	CHAR_STD_RIGHT,
	CHAR_STD_UP,
	CHAR_DIR_DOWN,
	CHAR_DIR_LEFT,
	CHAR_DIR_RIGHT,
	CHAR_DIR_UP,
};


//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct PLAYER
{
	D3DXVECTOR3		pos;			// ポリゴンの座標
	D3DXVECTOR3 pos_old;			// 移動前のポリゴンの座標
	D3DXVECTOR3		rot;			// ポリゴンの回転量
	BOOL			use;			// true:使っている  false:未使用
	float			w, h;			// 幅と高さ
	float			countAnim;		// アニメーションカウント
	int				patternAnim;	// アニメーションパターンナンバー
	int				texNo;			// テクスチャ番号
	D3DXVECTOR3		move;			// 移動速度

	int				dir;			// 向き（0:下 1:左 2:右 3:上 4:下 5:左 6:右 7:上）
	BOOL			moving;			// 移動中フラグ

	BOOL			dashing;		// ダッシュ中フラグ
	D3DXVECTOR3		offset[PLAYER_OFFSET_CNT];		// 残像ポリゴンの座標

	int				shadowtexNo;	// 影のテクスチャ番号

	D3DXVECTOR3		skillpos_a;		// スキルA(spaceキー/□ボタン)の座標
	float			skillmove_a;	// スキルA(spaceキー/□ボタン)の移動量

	D3DXVECTOR3		skillpos_b;		// スキルB(mキー/△ボタン)の座標
	float			skillmove_b;	// スキルB(mキー/△ボタン)の移動量
	int				skilltexNo;		// スキルのテクスチャ番号
	BOOL			skilluse;		// スキルチャージ中フラグ
	int				skillusing;		// 使用中のスキルを計算する用

	int				spellcouAnim;	// アニメーションカウント
	int				spellpatAnim;	// アニメーションパターンナンバー
	int				spelltexNo;		// テクスチャ番号


	int				HP,HP_MAX;		// HP,HP_MAX
	int				ST,ST_MAX;		// スタミナ,スタミナ_MAX
	int				atk;			// 攻撃力
	int				PerSecond;		// 単位時間当たりに行動させる用

	D3DXVECTOR3		damagepos;		// ダメージの座標
	float			damagemove;		// ダメージの移動量

	float			alpha;			// 不透明度
	BOOL			hitted;			// 攻撃されるフラグ
	int				hittedtime;		// アニメーションを描画時用
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);
BOOL GetGoal(void);




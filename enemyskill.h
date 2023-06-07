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
#define ENEMYSKILLA_MAX		(100)			// スキルAのMax数
#define ENEMYSKILLB_MAX		(300)			// スキルBのMax数
#define ENEMYSKILLC_MAX		(100)			// スキルCのMax数


// スキル構造体
struct ENEMYSKILL
{
	BOOL					use;					// true:使っている  false:未使用
	BOOL					hituse;					// true:当たり判定行う  false:行わない
	float					w, h;					// 幅と高さ
	D3DXVECTOR3				pos;					// スキルの座標
	D3DXVECTOR3				rot;					// スキルの回転量
	D3DXVECTOR3				move;					// スキルの移動量
	int						countAnim;				// アニメーションカウント
	int						patternAnim;			// アニメーションパターンナンバー
	int						texNo;					// 何番目のテクスチャーを使用するのか

	int						damage;
	int						time;					// スキルBの存在時間
	float					angle;					// スキルBの軌道の角度
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemySkill(void);
void UninitEnemySkill(void);
void UpdateEnemySkill(void);
void DrawEnemySkill(void);

ENEMYSKILL *GetEnemySkillA(void);
ENEMYSKILL *GetEnemySkillB(void);
void SetEnemySkillA(D3DXVECTOR3 pos, int damage);
void SetEnemySkillB(D3DXVECTOR3 pos, int damage);
void SetEnemySkillC(D3DXVECTOR3 pos, int damage);


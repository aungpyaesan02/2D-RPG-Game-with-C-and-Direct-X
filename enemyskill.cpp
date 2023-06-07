//=============================================================================
//

//
//=============================================================================
#include "enemyskill.h"
#include "sprite.h"
#include "player.h"
#include "collision.h"
#include "damage.h"
#include "sound.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH_1				(200)	// キャラサイズ
#define TEXTURE_HEIGHT_1			(150)	// 
#define TEXTURE_WIDTH_2				(80)	// キャラサイズ
#define TEXTURE_HEIGHT_2			(80)	// 
#define TEXTURE_WIDTH_3				(200)	// キャラサイズ
#define TEXTURE_HEIGHT_3			(200)	// 

#define TEXTURE_MAX					(3)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X_1	(2)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_1	(12)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_1			(TEXTURE_PATTERN_DIVIDE_X_1*TEXTURE_PATTERN_DIVIDE_Y_1)	// アニメーションパターン数
#define TEXTURE_PATTERN_DIVIDE_X_2	(10)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_2	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_2			(TEXTURE_PATTERN_DIVIDE_X_2*TEXTURE_PATTERN_DIVIDE_Y_2)	// アニメーションパターン数
#define TEXTURE_PATTERN_DIVIDE_X_3	(5)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_3	(2)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_3			(TEXTURE_PATTERN_DIVIDE_X_3*TEXTURE_PATTERN_DIVIDE_Y_3)	// アニメーションパターン数
#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/TEXTURE/enemyskill01.png",
	"data/TEXTURE/enemyskill02.png",
	"data/TEXTURE/enemyskill03.png",
};

static BOOL	  g_Load = FALSE;		// 初期化を行ったかのフラグ
static ENEMYSKILL g_Skilla[ENEMYSKILLA_MAX];	// スキル構造体
static ENEMYSKILL g_Skillb[ENEMYSKILLB_MAX];	// スキル構造体
static ENEMYSKILL g_Skillc[ENEMYSKILLC_MAX];	// スキル構造体


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemySkill(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// スキルA構造体の初期化
	for (int i = 0; i < ENEMYSKILLA_MAX; i++)
	{
		g_Skilla[i].use   = FALSE;			// 未使用
		g_Skilla[i].w     = TEXTURE_WIDTH_1;
		g_Skilla[i].h     = TEXTURE_HEIGHT_1;
		g_Skilla[i].pos   = D3DXVECTOR3(300, 300.0f, 0.0f);
		g_Skilla[i].rot   = D3DXVECTOR3(0.0f, 0.0f, 1.57f);
		g_Skilla[i].texNo = 0;

		g_Skilla[i].countAnim = 0;
		g_Skilla[i].patternAnim = 0;

		g_Skilla[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量を初期化
	
		g_Skilla[i].damage = 0;
		g_Skilla[i].time = 0;
		g_Skilla[i].angle = 0.0f;
	}
	
	// スキルB構造体の初期化
	for (int i = 0; i < ENEMYSKILLB_MAX; i++)
	{
		g_Skillb[i].use = FALSE;			// 未使用
		g_Skillb[i].w = TEXTURE_WIDTH_2;
		g_Skillb[i].h = TEXTURE_HEIGHT_2;
		g_Skillb[i].pos = D3DXVECTOR3(300, 300.0f, 0.0f);
		g_Skillb[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Skillb[i].texNo = 1;

		g_Skillb[i].countAnim = 0;
		g_Skillb[i].patternAnim = 0;

		g_Skillb[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量を初期化

		g_Skillb[i].damage = 0;
		g_Skillb[i].time = 0;
		g_Skillb[i].angle = 0.0f;
	}
	
	// スキルC構造体の初期化
	for (int i = 0; i < ENEMYSKILLC_MAX; i++)
	{
		g_Skillc[i].use = FALSE;			// 未使用
		g_Skillc[i].w = TEXTURE_WIDTH_3;
		g_Skillc[i].h = TEXTURE_HEIGHT_3;
		g_Skillc[i].pos = D3DXVECTOR3(300, 300.0f, 0.0f);
		g_Skillc[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Skillc[i].texNo = 2;

		g_Skillc[i].countAnim = 0;
		g_Skillc[i].patternAnim = 0;

		g_Skillc[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量を初期化

		g_Skillc[i].damage = 0;
		g_Skillc[i].time = 0;
		g_Skillc[i].angle = 0.0f;
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemySkill(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemySkill(void)
{
	BG *bg = GetBG();

	int skillCounta = 0;				// 処理したスキルの数

	for (int i = 0; i < ENEMYSKILLA_MAX; i++)
	{
		if (g_Skilla[i].use == TRUE)	// このスキルが使われている？
		{								// Yes
			// アニメーション  
			
			g_Skilla[i].countAnim++;
			if ((g_Skilla[i].countAnim % ANIM_WAIT) == 0)
			{
				// パターンの切り替え
				g_Skilla[i].patternAnim = (g_Skilla[i].patternAnim + 1) % ANIM_PATTERN_NUM_1;
			}
			
			if (g_Skilla[i].countAnim == (ANIM_WAIT*ANIM_PATTERN_NUM_1))
			{
				g_Skilla[i].use = FALSE;
				g_Skilla[i].countAnim = 0;
			}
			

			// 当たり判定処理
			{
				PLAYER *player = GetPlayer();

				// エネミーの数分当たり判定を行う
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// 生きてるエネミーと当たり判定をする
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Skilla[i].pos, g_Skilla[i].w, g_Skilla[i].h - 10,
							player[j].pos, player[j].w, player[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							if (g_Skilla[i].hituse == TRUE)
							{
								player[j].hitted = TRUE; 
								player[j].HP -= g_Skilla[i].damage;
								if (player[j].HP < 1)
								{
									player[j].HP = 0;
									player[j].use = FALSE;
								}
								PlaySound(SOUND_LABEL_SE_hit);
								D3DXVECTOR3 pos = player[j].damagepos;
								SetDamage(pos, g_Skilla[i].damage);
								g_Skilla[i].hituse = FALSE;		// 当たり判定フラグ
							}
						}
					}
				}
			}

			skillCounta++;
		}
	}


	int skillCountb = 0;				// 処理したスキルの数

	for (int i = 0; i < ENEMYSKILLB_MAX; i++)
	{
		if (g_Skillb[i].use == TRUE)	// このスキルが使われている？
		{								// Yes
			// アニメーション  

			g_Skillb[i].countAnim++;
			if ((g_Skillb[i].countAnim % ANIM_WAIT) == 0)
			{
				// パターンの切り替え
				g_Skillb[i].patternAnim = (g_Skillb[i].patternAnim + 1) % ANIM_PATTERN_NUM_2;
			}

			// スキルの移動処理
			{
				PLAYER *player = GetPlayer();
				g_Skillb[i].time++;
				
				D3DXVECTOR3 pos = player[0].pos - g_Skillb[i].pos;	// 自分とプレイヤーの差分を求めている
				float angle = atan2f(pos.y, pos.x) + D3DX_PI;		// その差分を使って角度を求めている
				float speed = -3.0f;								// スピードはちょっと遅くしてみる

				if (g_Skillb[i].time < 120)
				{
					g_Skillb[i].pos.x += cosf(angle) * speed;						// angleの方向へ移動
					g_Skillb[i].pos.y += sinf(angle) * speed;						// angleの方向へ移動
				}
				
				if (g_Skillb[i].time == 120)
				{
					g_Skillb[i].angle = angle;
				}
				
				if (g_Skillb[i].time > 120 && g_Skillb[i].time < 270)
				{
					g_Skillb[i].pos.x += cosf(g_Skillb[i].angle) * speed;						// angleの方向へ移動
					g_Skillb[i].pos.y += sinf(g_Skillb[i].angle) * speed;						// angleの方向へ移動
				}
				
				if (g_Skillb[i].time > 270)
				{
					g_Skillb[i].use = FALSE;
					g_Skillb[i].time = 0;
				}
			}
		
			//// 画面外まで進んだ？
			//if (g_Skillb[i].pos.y < bg->pos.y + (-g_Skillb[i].h / 2))		// 自分の大きさを考慮して画面外か判定している
			//{
			//	g_Skillb[i].use = false;
			//}
			//if (g_Skillb[i].pos.y > bg->pos.y + (SCREEN_HEIGHT + g_Skillb[i].h / 2))	// 自分の大きさを考慮して画面外か判定している
			//{
			//	g_Skillb[i].use = false;
			//}

			// 当たり判定処理
			{
				PLAYER *player = GetPlayer();

				// エネミーの数分当たり判定を行う
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// 生きてるエネミーと当たり判定をする
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Skillb[i].pos, g_Skillb[i].w, g_Skillb[i].h,
							player[j].pos, player[j].w, player[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							if (g_Skillb[i].hituse == TRUE)
							{
								g_Skillb[i].use = FALSE;		// 当たり判定フラグ
								player[j].hitted = TRUE;
								player[j].HP -= g_Skillb[i].damage;
								if (player[j].HP < 1)
								{
									player[j].HP = 0;
									player[j].use = FALSE;
								}
								PlaySound(SOUND_LABEL_SE_hit);
								D3DXVECTOR3 pos = player[j].damagepos;
								SetDamage(pos, g_Skillb[i].damage);
								g_Skillb[i].hituse = FALSE;		// 当たり判定フラグ
							}
						}
					}
				}
			}

			skillCountb++;
		}
	}


	int skillCountc = 0;				// 処理したスキルの数

	for (int i = 0; i < ENEMYSKILLC_MAX; i++)
	{
		if (g_Skillc[i].use == TRUE)	// このスキルが使われている？
		{								// Yes
			// アニメーション  

			g_Skillc[i].countAnim++;
			if ((g_Skillc[i].countAnim % ANIM_WAIT) == 0)
			{
				// パターンの切り替え
				g_Skillc[i].patternAnim = (g_Skillc[i].patternAnim + 1) % ANIM_PATTERN_NUM_3;
			}

			if (g_Skillc[i].countAnim == (ANIM_WAIT*ANIM_PATTERN_NUM_3))
			{
				g_Skillc[i].use = FALSE;
				g_Skillc[i].countAnim = 0;
			}


			// 当たり判定処理
			{
				PLAYER *player = GetPlayer();

				// エネミーの数分当たり判定を行う
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// 生きてるエネミーと当たり判定をする
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Skillc[i].pos, g_Skillc[i].w, g_Skillc[i].h,
							player[j].pos, player[j].w, player[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							if (g_Skillc[i].hituse == TRUE)
							{
								player[j].hitted = TRUE;
								player[j].HP -= g_Skillc[i].damage;
								if (player[j].HP < 1)
								{
									player[j].HP = 0;
									player[j].use = FALSE;
								}
								PlaySound(SOUND_LABEL_SE_hit);
								D3DXVECTOR3 pos = player[j].damagepos;
								SetDamage(pos, g_Skillc[i].damage);
								g_Skillc[i].hituse = FALSE;		// 当たり判定フラグ
							}
						}
					}
				}
			}

			skillCountc++;
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemySkill(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG *bg = GetBG();

	for (int i = 0; i < ENEMYSKILLA_MAX; i++)
	{
		if (g_Skilla[i].use == TRUE)		// このスキルが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skilla[i].texNo]);

			//スキルの位置やテクスチャー座標を反映
			float px = g_Skilla[i].pos.x - bg->pos.x;	// スキルの表示位置X
			float py = g_Skilla[i].pos.y - bg->pos.y;	// スキルの表示位置Y
			float pw = g_Skilla[i].w;		// スキルの表示幅
			float ph = g_Skilla[i].h;		// スキルの表示高さ

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_1;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_1;	// テクスチャの高さ
			float tx = (float)(g_Skilla[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X_1) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Skilla[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X_1) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, 
				px, py, pw, ph, 
				tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				g_Skilla[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	for (int i = 0; i < ENEMYSKILLB_MAX; i++)
	{
		if (g_Skillb[i].use == TRUE)		// このスキルが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skillb[i].texNo]);

			//スキルの位置やテクスチャー座標を反映
			float px = g_Skillb[i].pos.x - bg->pos.x;	// スキルの表示位置X
			float py = g_Skillb[i].pos.y - bg->pos.y;	// スキルの表示位置Y
			float pw = g_Skillb[i].w;		// スキルの表示幅
			float ph = g_Skillb[i].h;		// スキルの表示高さ

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_2;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_2;	// テクスチャの高さ
			float tx = (float)(g_Skillb[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X_2) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Skillb[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X_2) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				g_Skillb[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	for (int i = 0; i < ENEMYSKILLC_MAX; i++)
	{
		if (g_Skillc[i].use == TRUE)		// このスキルが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skillc[i].texNo]);

			//スキルの位置やテクスチャー座標を反映
			float px = g_Skillc[i].pos.x - bg->pos.x;	// スキルの表示位置X
			float py = g_Skillc[i].pos.y - bg->pos.y;	// スキルの表示位置Y
			float pw = g_Skillc[i].w;		// スキルの表示幅
			float ph = g_Skillc[i].h;		// スキルの表示高さ

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_3;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_3;	// テクスチャの高さ
			float tx = (float)(g_Skillc[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X_3) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Skillc[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X_3) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				g_Skillc[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// スキル構造体の先頭アドレスを取得
//=============================================================================
ENEMYSKILL *GetEnemySkillA(void)
{
	return &g_Skilla[0];
}

ENEMYSKILL *GetEnemySkillB(void)
{
	return &g_Skillb[0];
}


//=============================================================================
// スキルAの使用設定
//=============================================================================
void SetEnemySkillA(D3DXVECTOR3 pos,int damage)
{
	// もし未使用のスキル数が無かったら使わない( =これ以使ってないって事 )
	for (int i = 0; i < ENEMYSKILLA_MAX; i++)
	{
		if (g_Skilla[i].use == FALSE)		// 未使用状態のスキルを見つける
		{
			g_Skilla[i].use = TRUE;			// 使用状態へ変更する
			g_Skilla[i].hituse = TRUE;		// 当たり判定フラグ
			g_Skilla[i].pos = pos;			// 座標をセット
			g_Skilla[i].damage = damage;
			PlaySound(SOUND_LABEL_SE_enemyskill_a);
			return;							// 1発セットしたので終了する
		}
	}
}

//=============================================================================
// スキルBの使用設定
//=============================================================================
void SetEnemySkillB(D3DXVECTOR3 pos, int damage)
{
	// もし未使用のスキル数が無かったら使わない( =これ以使ってないって事 )
	for (int i = 0; i < ENEMYSKILLB_MAX; i++)
	{
		if (g_Skillb[i].use == FALSE)		// 未使用状態のスキルを見つける
		{
			g_Skillb[i].use = TRUE;			// 使用状態へ変更する
			g_Skillb[i].hituse = TRUE;		// 当たり判定フラグ
			g_Skillb[i].pos = pos;			// 座標をセット
			g_Skillb[i].damage = damage;
			PlaySound(SOUND_LABEL_SE_enemyskill_b);
			return;							// 1発セットしたので終了する
		}
	}
}

//=============================================================================
// スキルCの使用設定
//=============================================================================
void SetEnemySkillC(D3DXVECTOR3 pos, int damage)
{
	// もし未使用のスキル数が無かったら使わない( =これ以使ってないって事 )
	for (int i = 0; i < ENEMYSKILLC_MAX; i++)
	{
		if (g_Skillc[i].use == FALSE)		// 未使用状態のスキルを見つける
		{
			g_Skillc[i].use = TRUE;			// 使用状態へ変更する
			g_Skillc[i].hituse = TRUE;		// 当たり判定フラグ
			g_Skillc[i].pos = pos;			// 座標をセット
			g_Skillc[i].damage = damage;
			PlaySound(SOUND_LABEL_SE_enemyskill_c1);
			PlaySound(SOUND_LABEL_SE_enemyskill_c2);
			PlaySound(SOUND_LABEL_SE_enemyskill_c3);
			return;							// 1発セットしたので終了する
		}
	}
}

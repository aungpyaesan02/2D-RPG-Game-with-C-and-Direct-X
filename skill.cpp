//=============================================================================
//

//
//=============================================================================
#include "skill.h"
#include "sprite.h"
#include "enemy.h"
#include "player.h"
#include "collision.h"
#include "damage.h"
#include "sound.h"
#include "bg.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH_1				(150)	// キャラサイズ
#define TEXTURE_HEIGHT_1			(150)	// 
#define TEXTURE_MAX					(2)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X_1	(5)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_1	(2)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_1			(TEXTURE_PATTERN_DIVIDE_X_1*TEXTURE_PATTERN_DIVIDE_Y_1)	// アニメーションパターン数
#define TEXTURE_PATTERN_DIVIDE_X_2	(2)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_2	(5)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_2			(TEXTURE_PATTERN_DIVIDE_X_2*TEXTURE_PATTERN_DIVIDE_Y_2)	// アニメーションパターン数

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
	"data/TEXTURE/skillA.png",
	"data/TEXTURE/skillB.png",
 };

static BOOL	  g_Load = FALSE;		// 初期化を行ったかのフラグ
static SKILL g_Skilla[SKILLA_MAX];	// スキル構造体
static SKILL g_Skillb[SKILLB_MAX];	// スキル構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSkill(void)
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
	for (int i = 0; i < SKILLA_MAX; i++)
	{
		g_Skilla[i].use   = FALSE;			// 未使用
		g_Skilla[i].w     = TEXTURE_WIDTH_1;
		g_Skilla[i].h     = TEXTURE_HEIGHT_1;
		g_Skilla[i].pos   = D3DXVECTOR3(300, 300.0f, 0.0f);
		g_Skilla[i].rot   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Skilla[i].texNo = 0;

		g_Skilla[i].countAnim = 0;
		g_Skilla[i].patternAnim = 0;

		g_Skilla[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量を初期化
		g_Skilla[i].hit = FALSE;
		g_Skilla[i].damage = 0;

	}
	
	// スキルB構造体の初期化
	for (int i = 0; i < SKILLB_MAX; i++)
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
		g_Skillb[i].hit = FALSE;
		g_Skillb[i].damage = 0;
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSkill(void)
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
void UpdateSkill(void)
{
	BG *bg = GetBG();

	int skillCounta = 0;				// 処理したスキルの数

	for (int i = 0; i < SKILLA_MAX; i++)
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
				ENEMY *enemy = GetEnemy();

				// エネミーの数分当たり判定を行う
				for (int j = 0; j < ENEMY_MAX; j++)
				{
					// 生きてるエネミーと当たり判定をする
					if (enemy[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Skilla[i].pos, g_Skilla[i].w/2, g_Skilla[i].h/2,
							enemy[j].pos, enemy[j].w-20, enemy[j].h-20);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							if (enemy[j].hit == TRUE)
							{
								enemy[j].hitted = TRUE;
								enemy[j].HP -= g_Skilla[i].damage;
								if (enemy[j].HP < 1)
								{
									enemy[j].HP = 0;
									enemy[j].use = FALSE;
									enemy[j].pop = 0.0f;
								}
								PlaySound(SOUND_LABEL_SE_hit);
								D3DXVECTOR3 pos = enemy[j].damagepos;
								SetDamage(pos, g_Skilla[i].damage);
								enemy[j].hit = FALSE;		// 当たり判定フラグ
							}
						}
					}
				}
			}

			// チュートリアルの当たり判定
			{
				TUTORIAL *dummy = GetDummy();

				// 生きてるダミーと当たり判定をする
				if (dummy[0].use == TRUE)
				{
					BOOL ans = CollisionBB(g_Skilla[i].pos, g_Skilla[i].w/2, g_Skilla[i].h/2,
						dummy[0].pos, dummy[0].w, dummy[0].h);
					// 当たっている？
					if (ans == TRUE)
					{
						// 当たった時の処理
						PlaySound(SOUND_LABEL_SE_hit);
						g_Skilla[i].hit = TRUE;
					}
				}
			}

			skillCounta++;
		}
	}

	int skillCountb = 0;				// 処理したスキルの数

	for (int i = 0; i < SKILLB_MAX; i++)
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

			if (g_Skillb[i].countAnim == (ANIM_WAIT*ANIM_PATTERN_NUM_2*2))
			{
				g_Skillb[i].use = FALSE;
				g_Skillb[i].countAnim = 0;
			}

			// 当たり判定処理
			{
				ENEMY *enemy = GetEnemy();

				// エネミーの数分当たり判定を行う
				for (int j = 0; j < ENEMY_MAX; j++)
				{
					// 生きてるエネミーと当たり判定をする
					if (enemy[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Skillb[i].pos, g_Skillb[i].w, g_Skillb[i].h,
							enemy[j].pos, enemy[j].w, enemy[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							if (enemy[j].hit == TRUE)
							{
								enemy[j].hitted = TRUE;
								enemy[j].HP -= g_Skillb[i].damage;
								if (enemy[j].HP < 1)
								{
									enemy[j].HP = 0;
									enemy[j].use = FALSE;
									enemy[j].pop = 0.0f;
								}
								PlaySound(SOUND_LABEL_SE_hit);
								D3DXVECTOR3 pos = enemy[j].damagepos;
								SetDamage(pos, g_Skillb[i].damage);
								enemy[j].hit = FALSE;		// 当たり判定フラグ
							}
						}
					}
				}
			}

			// チュートリアルの当たり判定
			{
				TUTORIAL *dummy = GetDummy();

				// 生きてるダミーと当たり判定をする
				if (dummy[1].use == TRUE)
				{
					BOOL ans = CollisionBB(g_Skillb[i].pos, g_Skillb[i].w, g_Skillb[i].h,
						dummy[1].pos, dummy[1].w, dummy[1].h);
					// 当たっている？
					if (ans == TRUE)
					{
						// 当たった時の処理
						PlaySound(SOUND_LABEL_SE_hit);
						g_Skillb[i].hit = TRUE;
					}
				}
			}

			skillCountb++;
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSkill(void)
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

	for (int i = 0; i < SKILLA_MAX; i++)
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

	for (int i = 0; i < SKILLB_MAX; i++)
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

}


//=============================================================================
// スキル構造体の先頭アドレスを取得
//=============================================================================
SKILL *GetSkillA(void)
{
	return &g_Skilla[0];
}

SKILL *GetSkillB(void)
{
	return &g_Skillb[0];
}

//=============================================================================
// スキルAの使用設定
//=============================================================================
void SetSkillA(D3DXVECTOR3 pos, int damage)
{
	// もし未使用のスキル数が無かったら使わない( =これ以使ってないって事 )
	for (int i = 0; i < SKILLA_MAX; i++)
	{
		if (g_Skilla[i].use == FALSE)		// 未使用状態のスキルを見つける
		{
			g_Skilla[i].use = TRUE;			// 使用状態へ変更する
			g_Skilla[i].pos = pos;			// 座標をセット
			g_Skilla[i].damage = damage;
			PlaySound(SOUND_LABEL_SE_skill_a);
			
			ENEMY *enemy = GetEnemy();
			for (int j = 0; j < ENEMY_MAX; j++)
			{
				enemy[j].hit = TRUE;
			}
			return;							// 1発セットしたので終了する
		}
	}
}

//=============================================================================
// スキルBの使用設定
//=============================================================================
void SetSkillB(D3DXVECTOR3 pos, int damage)
{
	// もし未使用のスキル数が無かったら使わない( =これ以使ってないって事 )
	for (int i = 0; i < SKILLB_MAX; i++)
	{
		if (g_Skillb[i].use == FALSE)		// 未使用状態のスキルを見つける
		{
			g_Skillb[i].use = TRUE;			// 使用状態へ変更する
			g_Skillb[i].pos = pos;			// 座標をセット
			g_Skillb[i].damage = damage;
			PlaySound(SOUND_LABEL_SE_skill_b);
			
			ENEMY *enemy = GetEnemy();
			for (int j = 0; j < ENEMY_MAX; j++)
			{
				enemy[j].hit = TRUE;
			}
			return;							// 1発セットしたので終了する
		}
	}
}

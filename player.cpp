//=============================================================================
//

//
//=============================================================================
#include "player.h"
#include "sprite.h"
#include "input.h"
#include "bg.h"
#include "skill.h"
#include "enemy.h"
#include "collision.h"
#include "damage.h"
#include "sound.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(150/2)	// キャラサイズ
#define TEXTURE_HEIGHT				(150/2)	// 
#define TEXTURE_MAX					(6)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(8)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

#define TEXTURE_PATTERN_DIVIDE_X_2	(2)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_2	(8)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_2			(TEXTURE_PATTERN_DIVIDE_X_2*TEXTURE_PATTERN_DIVIDE_Y_2)	// アニメーションパターン数

#define ANIM_WAIT					(10)		// アニメーションの切り替わるWait値
#define ANIM_WAIT_2					(5)			// アニメーションの切り替わるWait値

// プレイヤーの画面内配置座標
#define PLAYER_DISP_X				(SCREEN_WIDTH/2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_HEIGHT)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawPlayerShadow(int no);
void DrawPlayerOffset(int no);
void DrawSkillAim(int no);
void DrawSkillSpell(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/char01.png",
	"data/TEXTURE/char02.png",
	"data/TEXTURE/char03.png",
	"data/TEXTURE/shadow.png",
	"data/TEXTURE/aim.png",
	"data/TEXTURE/spell.png",
};


static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static PLAYER	g_Player[PLAYER_MAX];	// プレイヤー構造体

static BOOL		g_goal;			// FALSE:チュートリアルのマークまで着いてない  TRUE:着いた

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
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


	// プレイヤー構造体の初期化
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].use = TRUE;
		g_Player[i].pos = D3DXVECTOR3(50.0f, 300.0f, 0.0f);	// プレイヤーの最初位置
		g_Player[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].w   = TEXTURE_WIDTH;
		g_Player[i].h   = TEXTURE_HEIGHT;
		g_Player[i].texNo = 0;

		g_Player[i].countAnim = 0;
		g_Player[i].patternAnim = 0;

		g_Player[i].move = D3DXVECTOR3(4.0f, 0.0f, 0.0f);		// 移動量

		g_Player[i].dir = CHAR_STD_DOWN;						// 下向きにしとくか
		g_Player[i].moving = FALSE;								// 移動中フラグ
		g_Player[i].patternAnim = g_Player[i].dir * TEXTURE_PATTERN_DIVIDE_X;

		g_Player[i].shadowtexNo = 3;
	
		g_Player[i].skillmove_a = 110.0f;						// スキルAの移動量
		g_Player[i].skillmove_b = 8.0f;							// スキルBの移動量
		g_Player[i].skilluse = FALSE;							// スキルチャージ中フラグ
		g_Player[i].skillusing = 0;
																// スキルの表示座標
		g_Player[i].skillpos_a = g_Player[i].skillpos_b = D3DXVECTOR3(g_Player[i].pos.x, g_Player[i].pos.y + g_Player[i].skillmove_a, 0.0f);
		g_Player[i].skilltexNo = 4;

		g_Player[i].spellcouAnim = 0;
		g_Player[i].spellpatAnim = 0;
		g_Player[i].spelltexNo = 5;

		g_Player[i].dashing = FALSE;							// ダッシュ中
		// 分身用
		for (int j = 0; j < PLAYER_OFFSET_CNT; j++)
		{
			g_Player[i].offset[j] = g_Player[i].pos;
		}
	
		// プレイヤーのパラメーター
		g_Player[i].HP = g_Player[i].HP_MAX = 100;
		g_Player[i].ST = g_Player[i].ST_MAX = 200;
		g_Player[i].atk = 50;
		g_Player[i].PerSecond = 0;
	
		g_Player[i].damagepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].damagemove = 30.0f;

		g_Player[i].hitted = FALSE;
		g_Player[i].hittedtime = 30;
		g_Player[i].alpha = 1.0f;

		// チュートリアルの時だけ行う初期化処理
		int mode = GetMode();
		if (mode == MODE_TUTORIAL)
		{
			g_Player[i].dir = CHAR_STD_DOWN;
			g_Player[i].pos = D3DXVECTOR3(700.0f, 200.0f, 0.0f);
			g_goal = FALSE;
		}
	}
	
	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
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
void UpdatePlayer(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// 生きてるプレイヤーだけ処理をする
		if (g_Player[i].use == TRUE)
		{
			// 地形との当たり判定用に座標のバックアップを取っておく
			g_Player[i].pos_old = g_Player[i].pos;

			// 分身用
			for (int j = PLAYER_OFFSET_CNT - 1; j > 0; j--)
			{
				g_Player[i].offset[j] = g_Player[i].offset[j - 1];
			}
			g_Player[i].offset[0] = g_Player[i].pos_old;


			// アニメーション 
			{
				g_Player[i].countAnim += 1.0f;
				if (g_Player[i].dashing == TRUE)
				{
					g_Player[i].countAnim += 1.0f;
				}
				if (g_Player[i].countAnim > ANIM_WAIT)
				{
					g_Player[i].countAnim = 0.0f;
					// パターンの切り替え
					g_Player[i].patternAnim = (g_Player[i].dir * TEXTURE_PATTERN_DIVIDE_X) + ((g_Player[i].patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
				}
			}
			// スペルのアニメーション
			{
				if (g_Player[i].skilluse == TRUE)
				{
					g_Player[i].spellcouAnim++;
					if ((g_Player[i].spellcouAnim % ANIM_WAIT_2) == 0)
					{
						// パターンの切り替え
						g_Player[i].spellpatAnim = (g_Player[i].spellpatAnim + 1) % ANIM_PATTERN_NUM_2;
					}
				}
			}
			// 攻撃される時のアニメーションを描画
			if (g_Player[i].hitted == TRUE)
			{
				g_Player[i].hittedtime--;
				if (g_Player[i].hittedtime % 6 == 0)
				{
					g_Player[i].alpha = 0.0f;
				}
				else if (g_Player[i].hittedtime % 3 == 0)
				{
					g_Player[i].alpha = 1.0f;
				}
				if (g_Player[i].hittedtime < 0)
				{
					g_Player[i].hitted = FALSE;
					g_Player[i].alpha = 1.0f;
					g_Player[i].hittedtime = 30;
				}
			}


			if (g_Player[i].moving == FALSE)
			{// プレイヤーの向きを調べる
				if (g_Player[i].dir == CHAR_DIR_DOWN)
				{
					g_Player[i].dir = CHAR_STD_DOWN;
				}
				else if (g_Player[i].dir == CHAR_DIR_UP)
				{
					g_Player[i].dir = CHAR_STD_UP;
				}
				else if (g_Player[i].dir == CHAR_DIR_RIGHT)
				{
					g_Player[i].dir = CHAR_STD_RIGHT;
				}
				else if (g_Player[i].dir == CHAR_DIR_LEFT)
				{
					g_Player[i].dir = CHAR_STD_LEFT;
				}
			}

			// 向きに合わせてスキルの表示位置を変える
			if (g_Player[i].dir == CHAR_DIR_DOWN)
			{
				g_Player[i].skillpos_a = g_Player[i].skillpos_b = D3DXVECTOR3(g_Player[i].pos.x, g_Player[i].pos.y + g_Player[i].skillmove_a, 0.0f);
			}
			else if (g_Player[i].dir == CHAR_DIR_UP)
			{
				g_Player[i].skillpos_a = g_Player[i].skillpos_b = D3DXVECTOR3(g_Player[i].pos.x, g_Player[i].pos.y - g_Player[i].skillmove_a, 0.0f);
			}
			else if (g_Player[i].dir == CHAR_DIR_RIGHT)
			{
				g_Player[i].skillpos_a = g_Player[i].skillpos_b = D3DXVECTOR3(g_Player[i].pos.x + g_Player[i].skillmove_a, g_Player[i].pos.y, 0.0f);
			}
			else if (g_Player[i].dir == CHAR_DIR_LEFT)
			{
				g_Player[i].skillpos_a = g_Player[i].skillpos_b = D3DXVECTOR3(g_Player[i].pos.x - g_Player[i].skillmove_a, g_Player[i].pos.y, 0.0f);
			}



			// キー入力で移動 
			{
				float speed = g_Player[i].move.x;

				g_Player[i].moving = FALSE;
				g_Player[i].dashing = FALSE;
				
				if (GetKeyboardPress(DIK_SPACE) && g_Player[i].ST > 0)
				{// N押したら、スタミナを消耗してダッシュする
					g_Player[i].ST -= 1;

					if (g_Player[i].ST < 0)
					{
						g_Player[i].ST = 0;
					}
					
					g_Player[i].PerSecond = 0;
					g_Player[i].dashing = TRUE;
					speed *= 2;
				}

				else
				{// N押してない時、スタミナを回復する
					g_Player[i].PerSecond++;
					if (g_Player[i].PerSecond > 150)
					{
						g_Player[i].ST += 1;
						if (g_Player[i].ST > g_Player[i].ST_MAX)
						{
							g_Player[i].ST = g_Player[i].ST_MAX;
						}
					}
				}

				for (int l = 0; l < SKILLA_MAX; l++)
				{
					SKILL *skill = GetSkillA();

					if (GetKeyboardPress(DIK_S) && g_Player[i].skilluse == FALSE && skill[l].use == FALSE)
					{
						g_Player[i].pos.y += speed;
						g_Player[i].dir = CHAR_DIR_DOWN;
						g_Player[i].moving = TRUE;

					}
					else if (GetKeyboardPress(DIK_W) && g_Player[i].skilluse == FALSE && skill[l].use == FALSE)
					{
						g_Player[i].pos.y -= speed;
						g_Player[i].dir = CHAR_DIR_UP;
						g_Player[i].moving = TRUE;
					}
					if (GetKeyboardPress(DIK_D) && g_Player[i].skilluse == FALSE && skill[l].use == FALSE)
					{
						g_Player[i].pos.x += speed;
						g_Player[i].dir = CHAR_DIR_RIGHT;
						g_Player[i].moving = TRUE;
					}
					else if (GetKeyboardPress(DIK_A) && g_Player[i].skilluse == FALSE && skill[l].use == FALSE)
					{
						g_Player[i].pos.x -= speed;
						g_Player[i].dir = CHAR_DIR_LEFT;
						g_Player[i].moving = TRUE;
					}
				}


				// MAP外チェック
				BG *bg = GetBG();

				if (g_Player[i].pos.x < 0.0f)
				{
					g_Player[i].pos.x = 0.0f;
				}

				if (g_Player[i].pos.x > bg->w)
				{
					g_Player[i].pos.x = bg->w;
				}

				if (g_Player[i].pos.y < 0.0f)
				{
					g_Player[i].pos.y = 0.0f;
				}

				if (g_Player[i].pos.y > bg->h)
				{
					g_Player[i].pos.y = bg->h;
				}


				// プレイヤーの立ち位置からMAPのスクロール座標を計算する
				bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
				if (bg->pos.x < 0) bg->pos.x = 0;
				if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

				bg->pos.y = g_Player[i].pos.y - PLAYER_DISP_Y;
				if (bg->pos.y < 0) bg->pos.y = 0;
				if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;


				// 移動が終わったらマップ上の障害物との当たり判定
				{
					int mode = GetMode();
					if (mode == MODE_GAME)
					{
						BG *obj = GetOBJ();
						// 障害物の数分当たり判定を行う
						for (int k = 0; k < OBJ_MAX; k++)
						{
							BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
								obj[k].pos, obj[k].w, obj[k].h);
							// 当たっている？
							if (ans == TRUE)
							{
								// 当たった時の処理
								g_Player[i].pos = g_Player[i].pos_old;
								PlaySound(SOUND_LABEL_SE_object);
							}
						}
					}
				}

				// 移動が終わったらエネミーとの当たり判定
				{
					ENEMY *enemy = GetEnemy();
					// エネミーの数分当たり判定を行う
					for (int j = 0; j < ENEMY_MAX; j++)
					{
						// 生きてるエネミーと当たり判定をする
						if (enemy[j].use == TRUE)
						{
							BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
								enemy[j].pos, enemy[j].w, enemy[j].h);
							// 当たっている？
							if (ans == TRUE)
							{
								// 当たった時の処理
								g_Player[i].pos = g_Player[i].pos_old;
							}
						}
					}
				}

				// チュートリアルの当たり判定
				{
					TUTORIAL *mark = GetMark();

					BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w/10, g_Player[i].h/10,
						mark->pos, mark->w/10, mark->h/10);
					// 当たっている？
					if (ans == TRUE)
					{
						// 当たった時の処理
						g_goal = TRUE;
					}
				}

				{
					TUTORIAL *dummy = GetDummy();
					// エネミーの数分当たり判定を行う
					for (int j = 0; j < DUMMY_MAX; j++)
					{
						// 生きてるエネミーと当たり判定をする
						if (dummy[j].use == TRUE)
						{
							BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
								dummy[j].pos, dummy[j].w, dummy[j].h);
							// 当たっている？
							if (ans == TRUE)
							{
								// 当たった時の処理
								g_Player[i].pos = g_Player[i].pos_old;
							}
						}
					}
				}


				// ダメージの表示位置
				{
					g_Player[i].damagepos = D3DXVECTOR3(g_Player[i].pos.x, g_Player[i].pos.y - g_Player[i].damagemove, 0.0f);
				}


				// 攻撃処理
				if (GetKeyboardTrigger(DIK_N))
				{// Space押したら、スキルAを使う
					g_Player[i].skillusing = 0;
					SKILL *skill = GetSkillB();
					// スキルBを使えてるかどうか
					for (int k = 0; k < SKILLB_MAX; k++)
					{
						if (skill[k].use == TRUE)
						{
							g_Player[i].skillusing++;
						}
					}
					// 使えてないならスキルAを発動
					if (g_Player[i].skillusing < 1)
					{
						D3DXVECTOR3 pos = g_Player[i].skillpos_a;
						int damage = g_Player[i].atk;
						SetSkillA(pos, damage);		// １発目
					}
				}

				if (GetKeyboardRepeat(DIK_M))
				{// M押しながら、攻撃範囲を調整
					g_Player[i].skillusing = 0;
					SKILL *skill = GetSkillA();
					// スキルAを使えてるかどうか
					for (int k = 0; k < SKILLA_MAX; k++)
					{
						if (skill[k].use == TRUE)
						{
							g_Player[i].skillusing++;
						}
					}
					// 使えてないならスキルBを発動
					if (g_Player[i].skillusing < 1)
					{
						g_Player[i].skilluse = TRUE;
						// 移動キー押したら、攻撃範囲を調整
						if (GetKeyboardPress(DIK_S))
						{
							g_Player[i].skillpos_b.y += g_Player[i].skillmove_b;
						}

						else if (GetKeyboardPress(DIK_W))
						{
							g_Player[i].skillpos_b.y -= g_Player[i].skillmove_b;
						}

						if (GetKeyboardPress(DIK_D))
						{
							g_Player[i].skillpos_b.x += g_Player[i].skillmove_b;
						}
						else if (GetKeyboardPress(DIK_A))
						{
							g_Player[i].skillpos_b.x -= g_Player[i].skillmove_b;
						}
					}
				}
				if (GetKeyboardRelease(DIK_M))
				{// M放したら、スキルBを使う
					g_Player[i].skillusing = 0;
					SKILL *skill = GetSkillA();
					// スキルAを使えてるかどうか
					for (int k = 0; k < SKILLA_MAX; k++)
					{
						if (skill[k].use == TRUE)
						{
							g_Player[i].skillusing++;
						}
					}
					// 使えてないならスキルBを発動
					if (g_Player[i].skillusing < 1)
					{
						D3DXVECTOR3 pos = g_Player[i].skillpos_b;
						int damage = g_Player[i].atk - 15;
						SetSkillB(pos, damage);		// １発目
						g_Player[i].skilluse = FALSE;
					}
				}
			}
		}

#ifdef _DEBUG
		// デバッグ表示
		PrintDebugProc("Player No%d  X:%f Y:%f\n", i, g_Player[i].pos.x, g_Player[i].pos.y);
		PrintDebugProc("Player No%d  ST:%d \n", i, g_Player[i].ST);
		PrintDebugProc("Player No%d  HP:%d \n", i, g_Player[i].HP);



#endif

	}
	
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
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

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)		// このプレイヤーが使われている？
		{									// Yes
			// プレイヤーの影を描画
			DrawPlayerShadow(i);

			if (g_Player[i].skilluse == TRUE)
			{// スキルの攻撃範囲とスペルを描画
				DrawSkillAim(i);
				DrawSkillSpell(i);
			}
			
			if (g_Player[i].dashing == TRUE)
			{// プレイヤーの分身を描画
				DrawPlayerOffset(i);
			}
			
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[i].texNo]);

			//プレイヤーの位置やテクスチャー座標を反映
			float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
			float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
			float pw = g_Player[i].w;		// プレイヤーの表示幅
			float ph = g_Player[i].h;		// プレイヤーの表示高さ

			// アニメーション用
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Player[i].alpha),
				g_Player[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// プレイヤー構造体の先頭アドレスを取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}


//=============================================================================
// 移動の結果を取得
//=============================================================================
BOOL GetGoal(void)
{
	return g_goal;
}


//=============================================================================
// プレイヤーの影を描画
//=============================================================================
void DrawPlayerShadow(int no)
{
	BG *bg = GetBG();

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].shadowtexNo]);

	//影の位置やテクスチャ座標を反映
	float px = g_Player[no].pos.x - bg->pos.x;		// 影の表示位置X
	float py = g_Player[no].pos.y - bg->pos.y + 5;	// 影の表示位置Y
	float pw = g_Player[no].w;		// 影の表示幅
	float ph = g_Player[no].h;		// 影の表示高さ

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, 1.0f, 1.0f, 1.0f, 1.0f,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

//=============================================================================
// プレイヤーの分身を描画
//=============================================================================
void DrawPlayerOffset(int no)
{
	BG *bg = GetBG();
	float alpha = 0.0f;

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].texNo]);

	for (int j = PLAYER_OFFSET_CNT - 1; j >= 0; j--)
	{
		//プレイヤーの位置やテクスチャー座標を反映
		float px = g_Player[no].offset[j].x - bg->pos.x;	// プレイヤーの表示位置X
		float py = g_Player[no].offset[j].y - bg->pos.y;	// プレイヤーの表示位置Y
		float pw = g_Player[no].w;		// プレイヤーの表示幅
		float ph = g_Player[no].h;		// プレイヤーの表示高さ

		// アニメーション用
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float tx = (float)(g_Player[no].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
		float ty = (float)(g_Player[no].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標


		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha),
			g_Player[no].rot.z);

		alpha += (1.0f / PLAYER_OFFSET_CNT);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}

//=============================================================================
// スキルの攻撃範囲を描画
//=============================================================================
void DrawSkillAim(int no)
{
	BG *bg = GetBG();

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].skilltexNo]);

	//スキルの位置やテクスチャ座標を反映
	float px = g_Player[no].skillpos_b.x - bg->pos.x;		// スキルの表示位置X
	float py = g_Player[no].skillpos_b.y - bg->pos.y;		// スキルの表示位置Y
	float pw = TEXTURE_WIDTH_2;			// スキルの表示幅
	float ph = TEXTURE_HEIGHT_2;		// スキルの表示高さ

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, 1.0f, 1.0f, 1.0f, 1.0f,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}


//=============================================================================
// スペルを描画
//=============================================================================
void DrawSkillSpell(int no)
{
	BG *bg = GetBG();

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].spelltexNo]);

	//位置やテクスチャ座標を反映
	float px = g_Player[no].pos.x - bg->pos.x;		// 表示位置X
	float py = g_Player[no].pos.y - bg->pos.y;		// 表示位置Y
	float pw = g_Player[no].w*3;		// 表示幅
	float ph = g_Player[no].h*3;		// 表示高さ

	// アニメーション用
	float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_2;	// テクスチャの幅
	float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_2;	// テクスチャの高さ
	float tx = (float)(g_Player[no].spellpatAnim % TEXTURE_PATTERN_DIVIDE_X_2) * tw;	// テクスチャの左上X座標
	float ty = (float)(g_Player[no].spellpatAnim / TEXTURE_PATTERN_DIVIDE_X_2) * th;	// テクスチャの左上Y座標

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

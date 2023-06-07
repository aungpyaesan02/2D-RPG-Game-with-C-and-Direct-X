//=============================================================================
//

//
//=============================================================================
#include "sprite.h"
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "bg.h"
#include "sound.h"
#include "player.h"
#include "skill.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(150)		// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(150)		// (SCREEN_HEIGHT)	// 背景サイズ
#define TEXTURE_WIDTH_3				(840)		// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT_3			(3871)		// (SCREEN_HEIGHT)	// 背景サイズ

#define TEXTURE_MAX					(7)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(5)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TEXTURE_PATTERN_DIVIDE_X_3	(4)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_3	(5)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_3			(TEXTURE_PATTERN_DIVIDE_X_3*TEXTURE_PATTERN_DIVIDE_Y_3)	// アニメーションパターン数

#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/mark.png",
	"data/TEXTURE/dummy.png",
	"data/TEXTURE/aurora.png",
	"data/TEXTURE/introduction.png",
	"data/TEXTURE/window_2.png",
	"data/TEXTURE/window_3.png",
	"data/TEXTURE/window_4.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static TUTORIAL	g_Window[WINDOW_MAX];
static TUTORIAL	g_Mark;
static TUTORIAL	g_Dummy[DUMMY_MAX];
static TUTORIAL	g_Aurora;

static int g_time;		// 時間に合わせて描画させる用(最初の時)
static int g_wait;		// 時間に合わせて描画させる用(毎回テクスチャを切り替わる時)
static int g_step;		// チュートリアルの進行段階を管理する用
static int g_sound;		// SEを鳴らすフラグ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTutorial(void)
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


	// 変数の初期化
	g_Aurora.use = TRUE;
	g_Aurora.w = 2200;
	g_Aurora.h = 1800;
	g_Aurora.pos = D3DXVECTOR3(700.0f, 400.0f, 0.0f);
	g_Aurora.rot = D3DXVECTOR3(0.0f, 0.0f, 3.14f);
	g_Aurora.texNo = 2;

	g_Aurora.countAnim = 0;
	g_Aurora.patternAnim = 0;


	g_Mark.use = FALSE;
	g_Mark.w = TEXTURE_WIDTH;
	g_Mark.h = TEXTURE_HEIGHT;
	g_Mark.pos = D3DXVECTOR3(700.0f, 600.0f, 0.0f);
	g_Mark.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Mark.texNo = 0;

	g_Mark.countAnim = 0;
	g_Mark.patternAnim = 0;

	for (int i = 0; i < DUMMY_MAX; i++)
	{
		g_Dummy[i].use = FALSE;
		g_Dummy[i].w = TEXTURE_WIDTH;
		g_Dummy[i].h = TEXTURE_HEIGHT;
		g_Dummy[i].pos = D3DXVECTOR3(500.0f*(i+1), 450.0f+(i*200.0f), 0.0f);
		g_Dummy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Dummy[i].texNo = 1;

		g_Dummy[i].countAnim = 0;
		g_Dummy[i].patternAnim = 0;
	}
	
	for (int i = 0; i < WINDOW_MAX; i++)
	{
		g_Window[i].use = FALSE;
		g_Window[i].w = TEXTURE_WIDTH_3;
		g_Window[i].h = TEXTURE_HEIGHT_3;
		g_Window[i].pos = D3DXVECTOR3(600.0f, 2435.5f, 0.0f);
		g_Window[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Window[i].texNo = 3;

		g_Window[i].countAnim = 0;
		g_Window[i].patternAnim = 0;
	}

	g_time = 0;
	g_wait = 0;
	g_sound = 0;
	g_step = 0;

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
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
void UpdateTutorial(void)
{
	// アニメーション  
	{
		// マークの処理
		g_Mark.countAnim++;
		if ((g_Mark.countAnim % ANIM_WAIT) == 0)
		{
			// パターンの切り替え
			g_Mark.patternAnim = (g_Mark.patternAnim + 1) % ANIM_PATTERN_NUM;
		}
		
		// オーロラの処理
		g_Aurora.countAnim++;
		if ((g_Aurora.countAnim % ANIM_WAIT) == 0)
		{
			// パターンの切り替え
			g_Aurora.patternAnim = (g_Aurora.patternAnim + 1) % ANIM_PATTERN_NUM_3;
		}
	}

	g_time++;
	//少し時間あけて描画させる
	if (g_time > 80 && g_step == 0)
	{
		for (int i = 0; i < WINDOW_MAX; i++)
		{
			g_Window[i].use = TRUE;
			g_sound = 0;		// SEを鳴らす
		}
		g_step++;
	}

	// SEを鳴らす処理
	{
		if (g_time > 80)
		{
			g_sound++;
		}

		if (g_sound == 1)
		{
			PlaySound(SOUND_LABEL_SE_system_03);
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
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


	// チュートリアル画面のウィンドウを描画
	{
		for (int i = 0; i < WINDOW_MAX; i++)
		{
			BG *bg = GetBG();

			if (g_Window[i].use == TRUE)
			{	// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Window[i].texNo]);

				//エネミーの位置やテクスチャー座標を反映
				float px = g_Window[i].pos.x - bg->pos.x;	// ウィンドウの表示位置X
				float py = g_Window[i].pos.y - bg->pos.y;	// ウィンドウの表示位置Y
				float pw = g_Window[i].w;		// ウィンドウの表示幅
				float ph = g_Window[i].h;		// ウィンドウの表示高さ

				float tw = 1.0f;	// テクスチャの幅
				float th = 1.0f;	// テクスチャの高さ
				float tx = 0.0f;	// テクスチャの左上X座標
				float ty = 0.0f;	// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					g_Window[i].rot.z);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}

	// オーロラを描画
	if (g_Aurora.use == TRUE)
	{
		BG *bg = GetBG();

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Aurora.texNo]);

		//エネミーの位置やテクスチャー座標を反映
		float px = g_Aurora.pos.x - bg->pos.x;	// ウィンドウの表示位置X
		float py = g_Aurora.pos.y - bg->pos.y;	// ウィンドウの表示位置Y
		float pw = g_Aurora.w;		// ウィンドウの表示幅
		float ph = g_Aurora.h;		// ウィンドウの表示高さ

		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_3;	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_3;	// テクスチャの高さ
		float tx = (float)(g_Aurora.patternAnim % TEXTURE_PATTERN_DIVIDE_X_3) * tw;	// テクスチャの左上X座標
		float ty = (float)(g_Aurora.patternAnim / TEXTURE_PATTERN_DIVIDE_X_3) * th;	// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),
			g_Aurora.rot.z);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}


}


//=============================================================================
// チュートリアル構造体の先頭アドレスを取得
//=============================================================================
TUTORIAL *GetDummy(void)
{
	return &g_Dummy[0];
}

TUTORIAL *GetMark(void)
{
	return &g_Mark;
}


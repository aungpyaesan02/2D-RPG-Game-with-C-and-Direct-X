//=============================================================================
//

//
//=============================================================================
#include "sprite.h"
#include "ui.h"
#include "bg.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(270)	// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(137)	// (SCREEN_HEIGHT)	// 背景サイズ
#define TEXTURE_WIDTH_3				(166)	// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT_3			(19)	// (SCREEN_HEIGHT)	// 背景サイズ

#define TEXTURE_MAX					(4)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

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
	"data/TEXTURE/ui_1.png",
	"data/TEXTURE/ui_hp.png",
	"data/TEXTURE/ui_st.png",
	"data/TEXTURE/ui_2.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static UI	g_UI[UI_MAX];


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUI(void)
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
	for (int i = 0; i < UI_MAX; i++)
	{
		g_UI[i].use = TRUE;
		g_UI[i].w = TEXTURE_WIDTH;
		g_UI[i].h = TEXTURE_HEIGHT;
		g_UI[i].pos = D3DXVECTOR3(32.0f, 540.0f, 0.0f);
		g_UI[i].texNo = i;

		g_UI[i].pw = TEXTURE_WIDTH;

		g_UI[i].countAnim = 0;
		g_UI[i].patternAnim = 0;
	}
	
	g_UI[1].w = TEXTURE_WIDTH_3;
	g_UI[1].h = TEXTURE_HEIGHT_3;
	g_UI[1].pos = D3DXVECTOR3(108.5f, 601.76f, 0.0f);
	g_UI[1].pw = TEXTURE_WIDTH_3;

	g_UI[2].w = TEXTURE_WIDTH_3;
	g_UI[2].h = TEXTURE_HEIGHT_3;
	g_UI[2].pos = D3DXVECTOR3(108.5f, 634.0f, 0.0f);
	g_UI[2].pw = TEXTURE_WIDTH_3;

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUI(void)
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
void UpdateUI(void)
{
	for (int i = 0; i < UI_MAX; i++)
	{
		if (g_UI[i].use == TRUE)
		{
			PLAYER *player = GetPlayer();
	
			for (int j = 0; j < PLAYER_MAX; j++)
			{
				// 直前のHPに合わせて描画
				if (i == 1)
				{
					g_UI[i].pw = g_UI[i].w * ((float)player[j].HP / (float)player[j].HP_MAX);
				}
				
				// 直前のSTに合わせて描画
				if (i == 2)
				{
					g_UI[i].pw = g_UI[i].w * ((float)player[j].ST / (float)player[j].ST_MAX);
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUI(void)
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



	// プレイヤーのステータスを描画
	{
		for (int i = 0; i < UI_MAX; i++)
		{
			if (g_UI[i].use == TRUE)
			{
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[i].texNo]);

				//エネミーの位置やテクスチャー座標を反映
				float px = g_UI[i].pos.x;	// ウィンドウの表示位置X
				float py = g_UI[i].pos.y;	// ウィンドウの表示位置Y
				float pw = g_UI[i].pw;		// ウィンドウの表示幅
				float ph = g_UI[i].h;		// ウィンドウの表示高さ

				float tw = 1.0f;	// テクスチャの幅
				float th = 1.0f;	// テクスチャの高さ
				float tx = 0.0f;	// テクスチャの左上X座標
				float ty = 0.0f;	// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteLTColor(g_VertexBuffer,
					px, py, pw, ph,
					tx, ty, tw, th,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}



}


//=============================================================================
// チュートリアル構造体の先頭アドレスを取得
//=============================================================================
UI *GetUI(void)
{
	return &g_UI[0];
}


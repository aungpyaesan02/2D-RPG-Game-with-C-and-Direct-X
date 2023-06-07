//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : GP11A132 AUNGPYAESAN
//
//=============================================================================
#include "title.h"
#include "sprite.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(2)				// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/title_bg.jpg",
	"data/TEXTURE/Stars.png",
};


static BOOL		g_Load = FALSE;		// 初期化を行ったかのフラグ
static TITLE	g_Title[TEXTURE_MAX];



// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
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
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		// 変数の初期化
		g_Title[i].w = TEXTURE_WIDTH;
		g_Title[i].h = TEXTURE_HEIGHT;
		g_Title[i].u = g_Title[i].v = 0.0f;
		g_Title[i].uw = g_Title[i].vh = 1.0f;
		g_Title[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Title[i].texNo = i;
	}

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
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
void UpdateTitle(void)
{
	g_Title[LAYER_2].v -= 0.005f;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	// 頂点バッファ設定
	for (int i = 0; i < TEXTURE_MAX; i++)
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

		// タイトルの背景を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Title[i].texNo]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				g_Title[i].pos.x, g_Title[i].pos.y, g_Title[i].w, g_Title[i].h,
				g_Title[i].u, g_Title[i].v, g_Title[i].uw, g_Title[i].vh,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

}






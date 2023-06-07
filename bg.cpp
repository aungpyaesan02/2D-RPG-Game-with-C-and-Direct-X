//=============================================================================
//

//
//=============================================================================
#include "bg.h"
#include "sprite.h"
#include "collision.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH_1				(SCREEN_WIDTH)		// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT_1			(SCREEN_HEIGHT)		// (SCREEN_HEIGHT)	// 背景サイズ
#define TEXTURE_WIDTH_2				(1400)		// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT_2			(4500)		// (SCREEN_HEIGHT)	// 背景サイズ

#define TEXTURE_MAX					(15)		// テクスチャの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/BG.png",
	"data/TEXTURE/block_bg.png",
	"data/TEXTURE/mountain_3.png",
	"data/TEXTURE/mountain_2.png",
	"data/TEXTURE/mountain_1.png",
	"data/TEXTURE/ruin.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static BG	g_BG;
static BG	g_OBJ[OBJ_MAX];


// 初期化処理
//=============================================================================
HRESULT InitBG(void)
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
	g_BG.w     = TEXTURE_WIDTH_1;
	g_BG.h     = TEXTURE_HEIGHT_1;
	g_BG.pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_BG.texNo = 0;

	g_OBJ[0].w = 200;
	g_OBJ[0].h = 200;
	g_OBJ[0].pos = D3DXVECTOR3(100.0f, 440.0f, 0.0f);
	g_OBJ[0].texNo = 1;

	g_OBJ[1].w = 200;
	g_OBJ[1].h = 200;
	g_OBJ[1].pos = D3DXVECTOR3(860.0f, 440.0f, 0.0f);
	g_OBJ[1].texNo = 1;

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBG(void)
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
void UpdateBG(void)
{
	int mode = GetMode();
	if (mode == MODE_TUTORIAL)
	{
		g_BG.texNo = 14;
		g_BG.w = TEXTURE_WIDTH_2;
		g_BG.h = TEXTURE_HEIGHT_2;
	}

	//for (int i = 0; i < OBJ_MAX; i++)
	//{// プレイヤーとの当たり判定
	//	
	//	PLAYER *player = GetPlayer();
	//	// 障害物の数分当たり判定を行う
	//	for (int j = 0; j < PLAYER_MAX; j++)
	//	{
	//		BOOL ans = CollisionBB(g_OBJ[i].pos, g_OBJ[i].w, g_OBJ[i].h,
	//			player[j].pos, player[j].w, player[j].h);
	//		// 当たっている？
	//		if (ans == TRUE)
	//		{
	//			// 当たった時の処理
	//			player[j].pos = player[i].pos_old;
	//		}
	//	}
	//}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void)
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


	// MAPの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BG.texNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			-g_BG.pos.x, -g_BG.pos.y, g_BG.w, g_BG.h,
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// MAPの障害物を描画
	{
		int mode = GetMode();
		if (mode == MODE_GAME)
		{
			for (int i = 0; i < OBJ_MAX; i++)
			{
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_OBJ[i].texNo]);

				//エネミーの位置やテクスチャー座標を反映
				float px = g_OBJ[i].pos.x - g_BG.pos.x;		// 障害物の表示位置X
				float py = g_OBJ[i].pos.y - g_BG.pos.y;		// 障害物の表示位置Y
				float pw = g_OBJ[i].w;		// 障害物の表示幅
				float ph = g_OBJ[i].h;		// 障害物の表示高さ

				float tw = 1.0f;	// テクスチャの幅
				float th = 1.0f;	// テクスチャの高さ
				float tx = 0.0f;	// テクスチャの左上X座標
				float ty = 0.0f;	// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					0.0f);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}


}


//=============================================================================
// BG構造体の先頭アドレスを取得
//=============================================================================
BG *GetBG(void)
{
	return &g_BG;
}

BG *GetOBJ(void)
{
	return &g_OBJ[0];
}





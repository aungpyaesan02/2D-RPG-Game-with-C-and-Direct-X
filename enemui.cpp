//=============================================================================
//

//
//=============================================================================
#include "sprite.h"
#include "enemui.h"
#include "bg.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_MAX				(ENEMY_MAX)		// ウィンドウの数

#define TEXTURE_WIDTH				(120)	// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(15)	// (SCREEN_HEIGHT)	// 背景サイズ

#define TEXTURE_MAX					(3)		// テクスチャの数

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
	"data/TEXTURE/enemui_1.png",
	"data/TEXTURE/enemui_hp.png",
	"data/TEXTURE/enemui_2.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static ENEMUI	g_UI_1[UI_MAX];
static ENEMUI	g_UI_hp[UI_MAX];
static ENEMUI	g_UI_2[UI_MAX];


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemUI(void)
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
		g_UI_1[i].use = FALSE;
		g_UI_1[i].w = TEXTURE_WIDTH;
		g_UI_1[i].h = TEXTURE_HEIGHT;
		g_UI_1[i].pos = D3DXVECTOR3(32.0f, 540.0f, 0.0f);
		g_UI_1[i].texNo = 0;
		g_UI_1[i].pw = TEXTURE_WIDTH;
		g_UI_1[i].countAnim = 0;
		g_UI_1[i].patternAnim = 0;


		g_UI_hp[i].use = FALSE;
		g_UI_hp[i].w = TEXTURE_WIDTH;
		g_UI_hp[i].h = TEXTURE_HEIGHT;
		g_UI_hp[i].pos = D3DXVECTOR3(32.0f, 540.0f, 0.0f);
		g_UI_hp[i].texNo = 1;
		g_UI_hp[i].pw = TEXTURE_WIDTH;
		g_UI_hp[i].countAnim = 0;
		g_UI_hp[i].patternAnim = 0;


		g_UI_2[i].use = FALSE;
		g_UI_2[i].w = TEXTURE_WIDTH;
		g_UI_2[i].h = TEXTURE_HEIGHT;
		g_UI_2[i].pos = D3DXVECTOR3(32.0f, 540.0f, 0.0f);
		g_UI_2[i].texNo = 2;
		g_UI_2[i].pw = TEXTURE_WIDTH;
		g_UI_2[i].countAnim = 0;
		g_UI_2[i].patternAnim = 0;
	}
	
	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemUI(void)
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
void UpdateEnemUI(void)
{
	for (int i = 0; i < UI_MAX; i++)
	{
		ENEMY *enemy = GetEnemy();
		
		// hpの位置をセットする
		g_UI_1[i].pos = D3DXVECTOR3(enemy[i].pos.x - (enemy[i].w/2), enemy[i].pos.y - (enemy[i].h/2) - 20, 0.0f);
		g_UI_hp[i].pos = D3DXVECTOR3(enemy[i].pos.x - (enemy[i].w/2), enemy[i].pos.y - (enemy[i].h/2) - 20, 0.0f);
		g_UI_2[i].pos = D3DXVECTOR3(enemy[i].pos.x - (enemy[i].w/2), enemy[i].pos.y - (enemy[i].h/2) - 20, 0.0f);

		// HPが減っている時だけ表示
		if (enemy[i].HP < enemy[i].HP_MAX)
		{
			g_UI_1[i].use = TRUE;
			g_UI_hp[i].use = TRUE;
			g_UI_2[i].use = TRUE;
		}
	
		// 直前のHPに合わせて描画
		if (g_UI_hp[i].use == TRUE)
		{
			g_UI_hp[i].pw = g_UI_hp[i].w * ((float)enemy[i].HP / (float)enemy[i].HP_MAX);
		}
	
		if (enemy[i].HP < 1)
		{
			g_UI_1[i].use = FALSE;
			g_UI_hp[i].use = FALSE;
			g_UI_2[i].use = FALSE;
		}

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemUI(void)
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



	// エネミーのHPを描画
	{
		for (int i = 0; i < UI_MAX; i++)
		{
			BG *bg = GetBG();

			if (g_UI_1[i].use == TRUE)
			{
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI_1[i].texNo]);

				//エネミーの位置やテクスチャー座標を反映
				float px = g_UI_1[i].pos.x - bg->pos.x;	// ウィンドウの表示位置X
				float py = g_UI_1[i].pos.y - bg->pos.y;	// ウィンドウの表示位置Y
				float pw = g_UI_1[i].pw;		// ウィンドウの表示幅
				float ph = g_UI_1[i].h;		// ウィンドウの表示高さ

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
		

			if (g_UI_hp[i].use == TRUE)
			{
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI_hp[i].texNo]);

				//エネミーの位置やテクスチャー座標を反映
				float px = g_UI_hp[i].pos.x - bg->pos.x;	// ウィンドウの表示位置X
				float py = g_UI_hp[i].pos.y - bg->pos.y;	// ウィンドウの表示位置Y
				float pw = g_UI_hp[i].pw;		// ウィンドウの表示幅
				float ph = g_UI_hp[i].h;		// ウィンドウの表示高さ

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


			if (g_UI_2[i].use == TRUE)
			{
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI_2[i].texNo]);

				//エネミーの位置やテクスチャー座標を反映
				float px = g_UI_2[i].pos.x - bg->pos.x;	// ウィンドウの表示位置X
				float py = g_UI_2[i].pos.y - bg->pos.y;	// ウィンドウの表示位置Y
				float pw = g_UI_2[i].pw;		// ウィンドウの表示幅
				float ph = g_UI_2[i].h;		// ウィンドウの表示高さ

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


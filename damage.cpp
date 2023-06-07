//=============================================================================
//
//=============================================================================
#include "damage.h"
#include "sprite.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(DAMAGE_DW)	// 数字のサイズ
#define TEXTURE_HEIGHT				(DAMAGE_DH)	// 
#define TEXTURE_MAX					(1)			// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/TEXTURE/number16x32.png",
};

static BOOL		g_Load = FALSE;		// 初期化を行ったかのフラグ
static DAMAGE	g_Damage[DAMAGE_MAX];


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDamage(void)
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
	for (int i = 0; i < DAMAGE_MAX; i++)
	{
		g_Damage[i].w = TEXTURE_WIDTH;
		g_Damage[i].h = TEXTURE_HEIGHT;
		g_Damage[i].pos = D3DXVECTOR3(DAMAGE_DX, DAMAGE_DY, 0.0f);
		g_Damage[i].texNo = 0;
		g_Damage[i].use = FALSE;
		g_Damage[i].damage = 0;	// ダメージを初期化
		g_Damage[i].move = -1.0f;
		g_Damage[i].alpha = 1.0f;
		g_Damage[i].time = 0;
	}

	g_Load = TRUE;		// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDamage(void)
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
void UpdateDamage(void)
{
	for (int i = 0; i < DAMAGE_MAX; i++)
	{
		float speed = g_Damage[i].move;
	
		if (g_Damage[i].use == TRUE)
		{
			g_Damage[i].time++;
			g_Damage[i].pos.y += speed;
			
			if (g_Damage[i].time > 30)
			{
				g_Damage[i].alpha -= 0.1f;
		
				if (g_Damage[i].alpha < 0.0f)
				{
					g_Damage[i].use = FALSE;
					g_Damage[i].time = 0;
					g_Damage[i].alpha = 1.0f;
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDamage(void)
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

	for (int j = 0; j < DAMAGE_MAX; j++)
	{
		if (g_Damage[j].use == TRUE)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Damage[j].texNo]);

			// 桁数分処理する
			int number = g_Damage[j].damage;

			for (int i = 0; i < DAMAGE_DIGIT; i++)
			{
				// 今回表示する桁の数字
				float x = (float)(number % 10);

				// 次の桁へ
				number /= 10;

				// スコアの位置やテクスチャー座標を反映
				float px = (g_Damage[j].pos.x - bg->pos.x) - g_Damage[i].w*i;	// スコアの表示位置X
				float py = g_Damage[j].pos.y - bg->pos.y;						// スコアの表示位置Y
				float pw = g_Damage[j].w;						// スコアの表示幅
				float ph = g_Damage[j].h;						// スコアの表示高さ

				float tw = 1.0f / 10;					// テクスチャの幅
				float th = 1.0f / 1;					// テクスチャの高さ
				float tx = x * tw;						// テクスチャの左上X座標
				float ty = 0.0f;						// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteLTColor(g_VertexBuffer,
					px, py, pw, ph,
					tx, ty, tw, th,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Damage[j].alpha));

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);

			}
		}
	}
}


//=============================================================================
// スコアを加算する
// 引数:add :追加する点数。マイナスも可能
//=============================================================================
void AddDamage(int add, int no)
{
	g_Damage[no].damage += add;
	if (g_Damage[no].damage > DAMAGE_MAX)
	{
		g_Damage[no].damage = DAMAGE_MAX;
	}
}


int GetDamage(void)
{
	return g_Damage[0].damage;
}


//=============================================================================
// ダメージの使用設定
//=============================================================================
void SetDamage(D3DXVECTOR3 pos, int damage)
{
	// もし未使用のダメージ数が無かったら使わない( =これ以使ってないって事 )
	for (int i = 0; i < DAMAGE_MAX; i++)
	{
		if (g_Damage[i].use == FALSE)		// 未使用状態のダメージを見つける
		{
			g_Damage[i].use = TRUE;			// 使用状態へ変更する
			g_Damage[i].pos = pos;			// 座標をセット
			g_Damage[i].damage = damage;	// ダメージをセット
			
			return;							// 桁数分セットしたので終了する
		}
	}
}


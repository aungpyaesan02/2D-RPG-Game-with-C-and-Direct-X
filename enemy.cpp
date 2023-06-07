//=============================================================================
//

//
//=============================================================================
#include "enemy.h"
#include "enemyskill.h"
#include "sprite.h"
#include "input.h"
#include "bg.h"
#include "player.h"
#include "collision.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(100/1)	// キャラサイズ
#define TEXTURE_HEIGHT				(100/1)	// 

#define TEXTURE_WIDTH_3				(200)	// キャラサイズ
#define TEXTURE_HEIGHT_3			(200)	// 

#define TEXTURE_MAX					(6)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(8)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

#define TEXTURE_PATTERN_DIVIDE_X_2	(2)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_2	(8)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_2			(TEXTURE_PATTERN_DIVIDE_X_2*TEXTURE_PATTERN_DIVIDE_Y_2)	// アニメーションパターン数

#define ANIM_WAIT					(8)		// アニメーションの切り替わるWait値


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawEnemyShadow(int no);
void DrawEnemySpell(int no);
void DrawEnemyAim(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/enemy01.png",
	"data/TEXTURE/enemy02.png",
	"data/TEXTURE/enemy03.png",
	"data/TEXTURE/shadow.png",
	"data/TEXTURE/spell.png",
	"data/TEXTURE/enemyaim.png",
};


static LINEAR_INTERPOLATION g_MoveTbl0[] = {
	//座標									回転率							拡大率							時間
	{ D3DXVECTOR3(160.0f,1236.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(160.0f,1040.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(464.0f,1040.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(160.0f,1040.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
};

static LINEAR_INTERPOLATION g_MoveTbl1[] = {
	//座標									回転率							拡大率							時間
	{ D3DXVECTOR3(1216.0f,1018.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(1216.0f,1284.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
};

static LINEAR_INTERPOLATION g_MoveTbl2[] = {
	//座標									回転率							拡大率							時間
	{ D3DXVECTOR3(540.0f,1432.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(540.0f,1664.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.010f },
	{ D3DXVECTOR3(984.0f,1664.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(984.0f,1432.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.010f },
};

static LINEAR_INTERPOLATION g_MoveTbl3[] = {
	//座標									回転率							拡大率							時間
	{ D3DXVECTOR3(2270.0f,2076.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.006f },
	{ D3DXVECTOR3(2970.0f,2076.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.006f },
};

static LINEAR_INTERPOLATION g_MoveTbl4[] = {
	//座標									回転率							拡大率							時間
	{ D3DXVECTOR3(1560.0f,2164.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(1560.0f,2412.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
};

static LINEAR_INTERPOLATION *g_MoveTblAdr[] = 
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,
	g_MoveTbl3,
	g_MoveTbl4,
};


static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static ENEMY	g_Enemy[ENEMY_MAX];		// エネミー構造体

static BOOL		skillcharge;			// スキルチャージ中フラグ

static int		spellcouAnim;			// スペルのアニメーションカウント
static int		spellpatAnim;			// スペルのアニメーションパターン
static int		spelltexNo;				// スペルのテクスチャ番号

static D3DXVECTOR3	aimpos ;			// 攻撃範囲の座標
static int			aimtime;			// 攻撃範囲の存在時間を管理ため
static int			aimtexNo;			// 攻撃範囲のテクスチャ番号

static int			musicup;			// ミュージックアップフラグ


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
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


	// エネミー構造体の初期化
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_Enemy[i].use = TRUE;
		g_Enemy[i].pos = D3DXVECTOR3(2000, 100.0f + i*50, 0.0f);	// 表示位置の初期化
		g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Enemy[i].w   = TEXTURE_WIDTH;
		g_Enemy[i].h   = TEXTURE_HEIGHT;
		g_Enemy[i].texNo = 0;

		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		g_Enemy[i].move = D3DXVECTOR3(4.0f, 0.0f, 0.0f);
		g_Enemy[i].pop = 0.0f;
		g_Enemy[i].homing = FALSE;

		g_Enemy[i].dir = ENEM_STD_DOWN;							// 下向きにしとくか
		g_Enemy[i].moving = FALSE;								// 移動中フラグ
		g_Enemy[i].patternAnim = g_Enemy[i].dir * TEXTURE_PATTERN_DIVIDE_X;
		g_Enemy[i].shadowtexNo = 3;

		g_Enemy[i].HP = g_Enemy[i].HP_MAX = 80;
		g_Enemy[i].atk = 10;

		g_Enemy[i].skillmove = 110.0f;						// スキルAの移動量
		g_Enemy[i].skillpos = D3DXVECTOR3(g_Enemy[i].pos.x, g_Enemy[i].pos.y + g_Enemy[i].skillmove, 0.0f);	// スキルの移動量
		g_Enemy[i].PerSecond = 100;
		
		g_Enemy[i].damagepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].damagemove = 30.0f;

		g_Enemy[i].hit = FALSE;			// 当たり判定を行うフラグ

		g_Enemy[i].hitted = FALSE;		// 攻撃されるフラグ
		g_Enemy[i].hittedtime = 30;
		g_Enemy[i].alpha = 1.0f;

		// 行動パターンを初期化
		g_Enemy[i].time		= 0.0f;			// 線形補間用
		g_Enemy[i].moveTblNo= 0;			// データテーブル
		g_Enemy[i].tblMax	= sizeof(g_MoveTbl0) / sizeof(LINEAR_INTERPOLATION);// 線形補間用

	}

	// ２番目の奴の行動パターンをセット
	g_Enemy[1].moveTblNo = 1;				// データテーブル
	g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(LINEAR_INTERPOLATION);	// 線形補間用

	// ３番目の奴の行動パターンをセット
	g_Enemy[2].moveTblNo = 2;				// データテーブル
	g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(LINEAR_INTERPOLATION);	// 線形補間用

	// ４番目の奴の行動パターンをセット
	g_Enemy[3].moveTblNo = 3;				// データテーブル
	g_Enemy[3].tblMax = sizeof(g_MoveTbl3) / sizeof(LINEAR_INTERPOLATION);	// 線形補間用

	// ５番目の奴の行動パターンをセット
	g_Enemy[4].moveTblNo = 4;				// データテーブル
	g_Enemy[4].tblMax = sizeof(g_MoveTbl4) / sizeof(LINEAR_INTERPOLATION);	// 線形補間用

	// 6番目の奴
	g_Enemy[5].use = FALSE;
	g_Enemy[5].pop = 1.0f;
	g_Enemy[5].pos = D3DXVECTOR3(616, -100.0f, 0.0f);	// 表示位置の初期化

	// 7番目の奴
	g_Enemy[6].use = FALSE;
	g_Enemy[6].pop = 1.0f;
	g_Enemy[6].pos = D3DXVECTOR3(g_Enemy[5].pos.x+150.0f, 0.0f, 0.0f);	// 表示位置の初期化

	// 8番目の奴
	g_Enemy[7].use = FALSE;
	g_Enemy[7].pop = 1.0f;
	g_Enemy[7].pos = D3DXVECTOR3(g_Enemy[6].pos.x+150.0f, -400.0f, 0.0f);	// 表示位置の初期化

	// 9番目の奴
	g_Enemy[8].use = FALSE;
	g_Enemy[8].pop = 1.0f;
	g_Enemy[8].pos = D3DXVECTOR3(2220, 1588, 0.0f);	// 表示位置の初期化

	// 10番目の奴
	g_Enemy[9].use = FALSE;
	g_Enemy[9].pop = 1.0f;
	g_Enemy[9].pos = D3DXVECTOR3(g_Enemy[8].pos.x+200.0f, 1588, 0.0f);	// 表示位置の初期化

	// 11番目の奴
	g_Enemy[10].use = FALSE;
	g_Enemy[10].pop = 1.0f;
	g_Enemy[10].pos = D3DXVECTOR3(1436, 2164, 0.0f);						// 表示位置の初期化
	g_Enemy[10].dir = ENEM_STD_RIGHT;										// 右向きにしとく
	g_Enemy[10].texNo = 1;

	// 12番目の奴
	g_Enemy[11].use = FALSE;
	g_Enemy[11].pop = 1.0f;
	g_Enemy[11].pos = D3DXVECTOR3(g_Enemy[10].pos.x, g_Enemy[10].pos.y+120.0f, 0.0f);		// 表示位置の初期化
	g_Enemy[11].dir = ENEM_STD_RIGHT;										// 右向きにしとく
	g_Enemy[11].texNo = 1;

	// 13番目の奴
	g_Enemy[12].use = FALSE;
	g_Enemy[12].pop = 1.0f;
	g_Enemy[12].pos = D3DXVECTOR3(g_Enemy[10].pos.x, g_Enemy[11].pos.y + 120.0f, 0.0f);		// 表示位置の初期化
	g_Enemy[12].dir = ENEM_STD_RIGHT;										// 右向きにしとく
	g_Enemy[12].texNo = 1;

	// 14番目の奴
	g_Enemy[13].use = FALSE;
	g_Enemy[13].pop = 1.0f;
	g_Enemy[13].pos = D3DXVECTOR3(3270, 2108, 0.0f);						// 表示位置の初期化
	g_Enemy[13].dir = ENEM_STD_LEFT;										// 左向きにしとく
	g_Enemy[13].texNo = 1;

	// 15番目の奴
	g_Enemy[14].use = FALSE;
	g_Enemy[14].pop = 1.0f;
	g_Enemy[14].pos = D3DXVECTOR3(g_Enemy[13].pos.x, g_Enemy[13].pos.y+120.0f, 0.0f);						// 表示位置の初期化
	g_Enemy[14].dir = ENEM_STD_LEFT;										// 左向きにしとく
	g_Enemy[14].texNo = 1;

	// 16番目の奴
	g_Enemy[15].use = FALSE;
	g_Enemy[15].pop = 1.0f;
	g_Enemy[15].pos = D3DXVECTOR3(g_Enemy[13].pos.x, g_Enemy[14].pos.y+120.0f, 0.0f);						// 表示位置の初期化
	g_Enemy[15].dir = ENEM_STD_LEFT;										// 左向きにしとく
	g_Enemy[15].texNo = 1;

	// BOSS
	g_Enemy[16].use = FALSE;
	g_Enemy[16].pop = 1.0f;
	g_Enemy[16].pos = D3DXVECTOR3(4324, 1792, 0.0f);						// 表示位置の初期化
	g_Enemy[16].dir = ENEM_STD_DOWN;										// 左向きにしとく
	g_Enemy[16].texNo = 2;
	g_Enemy[16].HP = g_Enemy[16].HP_MAX = 200;

	skillcharge = FALSE;
	aimpos = D3DXVECTOR3(4.0f, 0.0f, 0.0f);
	aimtime = 0;
	aimtexNo = 5;

	spelltexNo = 4;
	spellcouAnim = 0;
	spellpatAnim = 0;

	musicup = 0;
	
	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
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
void UpdateEnemy(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE)	// このエネミーが使われている？
		{							// Yes
			PLAYER *player = GetPlayer();
		
			// 地形との当たり判定用に座標のバックアップを取っておく
			g_Enemy[i].pos_old = g_Enemy[i].pos;
			
			// 移動処理 
			{
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// １番名～５番目のやつの処理
					if (i < 5)
					{
						// プレイヤーとの距離を測る
						D3DXVECTOR3 temp = player[j].pos - g_Enemy[i].pos;
						float lenSq = D3DXVec3LengthSq(&temp);	// 2点間の距離（2乗した物）

						// もし距離は150000以上の場合
						if (lenSq > 150000.0f)
						{// 行動テーブルに従って座標移動（線形補間）
							int nowNo = (int)g_Enemy[i].time;			// 整数分であるテーブル番号を取り出している
							int maxNo = g_Enemy[i].tblMax;				// 登録テーブル数を数えている
							int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
							LINEAR_INTERPOLATION *tbl = g_MoveTblAdr[g_Enemy[i].moveTblNo];	// 行動テーブルのアドレスを取得
							D3DXVECTOR3	pos = tbl[nextNo].pos - tbl[nowNo].pos;	// XYZ移動量を計算している
							D3DXVECTOR3	rot = tbl[nextNo].rot - tbl[nowNo].rot;	// XYZ回転量を計算している
							D3DXVECTOR3	scl = tbl[nextNo].scl - tbl[nowNo].scl;	// XYZ拡大率を計算している
							float nowTime = g_Enemy[i].time - nowNo;	// 時間部分である少数を取り出している
							pos *= nowTime;								// 現在の移動量を計算している
							rot *= nowTime;								// 現在の回転量を計算している
							scl *= nowTime;								// 現在の拡大率を計算している

							// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
							g_Enemy[i].pos = tbl[nowNo].pos + pos;

							// 計算して求めた回転量を現在の移動テーブルに足している
							g_Enemy[i].rot = tbl[nowNo].rot + rot;

							// 計算して求めた拡大率を現在の移動テーブルに足している
							g_Enemy[i].scl = tbl[nowNo].scl + scl;
							g_Enemy[i].w = TEXTURE_WIDTH * g_Enemy[i].scl.x;
							g_Enemy[i].h = TEXTURE_HEIGHT * g_Enemy[i].scl.y;

							// frameを使て時間経過処理をする
							g_Enemy[i].time += tbl[nowNo].time;			// 時間を進めている
							if ((int)g_Enemy[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
							{
								g_Enemy[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
							}
						}

						// エネミーのホーミング処理
						else if (lenSq < 150000.0f)
						{// もし距離は150000以内ならホーミングする
							D3DXVECTOR3 pos = player[j].pos - g_Enemy[i].pos;	// 自分とプレイヤーの差分を求めている
							float angle = atan2f(pos.y, pos.x) + D3DX_PI;		// その差分を使って角度を求めている
							float speed = -4.0f;								// スピードはちょっと遅くしてみる

							g_Enemy[i].pos.x += cosf(angle) * speed;			// angleの方向へ移動
							g_Enemy[i].pos.y += sinf(angle) * speed;			// angleの方向へ移動
						}
					}
				
					// ６番目～8番目のやつの処理
					else if (i > 4 && i < 8)
					{
						float move = 4.0f;								//スピード

						if (g_Enemy[i].pos.y < 780.0f && g_Enemy[i].homing == FALSE)
						{
							g_Enemy[i].pos.y += move;
							
							if (g_Enemy[i].pos.y > 779.0f)
							{
								g_Enemy[i].homing = TRUE;
							}
						}
						// 指定された場所に移動したらホーミングされる
						if (g_Enemy[i].homing == TRUE)
						{
							D3DXVECTOR3 pos = player[j].pos - g_Enemy[i].pos;	// 自分とプレイヤーの差分を求めている
							float angle = atan2f(pos.y, pos.x) + D3DX_PI;		// その差分を使って角度を求めている
							float speed = -4.0f;								// スピードはちょっと遅くしてみる

							g_Enemy[i].pos.x += cosf(angle) * speed;			// angleの方向へ移動
							g_Enemy[i].pos.y += sinf(angle) * speed;			// angleの方向へ移動
						}
					}
				
					// 9番目～10番目のやつの処理
					else if (i > 7 && i < 10)
					{
						float move = 4.0f;								//スピード

						if (g_Enemy[i].pos.x > 1300.0f && g_Enemy[i].homing == FALSE)
						{
							g_Enemy[i].pos.x -= move;
							
							if (g_Enemy[i].pos.x < 1301.0f)
							{
								g_Enemy[i].homing = TRUE;
							}
						}
						// 指定された場所に移動したらホーミングされる
						if (g_Enemy[i].homing == TRUE)
						{
							D3DXVECTOR3 pos = player[j].pos - g_Enemy[i].pos;	// 自分とプレイヤーの差分を求めている
							float angle = atan2f(pos.y, pos.x) + D3DX_PI;		// その差分を使って角度を求めている
							float speed = -4.0f;								// スピードはちょっと遅くしてみる

							g_Enemy[i].pos.x += cosf(angle) * speed;			// angleの方向へ移動
							g_Enemy[i].pos.y += sinf(angle) * speed;			// angleの方向へ移動
						}
					}
				}
			}

			// 移動が終わったらプレイヤーとの当たり判定
			{
				// プレイヤーの数分当たり判定を行う
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// 生きてるプレイヤーと当たり判定をする
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
							player[j].pos, player[j].w, player[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							g_Enemy[i].pos = g_Enemy[i].pos_old;
						}
					}
				}
			}

			// 移動が終わったら他のエネミーとの当たり判定
			{
				// エネミーの数分当たり判定を行う
				for (int k = 0; k < ENEMY_MAX; k++)
				{
					// 生きてるエネミーと当たり判定をする
					if (g_Enemy[k].use == TRUE && k != i)
					{
						BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
							g_Enemy[k].pos, g_Enemy[k].w, g_Enemy[k].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							g_Enemy[i].pos = g_Enemy[i].pos_old;
						}
					}
				}
			}

			// 移動が終わったらMAP上のOBJとの当たり判定
			{
				// エネミーの数分当たり判定を行う
				for (int k = 0; k < ENEMY_MAX; k++)
				{
					BG *obj = GetOBJ();
					// 障害物の数分当たり判定を行う
					for (int l = 0; l < OBJ_MAX; l++)
					{
						BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
							obj[l].pos, obj[l].w, obj[l].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							g_Enemy[i].pos = g_Enemy[i].pos_old;
						}
					}
				}
			}


			// アニメーション
			{
				g_Enemy[i].countAnim++;
				if (g_Enemy[i].countAnim > ANIM_WAIT)
				{
					g_Enemy[i].countAnim = 0;
					// パターンの切り替え
					g_Enemy[i].patternAnim = (g_Enemy[i].dir * TEXTURE_PATTERN_DIVIDE_X) + ((g_Enemy[i].patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
				}
			}
			// 攻撃される時のアニメーションを描画
			if (g_Enemy[i].hitted == TRUE)
			{
				g_Enemy[i].hittedtime--;
				if (g_Enemy[i].hittedtime % 6 == 0)
				{
					g_Enemy[i].alpha = 0.0f;
				}
				else if (g_Enemy[i].hittedtime % 3 == 0)
				{
					g_Enemy[i].alpha = 1.0f;
				}
				if (g_Enemy[i].hittedtime < 0)
				{
					g_Enemy[i].hitted = FALSE;
					g_Enemy[i].alpha = 1.0f;
					g_Enemy[i].hittedtime = 30;
				}
			}
			// スペルのアニメーション
			{
				if (skillcharge == TRUE)
				{
					spellcouAnim++;
					if ((spellcouAnim % ANIM_WAIT) == 0)
					{
						// パターンの切り替え
						spellpatAnim = (spellpatAnim + 1) % ANIM_PATTERN_NUM_2;
					}
				}
			}


			// エネミーの向きを調べる
			if (g_Enemy[i].pos.x > g_Enemy[i].pos_old.x)
			{// 右へ行くと
				g_Enemy[i].dir = ENEM_DIR_RIGHT;
				g_Enemy[i].moving = TRUE;
			}

			else if (g_Enemy[i].pos.x < g_Enemy[i].pos_old.x)
			{// 左へ行くと
				g_Enemy[i].dir = ENEM_DIR_LEFT;
				g_Enemy[i].moving = TRUE;
			}

			else if (g_Enemy[i].pos.y > g_Enemy[i].pos_old.y)
			{// 下へ行くと
				g_Enemy[i].dir = ENEM_DIR_DOWN;
				g_Enemy[i].moving = TRUE;
			}
			else if (g_Enemy[i].pos.y < g_Enemy[i].pos_old.y)
			{// 上へ行くと
				g_Enemy[i].dir = ENEM_DIR_UP;
				g_Enemy[i].moving = TRUE;
			}

			else if (g_Enemy[i].pos == g_Enemy[i].pos_old)
			{// 止まると
				g_Enemy[i].moving = FALSE;
				if (g_Enemy[i].dir == ENEM_DIR_RIGHT || g_Enemy[i].dir == ENEM_STD_RIGHT)
				{
					g_Enemy[i].dir = ENEM_STD_RIGHT;
					g_Enemy[i].skillpos = D3DXVECTOR3(g_Enemy[i].pos.x + g_Enemy[i].skillmove, g_Enemy[i].pos.y, 0.0f);	// スキルの移動量
				}

				if (g_Enemy[i].dir == ENEM_DIR_LEFT || g_Enemy[i].dir == ENEM_STD_LEFT)
				{
					g_Enemy[i].dir = ENEM_STD_LEFT;
					g_Enemy[i].skillpos = D3DXVECTOR3(g_Enemy[i].pos.x - g_Enemy[i].skillmove, g_Enemy[i].pos.y, 0.0f);	// スキルの移動量
				}

				if (g_Enemy[i].dir == ENEM_DIR_DOWN || g_Enemy[i].dir == ENEM_STD_DOWN)
				{
					g_Enemy[i].dir = ENEM_STD_DOWN;
					g_Enemy[i].skillpos = D3DXVECTOR3(g_Enemy[i].pos.x, g_Enemy[i].pos.y + g_Enemy[i].skillmove, 0.0f);	// スキルの移動量
				}

				if (g_Enemy[i].dir == ENEM_DIR_UP || g_Enemy[i].dir == ENEM_STD_UP)
				{
					g_Enemy[i].dir = ENEM_STD_UP;
					g_Enemy[i].skillpos = D3DXVECTOR3(g_Enemy[i].pos.x, g_Enemy[i].pos.y - g_Enemy[i].skillmove, 0.0f);	// スキルの移動量
				}
			}


			// ダメージの表示位置
			{
				g_Enemy[i].damagepos = D3DXVECTOR3(g_Enemy[i].pos.x, g_Enemy[i].pos.y - g_Enemy[i].damagemove, 0.0f);
			}


			// BOSSの攻撃範囲の移動処理
			{
				if (skillcharge == TRUE)
				{
					aimtime++;

					if (aimtime < 160)
					{
						for (int j = 0; j < PLAYER_MAX; j++)
						{
							D3DXVECTOR3 pos = player[j].pos - aimpos;	// 自分とプレイヤーの差分を求めている
							float angle = atan2f(pos.y, pos.x) + D3DX_PI;		// その差分を使って角度を求めている
							float speed = -3.0f;								// スピードはちょっと遅くしてみる

							aimpos.x += cosf(angle) * speed;			// angleの方向へ移動
							aimpos.y += sinf(angle) * speed;			// angleの方向へ移動
						}
					}
					else if (aimtime > 159)
					{
						skillcharge = FALSE;
						D3DXVECTOR3 pos = aimpos;
						int damage = g_Enemy[16].atk + 5;
						SetEnemySkillC(pos, damage);
						aimtime = 0;
					}
				}
			}


			// バレット発射する？
			for (int j = 0; j < PLAYER_MAX; j++)
			{	// 攻撃範囲とプレイヤーの生還をチェック
				if (g_Enemy[i].pos == g_Enemy[i].pos_old && player[j].use == TRUE)
				{
					g_Enemy[i].PerSecond++;

					// 一番目～十番目の奴の処理
					if (i < 10)
					{
						// プレイヤーとの距離を測る
						D3DXVECTOR3 temp = player[j].pos - g_Enemy[i].pos;
						float lenSq = D3DXVec3LengthSq(&temp);	// 2点間の距離（2乗した物）

						if ((g_Enemy[i].PerSecond % 130) == 0 && lenSq < 40000)
						{// 180fps一回攻撃させる
							D3DXVECTOR3 pos = g_Enemy[i].skillpos;
							int damage = g_Enemy[i].atk;
							SetEnemySkillA(pos, damage);		// １発目
							g_Enemy[i].PerSecond = 0;
						}
					}
					// 11番目～16番目の奴の処理
					else if (i > 9 && i < 16)
					{
						if ((g_Enemy[i].PerSecond % 180) == 0)
						{// 180fps一回攻撃させる
							D3DXVECTOR3 pos = g_Enemy[i].skillpos;
							int damage = g_Enemy[i].atk - 7;
							SetEnemySkillB(pos, damage);		// １発目
							g_Enemy[i].PerSecond = 0;
						}
					}
					// BOSSの処理
					else if (i == 16)
					{
						for (int j = 0; j < PLAYER_MAX; j++)
						{
							D3DXVECTOR3 temp = player[j].pos - g_Enemy[i].pos;
							float lenSq = D3DXVec3LengthSq(&temp);	// 2点間の距離（2乗した物）

							if ((g_Enemy[i].PerSecond % 230) == 0 && skillcharge == FALSE)
							{
								musicup++;

								// 180fps一回攻撃させる
								if (lenSq > 150001.0f)
								{	// プレイヤーとの距離が150000以上の時、スキルC発動
									D3DXVECTOR3 pos = player[j].pos;
									aimpos = pos;
									skillcharge = TRUE;
								}
								else if (lenSq < 150000.0f)
								{	// プレイヤーとの距離が150000以下の時、スキルB発動
									D3DXVECTOR3 pos = g_Enemy[i].skillpos;
									int damage = g_Enemy[i].atk + 5;
									SetEnemySkillB(pos, damage);		// １発目
									g_Enemy[i].PerSecond = 0;
								}

								if (musicup == 1)
								{
									// BGMを切り替える（一回だけ処理する）
									StopSound();
									PlaySound(SOUND_LABEL_BGM_bossbattle);
								}
							}
						}
					}
				}
			}
		}
		
		else
		{	// まだ登場していない奴？
			if (g_Enemy[i].pop > 0.0f)
			{
				// 六番目～八番目の奴の処理
				if (i > 4 && i < 8)
				{	// 一番目～三番目の奴を倒したら
					if (g_Enemy[0].pop == 0.0f && g_Enemy[1].pop == 0.0f && g_Enemy[2].pop == 0.0f &&
						g_Enemy[0].use == FALSE && g_Enemy[1].use == FALSE && g_Enemy[2].use == FALSE)
					{	//六番目～八番目の奴を登場させる
						g_Enemy[5].pop = 0.0f;
						g_Enemy[5].use = TRUE;

						g_Enemy[6].pop = 0.0f;
						g_Enemy[6].use = TRUE;

						g_Enemy[7].pop = 0.0f;
						g_Enemy[7].use = TRUE;
					}
				}

				// 九番目～十番目の奴の処理
				if (i > 7 && i < 10)
				{	// 六番目～八番目の奴を倒したら
					if (g_Enemy[5].pop == 0.0f && g_Enemy[6].pop == 0.0f && g_Enemy[7].pop == 0.0f &&
						g_Enemy[5].use == FALSE && g_Enemy[6].use == FALSE && g_Enemy[7].use == FALSE)
					{	// 九番目～十番目の奴を登場させる
						g_Enemy[8].pop = 0.0f;
						g_Enemy[8].use = TRUE;

						g_Enemy[9].pop = 0.0f;
						g_Enemy[9].use = TRUE;
					}
				}
			
				// 11番目～16番目の奴の処理
				if (i > 9 && i < 16)
				{	// 六番目～八番目の奴を倒したら
					if (g_Enemy[3].pop == 0.0f && g_Enemy[4].pop == 0.0f &&
						g_Enemy[3].use == FALSE && g_Enemy[4].use == FALSE)
					{	// 11番目～16番目の奴を登場させる
						g_Enemy[10].pop = 0.0f;
						g_Enemy[10].use = TRUE;
						
						g_Enemy[11].pop = 0.0f;
						g_Enemy[11].use = TRUE;

						g_Enemy[12].pop = 0.0f;
						g_Enemy[12].use = TRUE;

						g_Enemy[13].pop = 0.0f;
						g_Enemy[13].use = TRUE;
				
						g_Enemy[14].pop = 0.0f;
						g_Enemy[14].use = TRUE;

						g_Enemy[15].pop = 0.0f;
						g_Enemy[15].use = TRUE;
					}
				}
			
				// 17番目(BOSS)の処理
				if (i == 16)
				{
					int count = 0;
					// エネミーの生還を確認
					for (int m = 0; m < ENEMY_MAX - 1; m++)
					{
						if (g_Enemy[m].pop > 0.0f || g_Enemy[m].use == TRUE)
						{
							count++;
						}
					}
					// BOSS以外全部ぶっ殺したら
					if (count < 1)
					{	// 17番目(BOSS)を登場させる
						g_Enemy[16].pop = 0.0f;
						g_Enemy[16].use = TRUE;
					}
				}
			}
		}

#ifdef _DEBUG
		// デバッグ表示
		PrintDebugProc("Enemy No%d  X:%f Y:%f  pop:%f use:%d\n", i, g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pop, g_Enemy[i].use);
#endif

	}
	

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
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

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE)		// このエネミーが使われている？
		{								// Yes

			// 影を描画
			DrawEnemyShadow(i);

			if (skillcharge == TRUE)
			{// スキルの攻撃範囲とスペルを描画
				DrawEnemyAim(i);
				DrawEnemySpell(i);
			}


			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[i].texNo]);

			//エネミーの位置やテクスチャー座標を反映
			float px = g_Enemy[i].pos.x - bg->pos.x;	// エネミーの表示位置X
			float py = g_Enemy[i].pos.y - bg->pos.y;	// エネミーの表示位置Y
			float pw = g_Enemy[i].w;		// エネミーの表示幅
			float ph = g_Enemy[i].h;		// エネミーの表示高さ

			// アニメーション用
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			float tx = (float)(g_Enemy[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Enemy[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Enemy[i].alpha),
				g_Enemy[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// エネミーの影を描画
//=============================================================================
void DrawEnemyShadow(int no)
{
	BG *bg = GetBG();

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[no].shadowtexNo]);

	//影の位置やテクスチャ座標を反映
	float px = g_Enemy[no].pos.x - bg->pos.x;		// 影の表示位置X
	float py = g_Enemy[no].pos.y - bg->pos.y + 5;	// 影の表示位置Y
	float pw = g_Enemy[no].w;		// 影の表示幅
	float ph = g_Enemy[no].h;		// 影の表示高さ

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
void DrawEnemySpell(int no)
{
	BG *bg = GetBG();

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[spelltexNo]);

	//位置やテクスチャ座標を反映
	float px = g_Enemy[no].pos.x - bg->pos.x;		// 表示位置X
	float py = g_Enemy[no].pos.y - bg->pos.y;		// 表示位置Y
	float pw = g_Enemy[no].w * 3;		// 表示幅
	float ph = g_Enemy[no].h * 3;		// 表示高さ

	// アニメーション用
	float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_2;	// テクスチャの幅
	float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_2;	// テクスチャの高さ
	float tx = (float)(spellpatAnim % TEXTURE_PATTERN_DIVIDE_X_2) * tw;	// テクスチャの左上X座標
	float ty = (float)(spellpatAnim / TEXTURE_PATTERN_DIVIDE_X_2) * th;	// テクスチャの左上Y座標

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}


//=============================================================================
// スキルの攻撃範囲を描画
//=============================================================================
void DrawEnemyAim(int no)
{
	BG *bg = GetBG();

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[aimtexNo]);

	//スキルの位置やテクスチャ座標を反映
	float px = aimpos.x - bg->pos.x;		// スキルの表示位置X
	float py = aimpos.y - bg->pos.y;		// スキルの表示位置Y
	float pw = TEXTURE_WIDTH_3;				// スキルの表示幅
	float ph = TEXTURE_HEIGHT_3;			// スキルの表示高さ

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, 1.0f, 1.0f, 1.0f, 1.0f,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}


//=============================================================================
// エネミー構造体の先頭アドレスを取得
//=============================================================================
ENEMY *GetEnemy(void)
{

	return &g_Enemy[0];
}





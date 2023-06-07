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
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100/1)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(100/1)	// 

#define TEXTURE_WIDTH_3				(200)	// �L�����T�C�Y
#define TEXTURE_HEIGHT_3			(200)	// 

#define TEXTURE_MAX					(6)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(8)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

#define TEXTURE_PATTERN_DIVIDE_X_2	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_2	(8)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_2			(TEXTURE_PATTERN_DIVIDE_X_2*TEXTURE_PATTERN_DIVIDE_Y_2)	// �A�j���[�V�����p�^�[����

#define ANIM_WAIT					(8)		// �A�j���[�V�����̐؂�ւ��Wait�l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void DrawEnemyShadow(int no);
void DrawEnemySpell(int no);
void DrawEnemyAim(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/enemy01.png",
	"data/TEXTURE/enemy02.png",
	"data/TEXTURE/enemy03.png",
	"data/TEXTURE/shadow.png",
	"data/TEXTURE/spell.png",
	"data/TEXTURE/enemyaim.png",
};


static LINEAR_INTERPOLATION g_MoveTbl0[] = {
	//���W									��]��							�g�嗦							����
	{ D3DXVECTOR3(160.0f,1236.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(160.0f,1040.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(464.0f,1040.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(160.0f,1040.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
};

static LINEAR_INTERPOLATION g_MoveTbl1[] = {
	//���W									��]��							�g�嗦							����
	{ D3DXVECTOR3(1216.0f,1018.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(1216.0f,1284.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
};

static LINEAR_INTERPOLATION g_MoveTbl2[] = {
	//���W									��]��							�g�嗦							����
	{ D3DXVECTOR3(540.0f,1432.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(540.0f,1664.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.010f },
	{ D3DXVECTOR3(984.0f,1664.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.013f },
	{ D3DXVECTOR3(984.0f,1432.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.010f },
};

static LINEAR_INTERPOLATION g_MoveTbl3[] = {
	//���W									��]��							�g�嗦							����
	{ D3DXVECTOR3(2270.0f,2076.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.006f },
	{ D3DXVECTOR3(2970.0f,2076.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.006f },
};

static LINEAR_INTERPOLATION g_MoveTbl4[] = {
	//���W									��]��							�g�嗦							����
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


static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static ENEMY	g_Enemy[ENEMY_MAX];		// �G�l�~�[�\����

static BOOL		skillcharge;			// �X�L���`���[�W���t���O

static int		spellcouAnim;			// �X�y���̃A�j���[�V�����J�E���g
static int		spellpatAnim;			// �X�y���̃A�j���[�V�����p�^�[��
static int		spelltexNo;				// �X�y���̃e�N�X�`���ԍ�

static D3DXVECTOR3	aimpos ;			// �U���͈͂̍��W
static int			aimtime;			// �U���͈͂̑��ݎ��Ԃ��Ǘ�����
static int			aimtexNo;			// �U���͈͂̃e�N�X�`���ԍ�

static int			musicup;			// �~���[�W�b�N�A�b�v�t���O


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �G�l�~�[�\���̂̏�����
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_Enemy[i].use = TRUE;
		g_Enemy[i].pos = D3DXVECTOR3(2000, 100.0f + i*50, 0.0f);	// �\���ʒu�̏�����
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

		g_Enemy[i].dir = ENEM_STD_DOWN;							// �������ɂ��Ƃ���
		g_Enemy[i].moving = FALSE;								// �ړ����t���O
		g_Enemy[i].patternAnim = g_Enemy[i].dir * TEXTURE_PATTERN_DIVIDE_X;
		g_Enemy[i].shadowtexNo = 3;

		g_Enemy[i].HP = g_Enemy[i].HP_MAX = 80;
		g_Enemy[i].atk = 10;

		g_Enemy[i].skillmove = 110.0f;						// �X�L��A�̈ړ���
		g_Enemy[i].skillpos = D3DXVECTOR3(g_Enemy[i].pos.x, g_Enemy[i].pos.y + g_Enemy[i].skillmove, 0.0f);	// �X�L���̈ړ���
		g_Enemy[i].PerSecond = 100;
		
		g_Enemy[i].damagepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].damagemove = 30.0f;

		g_Enemy[i].hit = FALSE;			// �����蔻����s���t���O

		g_Enemy[i].hitted = FALSE;		// �U�������t���O
		g_Enemy[i].hittedtime = 30;
		g_Enemy[i].alpha = 1.0f;

		// �s���p�^�[����������
		g_Enemy[i].time		= 0.0f;			// ���`��ԗp
		g_Enemy[i].moveTblNo= 0;			// �f�[�^�e�[�u��
		g_Enemy[i].tblMax	= sizeof(g_MoveTbl0) / sizeof(LINEAR_INTERPOLATION);// ���`��ԗp

	}

	// �Q�Ԗڂ̓z�̍s���p�^�[�����Z�b�g
	g_Enemy[1].moveTblNo = 1;				// �f�[�^�e�[�u��
	g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(LINEAR_INTERPOLATION);	// ���`��ԗp

	// �R�Ԗڂ̓z�̍s���p�^�[�����Z�b�g
	g_Enemy[2].moveTblNo = 2;				// �f�[�^�e�[�u��
	g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(LINEAR_INTERPOLATION);	// ���`��ԗp

	// �S�Ԗڂ̓z�̍s���p�^�[�����Z�b�g
	g_Enemy[3].moveTblNo = 3;				// �f�[�^�e�[�u��
	g_Enemy[3].tblMax = sizeof(g_MoveTbl3) / sizeof(LINEAR_INTERPOLATION);	// ���`��ԗp

	// �T�Ԗڂ̓z�̍s���p�^�[�����Z�b�g
	g_Enemy[4].moveTblNo = 4;				// �f�[�^�e�[�u��
	g_Enemy[4].tblMax = sizeof(g_MoveTbl4) / sizeof(LINEAR_INTERPOLATION);	// ���`��ԗp

	// 6�Ԗڂ̓z
	g_Enemy[5].use = FALSE;
	g_Enemy[5].pop = 1.0f;
	g_Enemy[5].pos = D3DXVECTOR3(616, -100.0f, 0.0f);	// �\���ʒu�̏�����

	// 7�Ԗڂ̓z
	g_Enemy[6].use = FALSE;
	g_Enemy[6].pop = 1.0f;
	g_Enemy[6].pos = D3DXVECTOR3(g_Enemy[5].pos.x+150.0f, 0.0f, 0.0f);	// �\���ʒu�̏�����

	// 8�Ԗڂ̓z
	g_Enemy[7].use = FALSE;
	g_Enemy[7].pop = 1.0f;
	g_Enemy[7].pos = D3DXVECTOR3(g_Enemy[6].pos.x+150.0f, -400.0f, 0.0f);	// �\���ʒu�̏�����

	// 9�Ԗڂ̓z
	g_Enemy[8].use = FALSE;
	g_Enemy[8].pop = 1.0f;
	g_Enemy[8].pos = D3DXVECTOR3(2220, 1588, 0.0f);	// �\���ʒu�̏�����

	// 10�Ԗڂ̓z
	g_Enemy[9].use = FALSE;
	g_Enemy[9].pop = 1.0f;
	g_Enemy[9].pos = D3DXVECTOR3(g_Enemy[8].pos.x+200.0f, 1588, 0.0f);	// �\���ʒu�̏�����

	// 11�Ԗڂ̓z
	g_Enemy[10].use = FALSE;
	g_Enemy[10].pop = 1.0f;
	g_Enemy[10].pos = D3DXVECTOR3(1436, 2164, 0.0f);						// �\���ʒu�̏�����
	g_Enemy[10].dir = ENEM_STD_RIGHT;										// �E�����ɂ��Ƃ�
	g_Enemy[10].texNo = 1;

	// 12�Ԗڂ̓z
	g_Enemy[11].use = FALSE;
	g_Enemy[11].pop = 1.0f;
	g_Enemy[11].pos = D3DXVECTOR3(g_Enemy[10].pos.x, g_Enemy[10].pos.y+120.0f, 0.0f);		// �\���ʒu�̏�����
	g_Enemy[11].dir = ENEM_STD_RIGHT;										// �E�����ɂ��Ƃ�
	g_Enemy[11].texNo = 1;

	// 13�Ԗڂ̓z
	g_Enemy[12].use = FALSE;
	g_Enemy[12].pop = 1.0f;
	g_Enemy[12].pos = D3DXVECTOR3(g_Enemy[10].pos.x, g_Enemy[11].pos.y + 120.0f, 0.0f);		// �\���ʒu�̏�����
	g_Enemy[12].dir = ENEM_STD_RIGHT;										// �E�����ɂ��Ƃ�
	g_Enemy[12].texNo = 1;

	// 14�Ԗڂ̓z
	g_Enemy[13].use = FALSE;
	g_Enemy[13].pop = 1.0f;
	g_Enemy[13].pos = D3DXVECTOR3(3270, 2108, 0.0f);						// �\���ʒu�̏�����
	g_Enemy[13].dir = ENEM_STD_LEFT;										// �������ɂ��Ƃ�
	g_Enemy[13].texNo = 1;

	// 15�Ԗڂ̓z
	g_Enemy[14].use = FALSE;
	g_Enemy[14].pop = 1.0f;
	g_Enemy[14].pos = D3DXVECTOR3(g_Enemy[13].pos.x, g_Enemy[13].pos.y+120.0f, 0.0f);						// �\���ʒu�̏�����
	g_Enemy[14].dir = ENEM_STD_LEFT;										// �������ɂ��Ƃ�
	g_Enemy[14].texNo = 1;

	// 16�Ԗڂ̓z
	g_Enemy[15].use = FALSE;
	g_Enemy[15].pop = 1.0f;
	g_Enemy[15].pos = D3DXVECTOR3(g_Enemy[13].pos.x, g_Enemy[14].pos.y+120.0f, 0.0f);						// �\���ʒu�̏�����
	g_Enemy[15].dir = ENEM_STD_LEFT;										// �������ɂ��Ƃ�
	g_Enemy[15].texNo = 1;

	// BOSS
	g_Enemy[16].use = FALSE;
	g_Enemy[16].pop = 1.0f;
	g_Enemy[16].pos = D3DXVECTOR3(4324, 1792, 0.0f);						// �\���ʒu�̏�����
	g_Enemy[16].dir = ENEM_STD_DOWN;										// �������ɂ��Ƃ�
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
	
	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE)	// ���̃G�l�~�[���g���Ă���H
		{							// Yes
			PLAYER *player = GetPlayer();
		
			// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���
			g_Enemy[i].pos_old = g_Enemy[i].pos;
			
			// �ړ����� 
			{
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// �P�Ԗ��`�T�Ԗڂ̂�̏���
					if (i < 5)
					{
						// �v���C���[�Ƃ̋����𑪂�
						D3DXVECTOR3 temp = player[j].pos - g_Enemy[i].pos;
						float lenSq = D3DXVec3LengthSq(&temp);	// 2�_�Ԃ̋����i2�悵�����j

						// ����������150000�ȏ�̏ꍇ
						if (lenSq > 150000.0f)
						{// �s���e�[�u���ɏ]���č��W�ړ��i���`��ԁj
							int nowNo = (int)g_Enemy[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
							int maxNo = g_Enemy[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
							int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
							LINEAR_INTERPOLATION *tbl = g_MoveTblAdr[g_Enemy[i].moveTblNo];	// �s���e�[�u���̃A�h���X���擾
							D3DXVECTOR3	pos = tbl[nextNo].pos - tbl[nowNo].pos;	// XYZ�ړ��ʂ��v�Z���Ă���
							D3DXVECTOR3	rot = tbl[nextNo].rot - tbl[nowNo].rot;	// XYZ��]�ʂ��v�Z���Ă���
							D3DXVECTOR3	scl = tbl[nextNo].scl - tbl[nowNo].scl;	// XYZ�g�嗦���v�Z���Ă���
							float nowTime = g_Enemy[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���
							pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
							rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
							scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

							// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
							g_Enemy[i].pos = tbl[nowNo].pos + pos;

							// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
							g_Enemy[i].rot = tbl[nowNo].rot + rot;

							// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
							g_Enemy[i].scl = tbl[nowNo].scl + scl;
							g_Enemy[i].w = TEXTURE_WIDTH * g_Enemy[i].scl.x;
							g_Enemy[i].h = TEXTURE_HEIGHT * g_Enemy[i].scl.y;

							// frame���g�Ď��Ԍo�ߏ���������
							g_Enemy[i].time += tbl[nowNo].time;			// ���Ԃ�i�߂Ă���
							if ((int)g_Enemy[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
							{
								g_Enemy[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
							}
						}

						// �G�l�~�[�̃z�[�~���O����
						else if (lenSq < 150000.0f)
						{// ����������150000�ȓ��Ȃ�z�[�~���O����
							D3DXVECTOR3 pos = player[j].pos - g_Enemy[i].pos;	// �����ƃv���C���[�̍��������߂Ă���
							float angle = atan2f(pos.y, pos.x) + D3DX_PI;		// ���̍������g���Ċp�x�����߂Ă���
							float speed = -4.0f;								// �X�s�[�h�͂�����ƒx�����Ă݂�

							g_Enemy[i].pos.x += cosf(angle) * speed;			// angle�̕����ֈړ�
							g_Enemy[i].pos.y += sinf(angle) * speed;			// angle�̕����ֈړ�
						}
					}
				
					// �U�Ԗځ`8�Ԗڂ̂�̏���
					else if (i > 4 && i < 8)
					{
						float move = 4.0f;								//�X�s�[�h

						if (g_Enemy[i].pos.y < 780.0f && g_Enemy[i].homing == FALSE)
						{
							g_Enemy[i].pos.y += move;
							
							if (g_Enemy[i].pos.y > 779.0f)
							{
								g_Enemy[i].homing = TRUE;
							}
						}
						// �w�肳�ꂽ�ꏊ�Ɉړ�������z�[�~���O�����
						if (g_Enemy[i].homing == TRUE)
						{
							D3DXVECTOR3 pos = player[j].pos - g_Enemy[i].pos;	// �����ƃv���C���[�̍��������߂Ă���
							float angle = atan2f(pos.y, pos.x) + D3DX_PI;		// ���̍������g���Ċp�x�����߂Ă���
							float speed = -4.0f;								// �X�s�[�h�͂�����ƒx�����Ă݂�

							g_Enemy[i].pos.x += cosf(angle) * speed;			// angle�̕����ֈړ�
							g_Enemy[i].pos.y += sinf(angle) * speed;			// angle�̕����ֈړ�
						}
					}
				
					// 9�Ԗځ`10�Ԗڂ̂�̏���
					else if (i > 7 && i < 10)
					{
						float move = 4.0f;								//�X�s�[�h

						if (g_Enemy[i].pos.x > 1300.0f && g_Enemy[i].homing == FALSE)
						{
							g_Enemy[i].pos.x -= move;
							
							if (g_Enemy[i].pos.x < 1301.0f)
							{
								g_Enemy[i].homing = TRUE;
							}
						}
						// �w�肳�ꂽ�ꏊ�Ɉړ�������z�[�~���O�����
						if (g_Enemy[i].homing == TRUE)
						{
							D3DXVECTOR3 pos = player[j].pos - g_Enemy[i].pos;	// �����ƃv���C���[�̍��������߂Ă���
							float angle = atan2f(pos.y, pos.x) + D3DX_PI;		// ���̍������g���Ċp�x�����߂Ă���
							float speed = -4.0f;								// �X�s�[�h�͂�����ƒx�����Ă݂�

							g_Enemy[i].pos.x += cosf(angle) * speed;			// angle�̕����ֈړ�
							g_Enemy[i].pos.y += sinf(angle) * speed;			// angle�̕����ֈړ�
						}
					}
				}
			}

			// �ړ����I�������v���C���[�Ƃ̓����蔻��
			{
				// �v���C���[�̐��������蔻����s��
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// �����Ă�v���C���[�Ɠ����蔻�������
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
							player[j].pos, player[j].w, player[j].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							g_Enemy[i].pos = g_Enemy[i].pos_old;
						}
					}
				}
			}

			// �ړ����I������瑼�̃G�l�~�[�Ƃ̓����蔻��
			{
				// �G�l�~�[�̐��������蔻����s��
				for (int k = 0; k < ENEMY_MAX; k++)
				{
					// �����Ă�G�l�~�[�Ɠ����蔻�������
					if (g_Enemy[k].use == TRUE && k != i)
					{
						BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
							g_Enemy[k].pos, g_Enemy[k].w, g_Enemy[k].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							g_Enemy[i].pos = g_Enemy[i].pos_old;
						}
					}
				}
			}

			// �ړ����I�������MAP���OBJ�Ƃ̓����蔻��
			{
				// �G�l�~�[�̐��������蔻����s��
				for (int k = 0; k < ENEMY_MAX; k++)
				{
					BG *obj = GetOBJ();
					// ��Q���̐��������蔻����s��
					for (int l = 0; l < OBJ_MAX; l++)
					{
						BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
							obj[l].pos, obj[l].w, obj[l].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							g_Enemy[i].pos = g_Enemy[i].pos_old;
						}
					}
				}
			}


			// �A�j���[�V����
			{
				g_Enemy[i].countAnim++;
				if (g_Enemy[i].countAnim > ANIM_WAIT)
				{
					g_Enemy[i].countAnim = 0;
					// �p�^�[���̐؂�ւ�
					g_Enemy[i].patternAnim = (g_Enemy[i].dir * TEXTURE_PATTERN_DIVIDE_X) + ((g_Enemy[i].patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
				}
			}
			// �U������鎞�̃A�j���[�V������`��
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
			// �X�y���̃A�j���[�V����
			{
				if (skillcharge == TRUE)
				{
					spellcouAnim++;
					if ((spellcouAnim % ANIM_WAIT) == 0)
					{
						// �p�^�[���̐؂�ւ�
						spellpatAnim = (spellpatAnim + 1) % ANIM_PATTERN_NUM_2;
					}
				}
			}


			// �G�l�~�[�̌����𒲂ׂ�
			if (g_Enemy[i].pos.x > g_Enemy[i].pos_old.x)
			{// �E�֍s����
				g_Enemy[i].dir = ENEM_DIR_RIGHT;
				g_Enemy[i].moving = TRUE;
			}

			else if (g_Enemy[i].pos.x < g_Enemy[i].pos_old.x)
			{// ���֍s����
				g_Enemy[i].dir = ENEM_DIR_LEFT;
				g_Enemy[i].moving = TRUE;
			}

			else if (g_Enemy[i].pos.y > g_Enemy[i].pos_old.y)
			{// ���֍s����
				g_Enemy[i].dir = ENEM_DIR_DOWN;
				g_Enemy[i].moving = TRUE;
			}
			else if (g_Enemy[i].pos.y < g_Enemy[i].pos_old.y)
			{// ��֍s����
				g_Enemy[i].dir = ENEM_DIR_UP;
				g_Enemy[i].moving = TRUE;
			}

			else if (g_Enemy[i].pos == g_Enemy[i].pos_old)
			{// �~�܂��
				g_Enemy[i].moving = FALSE;
				if (g_Enemy[i].dir == ENEM_DIR_RIGHT || g_Enemy[i].dir == ENEM_STD_RIGHT)
				{
					g_Enemy[i].dir = ENEM_STD_RIGHT;
					g_Enemy[i].skillpos = D3DXVECTOR3(g_Enemy[i].pos.x + g_Enemy[i].skillmove, g_Enemy[i].pos.y, 0.0f);	// �X�L���̈ړ���
				}

				if (g_Enemy[i].dir == ENEM_DIR_LEFT || g_Enemy[i].dir == ENEM_STD_LEFT)
				{
					g_Enemy[i].dir = ENEM_STD_LEFT;
					g_Enemy[i].skillpos = D3DXVECTOR3(g_Enemy[i].pos.x - g_Enemy[i].skillmove, g_Enemy[i].pos.y, 0.0f);	// �X�L���̈ړ���
				}

				if (g_Enemy[i].dir == ENEM_DIR_DOWN || g_Enemy[i].dir == ENEM_STD_DOWN)
				{
					g_Enemy[i].dir = ENEM_STD_DOWN;
					g_Enemy[i].skillpos = D3DXVECTOR3(g_Enemy[i].pos.x, g_Enemy[i].pos.y + g_Enemy[i].skillmove, 0.0f);	// �X�L���̈ړ���
				}

				if (g_Enemy[i].dir == ENEM_DIR_UP || g_Enemy[i].dir == ENEM_STD_UP)
				{
					g_Enemy[i].dir = ENEM_STD_UP;
					g_Enemy[i].skillpos = D3DXVECTOR3(g_Enemy[i].pos.x, g_Enemy[i].pos.y - g_Enemy[i].skillmove, 0.0f);	// �X�L���̈ړ���
				}
			}


			// �_���[�W�̕\���ʒu
			{
				g_Enemy[i].damagepos = D3DXVECTOR3(g_Enemy[i].pos.x, g_Enemy[i].pos.y - g_Enemy[i].damagemove, 0.0f);
			}


			// BOSS�̍U���͈͂̈ړ�����
			{
				if (skillcharge == TRUE)
				{
					aimtime++;

					if (aimtime < 160)
					{
						for (int j = 0; j < PLAYER_MAX; j++)
						{
							D3DXVECTOR3 pos = player[j].pos - aimpos;	// �����ƃv���C���[�̍��������߂Ă���
							float angle = atan2f(pos.y, pos.x) + D3DX_PI;		// ���̍������g���Ċp�x�����߂Ă���
							float speed = -3.0f;								// �X�s�[�h�͂�����ƒx�����Ă݂�

							aimpos.x += cosf(angle) * speed;			// angle�̕����ֈړ�
							aimpos.y += sinf(angle) * speed;			// angle�̕����ֈړ�
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


			// �o���b�g���˂���H
			for (int j = 0; j < PLAYER_MAX; j++)
			{	// �U���͈͂ƃv���C���[�̐��҂��`�F�b�N
				if (g_Enemy[i].pos == g_Enemy[i].pos_old && player[j].use == TRUE)
				{
					g_Enemy[i].PerSecond++;

					// ��Ԗځ`�\�Ԗڂ̓z�̏���
					if (i < 10)
					{
						// �v���C���[�Ƃ̋����𑪂�
						D3DXVECTOR3 temp = player[j].pos - g_Enemy[i].pos;
						float lenSq = D3DXVec3LengthSq(&temp);	// 2�_�Ԃ̋����i2�悵�����j

						if ((g_Enemy[i].PerSecond % 130) == 0 && lenSq < 40000)
						{// 180fps���U��������
							D3DXVECTOR3 pos = g_Enemy[i].skillpos;
							int damage = g_Enemy[i].atk;
							SetEnemySkillA(pos, damage);		// �P����
							g_Enemy[i].PerSecond = 0;
						}
					}
					// 11�Ԗځ`16�Ԗڂ̓z�̏���
					else if (i > 9 && i < 16)
					{
						if ((g_Enemy[i].PerSecond % 180) == 0)
						{// 180fps���U��������
							D3DXVECTOR3 pos = g_Enemy[i].skillpos;
							int damage = g_Enemy[i].atk - 7;
							SetEnemySkillB(pos, damage);		// �P����
							g_Enemy[i].PerSecond = 0;
						}
					}
					// BOSS�̏���
					else if (i == 16)
					{
						for (int j = 0; j < PLAYER_MAX; j++)
						{
							D3DXVECTOR3 temp = player[j].pos - g_Enemy[i].pos;
							float lenSq = D3DXVec3LengthSq(&temp);	// 2�_�Ԃ̋����i2�悵�����j

							if ((g_Enemy[i].PerSecond % 230) == 0 && skillcharge == FALSE)
							{
								musicup++;

								// 180fps���U��������
								if (lenSq > 150001.0f)
								{	// �v���C���[�Ƃ̋�����150000�ȏ�̎��A�X�L��C����
									D3DXVECTOR3 pos = player[j].pos;
									aimpos = pos;
									skillcharge = TRUE;
								}
								else if (lenSq < 150000.0f)
								{	// �v���C���[�Ƃ̋�����150000�ȉ��̎��A�X�L��B����
									D3DXVECTOR3 pos = g_Enemy[i].skillpos;
									int damage = g_Enemy[i].atk + 5;
									SetEnemySkillB(pos, damage);		// �P����
									g_Enemy[i].PerSecond = 0;
								}

								if (musicup == 1)
								{
									// BGM��؂�ւ���i��񂾂���������j
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
		{	// �܂��o�ꂵ�Ă��Ȃ��z�H
			if (g_Enemy[i].pop > 0.0f)
			{
				// �Z�Ԗځ`���Ԗڂ̓z�̏���
				if (i > 4 && i < 8)
				{	// ��Ԗځ`�O�Ԗڂ̓z��|������
					if (g_Enemy[0].pop == 0.0f && g_Enemy[1].pop == 0.0f && g_Enemy[2].pop == 0.0f &&
						g_Enemy[0].use == FALSE && g_Enemy[1].use == FALSE && g_Enemy[2].use == FALSE)
					{	//�Z�Ԗځ`���Ԗڂ̓z��o�ꂳ����
						g_Enemy[5].pop = 0.0f;
						g_Enemy[5].use = TRUE;

						g_Enemy[6].pop = 0.0f;
						g_Enemy[6].use = TRUE;

						g_Enemy[7].pop = 0.0f;
						g_Enemy[7].use = TRUE;
					}
				}

				// ��Ԗځ`�\�Ԗڂ̓z�̏���
				if (i > 7 && i < 10)
				{	// �Z�Ԗځ`���Ԗڂ̓z��|������
					if (g_Enemy[5].pop == 0.0f && g_Enemy[6].pop == 0.0f && g_Enemy[7].pop == 0.0f &&
						g_Enemy[5].use == FALSE && g_Enemy[6].use == FALSE && g_Enemy[7].use == FALSE)
					{	// ��Ԗځ`�\�Ԗڂ̓z��o�ꂳ����
						g_Enemy[8].pop = 0.0f;
						g_Enemy[8].use = TRUE;

						g_Enemy[9].pop = 0.0f;
						g_Enemy[9].use = TRUE;
					}
				}
			
				// 11�Ԗځ`16�Ԗڂ̓z�̏���
				if (i > 9 && i < 16)
				{	// �Z�Ԗځ`���Ԗڂ̓z��|������
					if (g_Enemy[3].pop == 0.0f && g_Enemy[4].pop == 0.0f &&
						g_Enemy[3].use == FALSE && g_Enemy[4].use == FALSE)
					{	// 11�Ԗځ`16�Ԗڂ̓z��o�ꂳ����
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
			
				// 17�Ԗ�(BOSS)�̏���
				if (i == 16)
				{
					int count = 0;
					// �G�l�~�[�̐��҂��m�F
					for (int m = 0; m < ENEMY_MAX - 1; m++)
					{
						if (g_Enemy[m].pop > 0.0f || g_Enemy[m].use == TRUE)
						{
							count++;
						}
					}
					// BOSS�ȊO�S���Ԃ��E������
					if (count < 1)
					{	// 17�Ԗ�(BOSS)��o�ꂳ����
						g_Enemy[16].pop = 0.0f;
						g_Enemy[16].use = TRUE;
					}
				}
			}
		}

#ifdef _DEBUG
		// �f�o�b�O�\��
		PrintDebugProc("Enemy No%d  X:%f Y:%f  pop:%f use:%d\n", i, g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pop, g_Enemy[i].use);
#endif

	}
	

}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG *bg = GetBG();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes

			// �e��`��
			DrawEnemyShadow(i);

			if (skillcharge == TRUE)
			{// �X�L���̍U���͈͂ƃX�y����`��
				DrawEnemyAim(i);
				DrawEnemySpell(i);
			}


			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[i].texNo]);

			//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Enemy[i].pos.x - bg->pos.x;	// �G�l�~�[�̕\���ʒuX
			float py = g_Enemy[i].pos.y - bg->pos.y;	// �G�l�~�[�̕\���ʒuY
			float pw = g_Enemy[i].w;		// �G�l�~�[�̕\����
			float ph = g_Enemy[i].h;		// �G�l�~�[�̕\������

			// �A�j���[�V�����p
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = (float)(g_Enemy[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Enemy[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Enemy[i].alpha),
				g_Enemy[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// �G�l�~�[�̉e��`��
//=============================================================================
void DrawEnemyShadow(int no)
{
	BG *bg = GetBG();

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[no].shadowtexNo]);

	//�e�̈ʒu��e�N�X�`�����W�𔽉f
	float px = g_Enemy[no].pos.x - bg->pos.x;		// �e�̕\���ʒuX
	float py = g_Enemy[no].pos.y - bg->pos.y + 5;	// �e�̕\���ʒuY
	float pw = g_Enemy[no].w;		// �e�̕\����
	float ph = g_Enemy[no].h;		// �e�̕\������

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, 1.0f, 1.0f, 1.0f, 1.0f,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}


//=============================================================================
// �X�y����`��
//=============================================================================
void DrawEnemySpell(int no)
{
	BG *bg = GetBG();

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[spelltexNo]);

	//�ʒu��e�N�X�`�����W�𔽉f
	float px = g_Enemy[no].pos.x - bg->pos.x;		// �\���ʒuX
	float py = g_Enemy[no].pos.y - bg->pos.y;		// �\���ʒuY
	float pw = g_Enemy[no].w * 3;		// �\����
	float ph = g_Enemy[no].h * 3;		// �\������

	// �A�j���[�V�����p
	float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_2;	// �e�N�X�`���̕�
	float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_2;	// �e�N�X�`���̍���
	float tx = (float)(spellpatAnim % TEXTURE_PATTERN_DIVIDE_X_2) * tw;	// �e�N�X�`���̍���X���W
	float ty = (float)(spellpatAnim / TEXTURE_PATTERN_DIVIDE_X_2) * th;	// �e�N�X�`���̍���Y���W

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}


//=============================================================================
// �X�L���̍U���͈͂�`��
//=============================================================================
void DrawEnemyAim(int no)
{
	BG *bg = GetBG();

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[aimtexNo]);

	//�X�L���̈ʒu��e�N�X�`�����W�𔽉f
	float px = aimpos.x - bg->pos.x;		// �X�L���̕\���ʒuX
	float py = aimpos.y - bg->pos.y;		// �X�L���̕\���ʒuY
	float pw = TEXTURE_WIDTH_3;				// �X�L���̕\����
	float ph = TEXTURE_HEIGHT_3;			// �X�L���̕\������

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, 1.0f, 1.0f, 1.0f, 1.0f,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}


//=============================================================================
// �G�l�~�[�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY *GetEnemy(void)
{

	return &g_Enemy[0];
}





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
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(150/2)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(150/2)	// 
#define TEXTURE_MAX					(6)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(8)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

#define TEXTURE_PATTERN_DIVIDE_X_2	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_2	(8)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_2			(TEXTURE_PATTERN_DIVIDE_X_2*TEXTURE_PATTERN_DIVIDE_Y_2)	// �A�j���[�V�����p�^�[����

#define ANIM_WAIT					(10)		// �A�j���[�V�����̐؂�ւ��Wait�l
#define ANIM_WAIT_2					(5)			// �A�j���[�V�����̐؂�ւ��Wait�l

// �v���C���[�̉�ʓ��z�u���W
#define PLAYER_DISP_X				(SCREEN_WIDTH/2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_HEIGHT)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void DrawPlayerShadow(int no);
void DrawPlayerOffset(int no);
void DrawSkillAim(int no);
void DrawSkillSpell(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/char01.png",
	"data/TEXTURE/char02.png",
	"data/TEXTURE/char03.png",
	"data/TEXTURE/shadow.png",
	"data/TEXTURE/aim.png",
	"data/TEXTURE/spell.png",
};


static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static PLAYER	g_Player[PLAYER_MAX];	// �v���C���[�\����

static BOOL		g_goal;			// FALSE:�`���[�g���A���̃}�[�N�܂Œ����ĂȂ�  TRUE:������

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
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


	// �v���C���[�\���̂̏�����
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].use = TRUE;
		g_Player[i].pos = D3DXVECTOR3(50.0f, 300.0f, 0.0f);	// �v���C���[�̍ŏ��ʒu
		g_Player[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].w   = TEXTURE_WIDTH;
		g_Player[i].h   = TEXTURE_HEIGHT;
		g_Player[i].texNo = 0;

		g_Player[i].countAnim = 0;
		g_Player[i].patternAnim = 0;

		g_Player[i].move = D3DXVECTOR3(4.0f, 0.0f, 0.0f);		// �ړ���

		g_Player[i].dir = CHAR_STD_DOWN;						// �������ɂ��Ƃ���
		g_Player[i].moving = FALSE;								// �ړ����t���O
		g_Player[i].patternAnim = g_Player[i].dir * TEXTURE_PATTERN_DIVIDE_X;

		g_Player[i].shadowtexNo = 3;
	
		g_Player[i].skillmove_a = 110.0f;						// �X�L��A�̈ړ���
		g_Player[i].skillmove_b = 8.0f;							// �X�L��B�̈ړ���
		g_Player[i].skilluse = FALSE;							// �X�L���`���[�W���t���O
		g_Player[i].skillusing = 0;
																// �X�L���̕\�����W
		g_Player[i].skillpos_a = g_Player[i].skillpos_b = D3DXVECTOR3(g_Player[i].pos.x, g_Player[i].pos.y + g_Player[i].skillmove_a, 0.0f);
		g_Player[i].skilltexNo = 4;

		g_Player[i].spellcouAnim = 0;
		g_Player[i].spellpatAnim = 0;
		g_Player[i].spelltexNo = 5;

		g_Player[i].dashing = FALSE;							// �_�b�V����
		// ���g�p
		for (int j = 0; j < PLAYER_OFFSET_CNT; j++)
		{
			g_Player[i].offset[j] = g_Player[i].pos;
		}
	
		// �v���C���[�̃p�����[�^�[
		g_Player[i].HP = g_Player[i].HP_MAX = 100;
		g_Player[i].ST = g_Player[i].ST_MAX = 200;
		g_Player[i].atk = 50;
		g_Player[i].PerSecond = 0;
	
		g_Player[i].damagepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].damagemove = 30.0f;

		g_Player[i].hitted = FALSE;
		g_Player[i].hittedtime = 30;
		g_Player[i].alpha = 1.0f;

		// �`���[�g���A���̎������s������������
		int mode = GetMode();
		if (mode == MODE_TUTORIAL)
		{
			g_Player[i].dir = CHAR_STD_DOWN;
			g_Player[i].pos = D3DXVECTOR3(700.0f, 200.0f, 0.0f);
			g_goal = FALSE;
		}
	}
	
	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// �����Ă�v���C���[��������������
		if (g_Player[i].use == TRUE)
		{
			// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���
			g_Player[i].pos_old = g_Player[i].pos;

			// ���g�p
			for (int j = PLAYER_OFFSET_CNT - 1; j > 0; j--)
			{
				g_Player[i].offset[j] = g_Player[i].offset[j - 1];
			}
			g_Player[i].offset[0] = g_Player[i].pos_old;


			// �A�j���[�V���� 
			{
				g_Player[i].countAnim += 1.0f;
				if (g_Player[i].dashing == TRUE)
				{
					g_Player[i].countAnim += 1.0f;
				}
				if (g_Player[i].countAnim > ANIM_WAIT)
				{
					g_Player[i].countAnim = 0.0f;
					// �p�^�[���̐؂�ւ�
					g_Player[i].patternAnim = (g_Player[i].dir * TEXTURE_PATTERN_DIVIDE_X) + ((g_Player[i].patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
				}
			}
			// �X�y���̃A�j���[�V����
			{
				if (g_Player[i].skilluse == TRUE)
				{
					g_Player[i].spellcouAnim++;
					if ((g_Player[i].spellcouAnim % ANIM_WAIT_2) == 0)
					{
						// �p�^�[���̐؂�ւ�
						g_Player[i].spellpatAnim = (g_Player[i].spellpatAnim + 1) % ANIM_PATTERN_NUM_2;
					}
				}
			}
			// �U������鎞�̃A�j���[�V������`��
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
			{// �v���C���[�̌����𒲂ׂ�
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

			// �����ɍ��킹�ăX�L���̕\���ʒu��ς���
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



			// �L�[���͂ňړ� 
			{
				float speed = g_Player[i].move.x;

				g_Player[i].moving = FALSE;
				g_Player[i].dashing = FALSE;
				
				if (GetKeyboardPress(DIK_SPACE) && g_Player[i].ST > 0)
				{// N��������A�X�^�~�i�����Ղ��ă_�b�V������
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
				{// N�����ĂȂ����A�X�^�~�i���񕜂���
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


				// MAP�O�`�F�b�N
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


				// �v���C���[�̗����ʒu����MAP�̃X�N���[�����W���v�Z����
				bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
				if (bg->pos.x < 0) bg->pos.x = 0;
				if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

				bg->pos.y = g_Player[i].pos.y - PLAYER_DISP_Y;
				if (bg->pos.y < 0) bg->pos.y = 0;
				if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;


				// �ړ����I�������}�b�v��̏�Q���Ƃ̓����蔻��
				{
					int mode = GetMode();
					if (mode == MODE_GAME)
					{
						BG *obj = GetOBJ();
						// ��Q���̐��������蔻����s��
						for (int k = 0; k < OBJ_MAX; k++)
						{
							BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
								obj[k].pos, obj[k].w, obj[k].h);
							// �������Ă���H
							if (ans == TRUE)
							{
								// �����������̏���
								g_Player[i].pos = g_Player[i].pos_old;
								PlaySound(SOUND_LABEL_SE_object);
							}
						}
					}
				}

				// �ړ����I�������G�l�~�[�Ƃ̓����蔻��
				{
					ENEMY *enemy = GetEnemy();
					// �G�l�~�[�̐��������蔻����s��
					for (int j = 0; j < ENEMY_MAX; j++)
					{
						// �����Ă�G�l�~�[�Ɠ����蔻�������
						if (enemy[j].use == TRUE)
						{
							BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
								enemy[j].pos, enemy[j].w, enemy[j].h);
							// �������Ă���H
							if (ans == TRUE)
							{
								// �����������̏���
								g_Player[i].pos = g_Player[i].pos_old;
							}
						}
					}
				}

				// �`���[�g���A���̓����蔻��
				{
					TUTORIAL *mark = GetMark();

					BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w/10, g_Player[i].h/10,
						mark->pos, mark->w/10, mark->h/10);
					// �������Ă���H
					if (ans == TRUE)
					{
						// �����������̏���
						g_goal = TRUE;
					}
				}

				{
					TUTORIAL *dummy = GetDummy();
					// �G�l�~�[�̐��������蔻����s��
					for (int j = 0; j < DUMMY_MAX; j++)
					{
						// �����Ă�G�l�~�[�Ɠ����蔻�������
						if (dummy[j].use == TRUE)
						{
							BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
								dummy[j].pos, dummy[j].w, dummy[j].h);
							// �������Ă���H
							if (ans == TRUE)
							{
								// �����������̏���
								g_Player[i].pos = g_Player[i].pos_old;
							}
						}
					}
				}


				// �_���[�W�̕\���ʒu
				{
					g_Player[i].damagepos = D3DXVECTOR3(g_Player[i].pos.x, g_Player[i].pos.y - g_Player[i].damagemove, 0.0f);
				}


				// �U������
				if (GetKeyboardTrigger(DIK_N))
				{// Space��������A�X�L��A���g��
					g_Player[i].skillusing = 0;
					SKILL *skill = GetSkillB();
					// �X�L��B���g���Ă邩�ǂ���
					for (int k = 0; k < SKILLB_MAX; k++)
					{
						if (skill[k].use == TRUE)
						{
							g_Player[i].skillusing++;
						}
					}
					// �g���ĂȂ��Ȃ�X�L��A�𔭓�
					if (g_Player[i].skillusing < 1)
					{
						D3DXVECTOR3 pos = g_Player[i].skillpos_a;
						int damage = g_Player[i].atk;
						SetSkillA(pos, damage);		// �P����
					}
				}

				if (GetKeyboardRepeat(DIK_M))
				{// M�����Ȃ���A�U���͈͂𒲐�
					g_Player[i].skillusing = 0;
					SKILL *skill = GetSkillA();
					// �X�L��A���g���Ă邩�ǂ���
					for (int k = 0; k < SKILLA_MAX; k++)
					{
						if (skill[k].use == TRUE)
						{
							g_Player[i].skillusing++;
						}
					}
					// �g���ĂȂ��Ȃ�X�L��B�𔭓�
					if (g_Player[i].skillusing < 1)
					{
						g_Player[i].skilluse = TRUE;
						// �ړ��L�[��������A�U���͈͂𒲐�
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
				{// M��������A�X�L��B���g��
					g_Player[i].skillusing = 0;
					SKILL *skill = GetSkillA();
					// �X�L��A���g���Ă邩�ǂ���
					for (int k = 0; k < SKILLA_MAX; k++)
					{
						if (skill[k].use == TRUE)
						{
							g_Player[i].skillusing++;
						}
					}
					// �g���ĂȂ��Ȃ�X�L��B�𔭓�
					if (g_Player[i].skillusing < 1)
					{
						D3DXVECTOR3 pos = g_Player[i].skillpos_b;
						int damage = g_Player[i].atk - 15;
						SetSkillB(pos, damage);		// �P����
						g_Player[i].skilluse = FALSE;
					}
				}
			}
		}

#ifdef _DEBUG
		// �f�o�b�O�\��
		PrintDebugProc("Player No%d  X:%f Y:%f\n", i, g_Player[i].pos.x, g_Player[i].pos.y);
		PrintDebugProc("Player No%d  ST:%d \n", i, g_Player[i].ST);
		PrintDebugProc("Player No%d  HP:%d \n", i, g_Player[i].HP);



#endif

	}
	
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
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

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)		// ���̃v���C���[���g���Ă���H
		{									// Yes
			// �v���C���[�̉e��`��
			DrawPlayerShadow(i);

			if (g_Player[i].skilluse == TRUE)
			{// �X�L���̍U���͈͂ƃX�y����`��
				DrawSkillAim(i);
				DrawSkillSpell(i);
			}
			
			if (g_Player[i].dashing == TRUE)
			{// �v���C���[�̕��g��`��
				DrawPlayerOffset(i);
			}
			
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[i].texNo]);

			//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Player[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
			float py = g_Player[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
			float pw = g_Player[i].w;		// �v���C���[�̕\����
			float ph = g_Player[i].h;		// �v���C���[�̕\������

			// �A�j���[�V�����p
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Player[i].alpha),
				g_Player[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// �v���C���[�\���̂̐擪�A�h���X���擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}


//=============================================================================
// �ړ��̌��ʂ��擾
//=============================================================================
BOOL GetGoal(void)
{
	return g_goal;
}


//=============================================================================
// �v���C���[�̉e��`��
//=============================================================================
void DrawPlayerShadow(int no)
{
	BG *bg = GetBG();

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].shadowtexNo]);

	//�e�̈ʒu��e�N�X�`�����W�𔽉f
	float px = g_Player[no].pos.x - bg->pos.x;		// �e�̕\���ʒuX
	float py = g_Player[no].pos.y - bg->pos.y + 5;	// �e�̕\���ʒuY
	float pw = g_Player[no].w;		// �e�̕\����
	float ph = g_Player[no].h;		// �e�̕\������

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, 1.0f, 1.0f, 1.0f, 1.0f,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}

//=============================================================================
// �v���C���[�̕��g��`��
//=============================================================================
void DrawPlayerOffset(int no)
{
	BG *bg = GetBG();
	float alpha = 0.0f;

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].texNo]);

	for (int j = PLAYER_OFFSET_CNT - 1; j >= 0; j--)
	{
		//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Player[no].offset[j].x - bg->pos.x;	// �v���C���[�̕\���ʒuX
		float py = g_Player[no].offset[j].y - bg->pos.y;	// �v���C���[�̕\���ʒuY
		float pw = g_Player[no].w;		// �v���C���[�̕\����
		float ph = g_Player[no].h;		// �v���C���[�̕\������

		// �A�j���[�V�����p
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
		float tx = (float)(g_Player[no].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
		float ty = (float)(g_Player[no].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W


		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha),
			g_Player[no].rot.z);

		alpha += (1.0f / PLAYER_OFFSET_CNT);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}

//=============================================================================
// �X�L���̍U���͈͂�`��
//=============================================================================
void DrawSkillAim(int no)
{
	BG *bg = GetBG();

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].skilltexNo]);

	//�X�L���̈ʒu��e�N�X�`�����W�𔽉f
	float px = g_Player[no].skillpos_b.x - bg->pos.x;		// �X�L���̕\���ʒuX
	float py = g_Player[no].skillpos_b.y - bg->pos.y;		// �X�L���̕\���ʒuY
	float pw = TEXTURE_WIDTH_2;			// �X�L���̕\����
	float ph = TEXTURE_HEIGHT_2;		// �X�L���̕\������

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
void DrawSkillSpell(int no)
{
	BG *bg = GetBG();

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].spelltexNo]);

	//�ʒu��e�N�X�`�����W�𔽉f
	float px = g_Player[no].pos.x - bg->pos.x;		// �\���ʒuX
	float py = g_Player[no].pos.y - bg->pos.y;		// �\���ʒuY
	float pw = g_Player[no].w*3;		// �\����
	float ph = g_Player[no].h*3;		// �\������

	// �A�j���[�V�����p
	float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_2;	// �e�N�X�`���̕�
	float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_2;	// �e�N�X�`���̍���
	float tx = (float)(g_Player[no].spellpatAnim % TEXTURE_PATTERN_DIVIDE_X_2) * tw;	// �e�N�X�`���̍���X���W
	float ty = (float)(g_Player[no].spellpatAnim / TEXTURE_PATTERN_DIVIDE_X_2) * th;	// �e�N�X�`���̍���Y���W

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}

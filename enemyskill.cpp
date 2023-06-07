//=============================================================================
//

//
//=============================================================================
#include "enemyskill.h"
#include "sprite.h"
#include "player.h"
#include "collision.h"
#include "damage.h"
#include "sound.h"
#include "bg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH_1				(200)	// �L�����T�C�Y
#define TEXTURE_HEIGHT_1			(150)	// 
#define TEXTURE_WIDTH_2				(80)	// �L�����T�C�Y
#define TEXTURE_HEIGHT_2			(80)	// 
#define TEXTURE_WIDTH_3				(200)	// �L�����T�C�Y
#define TEXTURE_HEIGHT_3			(200)	// 

#define TEXTURE_MAX					(3)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X_1	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_1	(12)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_1			(TEXTURE_PATTERN_DIVIDE_X_1*TEXTURE_PATTERN_DIVIDE_Y_1)	// �A�j���[�V�����p�^�[����
#define TEXTURE_PATTERN_DIVIDE_X_2	(10)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_2	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_2			(TEXTURE_PATTERN_DIVIDE_X_2*TEXTURE_PATTERN_DIVIDE_Y_2)	// �A�j���[�V�����p�^�[����
#define TEXTURE_PATTERN_DIVIDE_X_3	(5)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_3	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_3			(TEXTURE_PATTERN_DIVIDE_X_3*TEXTURE_PATTERN_DIVIDE_Y_3)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/TEXTURE/enemyskill01.png",
	"data/TEXTURE/enemyskill02.png",
	"data/TEXTURE/enemyskill03.png",
};

static BOOL	  g_Load = FALSE;		// ���������s�������̃t���O
static ENEMYSKILL g_Skilla[ENEMYSKILLA_MAX];	// �X�L���\����
static ENEMYSKILL g_Skillb[ENEMYSKILLB_MAX];	// �X�L���\����
static ENEMYSKILL g_Skillc[ENEMYSKILLC_MAX];	// �X�L���\����


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemySkill(void)
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


	// �X�L��A�\���̂̏�����
	for (int i = 0; i < ENEMYSKILLA_MAX; i++)
	{
		g_Skilla[i].use   = FALSE;			// ���g�p
		g_Skilla[i].w     = TEXTURE_WIDTH_1;
		g_Skilla[i].h     = TEXTURE_HEIGHT_1;
		g_Skilla[i].pos   = D3DXVECTOR3(300, 300.0f, 0.0f);
		g_Skilla[i].rot   = D3DXVECTOR3(0.0f, 0.0f, 1.57f);
		g_Skilla[i].texNo = 0;

		g_Skilla[i].countAnim = 0;
		g_Skilla[i].patternAnim = 0;

		g_Skilla[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ�������
	
		g_Skilla[i].damage = 0;
		g_Skilla[i].time = 0;
		g_Skilla[i].angle = 0.0f;
	}
	
	// �X�L��B�\���̂̏�����
	for (int i = 0; i < ENEMYSKILLB_MAX; i++)
	{
		g_Skillb[i].use = FALSE;			// ���g�p
		g_Skillb[i].w = TEXTURE_WIDTH_2;
		g_Skillb[i].h = TEXTURE_HEIGHT_2;
		g_Skillb[i].pos = D3DXVECTOR3(300, 300.0f, 0.0f);
		g_Skillb[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Skillb[i].texNo = 1;

		g_Skillb[i].countAnim = 0;
		g_Skillb[i].patternAnim = 0;

		g_Skillb[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ�������

		g_Skillb[i].damage = 0;
		g_Skillb[i].time = 0;
		g_Skillb[i].angle = 0.0f;
	}
	
	// �X�L��C�\���̂̏�����
	for (int i = 0; i < ENEMYSKILLC_MAX; i++)
	{
		g_Skillc[i].use = FALSE;			// ���g�p
		g_Skillc[i].w = TEXTURE_WIDTH_3;
		g_Skillc[i].h = TEXTURE_HEIGHT_3;
		g_Skillc[i].pos = D3DXVECTOR3(300, 300.0f, 0.0f);
		g_Skillc[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Skillc[i].texNo = 2;

		g_Skillc[i].countAnim = 0;
		g_Skillc[i].patternAnim = 0;

		g_Skillc[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ�������

		g_Skillc[i].damage = 0;
		g_Skillc[i].time = 0;
		g_Skillc[i].angle = 0.0f;
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemySkill(void)
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
void UpdateEnemySkill(void)
{
	BG *bg = GetBG();

	int skillCounta = 0;				// ���������X�L���̐�

	for (int i = 0; i < ENEMYSKILLA_MAX; i++)
	{
		if (g_Skilla[i].use == TRUE)	// ���̃X�L�����g���Ă���H
		{								// Yes
			// �A�j���[�V����  
			
			g_Skilla[i].countAnim++;
			if ((g_Skilla[i].countAnim % ANIM_WAIT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_Skilla[i].patternAnim = (g_Skilla[i].patternAnim + 1) % ANIM_PATTERN_NUM_1;
			}
			
			if (g_Skilla[i].countAnim == (ANIM_WAIT*ANIM_PATTERN_NUM_1))
			{
				g_Skilla[i].use = FALSE;
				g_Skilla[i].countAnim = 0;
			}
			

			// �����蔻�菈��
			{
				PLAYER *player = GetPlayer();

				// �G�l�~�[�̐��������蔻����s��
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// �����Ă�G�l�~�[�Ɠ����蔻�������
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Skilla[i].pos, g_Skilla[i].w, g_Skilla[i].h - 10,
							player[j].pos, player[j].w, player[j].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							if (g_Skilla[i].hituse == TRUE)
							{
								player[j].hitted = TRUE; 
								player[j].HP -= g_Skilla[i].damage;
								if (player[j].HP < 1)
								{
									player[j].HP = 0;
									player[j].use = FALSE;
								}
								PlaySound(SOUND_LABEL_SE_hit);
								D3DXVECTOR3 pos = player[j].damagepos;
								SetDamage(pos, g_Skilla[i].damage);
								g_Skilla[i].hituse = FALSE;		// �����蔻��t���O
							}
						}
					}
				}
			}

			skillCounta++;
		}
	}


	int skillCountb = 0;				// ���������X�L���̐�

	for (int i = 0; i < ENEMYSKILLB_MAX; i++)
	{
		if (g_Skillb[i].use == TRUE)	// ���̃X�L�����g���Ă���H
		{								// Yes
			// �A�j���[�V����  

			g_Skillb[i].countAnim++;
			if ((g_Skillb[i].countAnim % ANIM_WAIT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_Skillb[i].patternAnim = (g_Skillb[i].patternAnim + 1) % ANIM_PATTERN_NUM_2;
			}

			// �X�L���̈ړ�����
			{
				PLAYER *player = GetPlayer();
				g_Skillb[i].time++;
				
				D3DXVECTOR3 pos = player[0].pos - g_Skillb[i].pos;	// �����ƃv���C���[�̍��������߂Ă���
				float angle = atan2f(pos.y, pos.x) + D3DX_PI;		// ���̍������g���Ċp�x�����߂Ă���
				float speed = -3.0f;								// �X�s�[�h�͂�����ƒx�����Ă݂�

				if (g_Skillb[i].time < 120)
				{
					g_Skillb[i].pos.x += cosf(angle) * speed;						// angle�̕����ֈړ�
					g_Skillb[i].pos.y += sinf(angle) * speed;						// angle�̕����ֈړ�
				}
				
				if (g_Skillb[i].time == 120)
				{
					g_Skillb[i].angle = angle;
				}
				
				if (g_Skillb[i].time > 120 && g_Skillb[i].time < 270)
				{
					g_Skillb[i].pos.x += cosf(g_Skillb[i].angle) * speed;						// angle�̕����ֈړ�
					g_Skillb[i].pos.y += sinf(g_Skillb[i].angle) * speed;						// angle�̕����ֈړ�
				}
				
				if (g_Skillb[i].time > 270)
				{
					g_Skillb[i].use = FALSE;
					g_Skillb[i].time = 0;
				}
			}
		
			//// ��ʊO�܂Ői�񂾁H
			//if (g_Skillb[i].pos.y < bg->pos.y + (-g_Skillb[i].h / 2))		// �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
			//{
			//	g_Skillb[i].use = false;
			//}
			//if (g_Skillb[i].pos.y > bg->pos.y + (SCREEN_HEIGHT + g_Skillb[i].h / 2))	// �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
			//{
			//	g_Skillb[i].use = false;
			//}

			// �����蔻�菈��
			{
				PLAYER *player = GetPlayer();

				// �G�l�~�[�̐��������蔻����s��
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// �����Ă�G�l�~�[�Ɠ����蔻�������
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Skillb[i].pos, g_Skillb[i].w, g_Skillb[i].h,
							player[j].pos, player[j].w, player[j].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							if (g_Skillb[i].hituse == TRUE)
							{
								g_Skillb[i].use = FALSE;		// �����蔻��t���O
								player[j].hitted = TRUE;
								player[j].HP -= g_Skillb[i].damage;
								if (player[j].HP < 1)
								{
									player[j].HP = 0;
									player[j].use = FALSE;
								}
								PlaySound(SOUND_LABEL_SE_hit);
								D3DXVECTOR3 pos = player[j].damagepos;
								SetDamage(pos, g_Skillb[i].damage);
								g_Skillb[i].hituse = FALSE;		// �����蔻��t���O
							}
						}
					}
				}
			}

			skillCountb++;
		}
	}


	int skillCountc = 0;				// ���������X�L���̐�

	for (int i = 0; i < ENEMYSKILLC_MAX; i++)
	{
		if (g_Skillc[i].use == TRUE)	// ���̃X�L�����g���Ă���H
		{								// Yes
			// �A�j���[�V����  

			g_Skillc[i].countAnim++;
			if ((g_Skillc[i].countAnim % ANIM_WAIT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_Skillc[i].patternAnim = (g_Skillc[i].patternAnim + 1) % ANIM_PATTERN_NUM_3;
			}

			if (g_Skillc[i].countAnim == (ANIM_WAIT*ANIM_PATTERN_NUM_3))
			{
				g_Skillc[i].use = FALSE;
				g_Skillc[i].countAnim = 0;
			}


			// �����蔻�菈��
			{
				PLAYER *player = GetPlayer();

				// �G�l�~�[�̐��������蔻����s��
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// �����Ă�G�l�~�[�Ɠ����蔻�������
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Skillc[i].pos, g_Skillc[i].w, g_Skillc[i].h,
							player[j].pos, player[j].w, player[j].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							if (g_Skillc[i].hituse == TRUE)
							{
								player[j].hitted = TRUE;
								player[j].HP -= g_Skillc[i].damage;
								if (player[j].HP < 1)
								{
									player[j].HP = 0;
									player[j].use = FALSE;
								}
								PlaySound(SOUND_LABEL_SE_hit);
								D3DXVECTOR3 pos = player[j].damagepos;
								SetDamage(pos, g_Skillc[i].damage);
								g_Skillc[i].hituse = FALSE;		// �����蔻��t���O
							}
						}
					}
				}
			}

			skillCountc++;
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemySkill(void)
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

	for (int i = 0; i < ENEMYSKILLA_MAX; i++)
	{
		if (g_Skilla[i].use == TRUE)		// ���̃X�L�����g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skilla[i].texNo]);

			//�X�L���̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Skilla[i].pos.x - bg->pos.x;	// �X�L���̕\���ʒuX
			float py = g_Skilla[i].pos.y - bg->pos.y;	// �X�L���̕\���ʒuY
			float pw = g_Skilla[i].w;		// �X�L���̕\����
			float ph = g_Skilla[i].h;		// �X�L���̕\������

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_1;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_1;	// �e�N�X�`���̍���
			float tx = (float)(g_Skilla[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X_1) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Skilla[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X_1) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, 
				px, py, pw, ph, 
				tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				g_Skilla[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	for (int i = 0; i < ENEMYSKILLB_MAX; i++)
	{
		if (g_Skillb[i].use == TRUE)		// ���̃X�L�����g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skillb[i].texNo]);

			//�X�L���̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Skillb[i].pos.x - bg->pos.x;	// �X�L���̕\���ʒuX
			float py = g_Skillb[i].pos.y - bg->pos.y;	// �X�L���̕\���ʒuY
			float pw = g_Skillb[i].w;		// �X�L���̕\����
			float ph = g_Skillb[i].h;		// �X�L���̕\������

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_2;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_2;	// �e�N�X�`���̍���
			float tx = (float)(g_Skillb[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X_2) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Skillb[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X_2) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				g_Skillb[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	for (int i = 0; i < ENEMYSKILLC_MAX; i++)
	{
		if (g_Skillc[i].use == TRUE)		// ���̃X�L�����g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skillc[i].texNo]);

			//�X�L���̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Skillc[i].pos.x - bg->pos.x;	// �X�L���̕\���ʒuX
			float py = g_Skillc[i].pos.y - bg->pos.y;	// �X�L���̕\���ʒuY
			float pw = g_Skillc[i].w;		// �X�L���̕\����
			float ph = g_Skillc[i].h;		// �X�L���̕\������

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_3;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_3;	// �e�N�X�`���̍���
			float tx = (float)(g_Skillc[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X_3) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Skillc[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X_3) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				g_Skillc[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


//=============================================================================
// �X�L���\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMYSKILL *GetEnemySkillA(void)
{
	return &g_Skilla[0];
}

ENEMYSKILL *GetEnemySkillB(void)
{
	return &g_Skillb[0];
}


//=============================================================================
// �X�L��A�̎g�p�ݒ�
//=============================================================================
void SetEnemySkillA(D3DXVECTOR3 pos,int damage)
{
	// �������g�p�̃X�L����������������g��Ȃ�( =����Ȏg���ĂȂ����Ď� )
	for (int i = 0; i < ENEMYSKILLA_MAX; i++)
	{
		if (g_Skilla[i].use == FALSE)		// ���g�p��Ԃ̃X�L����������
		{
			g_Skilla[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Skilla[i].hituse = TRUE;		// �����蔻��t���O
			g_Skilla[i].pos = pos;			// ���W���Z�b�g
			g_Skilla[i].damage = damage;
			PlaySound(SOUND_LABEL_SE_enemyskill_a);
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

//=============================================================================
// �X�L��B�̎g�p�ݒ�
//=============================================================================
void SetEnemySkillB(D3DXVECTOR3 pos, int damage)
{
	// �������g�p�̃X�L����������������g��Ȃ�( =����Ȏg���ĂȂ����Ď� )
	for (int i = 0; i < ENEMYSKILLB_MAX; i++)
	{
		if (g_Skillb[i].use == FALSE)		// ���g�p��Ԃ̃X�L����������
		{
			g_Skillb[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Skillb[i].hituse = TRUE;		// �����蔻��t���O
			g_Skillb[i].pos = pos;			// ���W���Z�b�g
			g_Skillb[i].damage = damage;
			PlaySound(SOUND_LABEL_SE_enemyskill_b);
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

//=============================================================================
// �X�L��C�̎g�p�ݒ�
//=============================================================================
void SetEnemySkillC(D3DXVECTOR3 pos, int damage)
{
	// �������g�p�̃X�L����������������g��Ȃ�( =����Ȏg���ĂȂ����Ď� )
	for (int i = 0; i < ENEMYSKILLC_MAX; i++)
	{
		if (g_Skillc[i].use == FALSE)		// ���g�p��Ԃ̃X�L����������
		{
			g_Skillc[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Skillc[i].hituse = TRUE;		// �����蔻��t���O
			g_Skillc[i].pos = pos;			// ���W���Z�b�g
			g_Skillc[i].damage = damage;
			PlaySound(SOUND_LABEL_SE_enemyskill_c1);
			PlaySound(SOUND_LABEL_SE_enemyskill_c2);
			PlaySound(SOUND_LABEL_SE_enemyskill_c3);
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

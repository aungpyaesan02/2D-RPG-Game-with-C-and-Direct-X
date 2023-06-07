//=============================================================================
//

//
//=============================================================================
#include "skill.h"
#include "sprite.h"
#include "enemy.h"
#include "player.h"
#include "collision.h"
#include "damage.h"
#include "sound.h"
#include "bg.h"
#include "tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH_1				(150)	// �L�����T�C�Y
#define TEXTURE_HEIGHT_1			(150)	// 
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X_1	(5)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_1	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_1			(TEXTURE_PATTERN_DIVIDE_X_1*TEXTURE_PATTERN_DIVIDE_Y_1)	// �A�j���[�V�����p�^�[����
#define TEXTURE_PATTERN_DIVIDE_X_2	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_2	(5)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_2			(TEXTURE_PATTERN_DIVIDE_X_2*TEXTURE_PATTERN_DIVIDE_Y_2)	// �A�j���[�V�����p�^�[����

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
	"data/TEXTURE/skillA.png",
	"data/TEXTURE/skillB.png",
 };

static BOOL	  g_Load = FALSE;		// ���������s�������̃t���O
static SKILL g_Skilla[SKILLA_MAX];	// �X�L���\����
static SKILL g_Skillb[SKILLB_MAX];	// �X�L���\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSkill(void)
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
	for (int i = 0; i < SKILLA_MAX; i++)
	{
		g_Skilla[i].use   = FALSE;			// ���g�p
		g_Skilla[i].w     = TEXTURE_WIDTH_1;
		g_Skilla[i].h     = TEXTURE_HEIGHT_1;
		g_Skilla[i].pos   = D3DXVECTOR3(300, 300.0f, 0.0f);
		g_Skilla[i].rot   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Skilla[i].texNo = 0;

		g_Skilla[i].countAnim = 0;
		g_Skilla[i].patternAnim = 0;

		g_Skilla[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ�������
		g_Skilla[i].hit = FALSE;
		g_Skilla[i].damage = 0;

	}
	
	// �X�L��B�\���̂̏�����
	for (int i = 0; i < SKILLB_MAX; i++)
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
		g_Skillb[i].hit = FALSE;
		g_Skillb[i].damage = 0;
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSkill(void)
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
void UpdateSkill(void)
{
	BG *bg = GetBG();

	int skillCounta = 0;				// ���������X�L���̐�

	for (int i = 0; i < SKILLA_MAX; i++)
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
				ENEMY *enemy = GetEnemy();

				// �G�l�~�[�̐��������蔻����s��
				for (int j = 0; j < ENEMY_MAX; j++)
				{
					// �����Ă�G�l�~�[�Ɠ����蔻�������
					if (enemy[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Skilla[i].pos, g_Skilla[i].w/2, g_Skilla[i].h/2,
							enemy[j].pos, enemy[j].w-20, enemy[j].h-20);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							if (enemy[j].hit == TRUE)
							{
								enemy[j].hitted = TRUE;
								enemy[j].HP -= g_Skilla[i].damage;
								if (enemy[j].HP < 1)
								{
									enemy[j].HP = 0;
									enemy[j].use = FALSE;
									enemy[j].pop = 0.0f;
								}
								PlaySound(SOUND_LABEL_SE_hit);
								D3DXVECTOR3 pos = enemy[j].damagepos;
								SetDamage(pos, g_Skilla[i].damage);
								enemy[j].hit = FALSE;		// �����蔻��t���O
							}
						}
					}
				}
			}

			// �`���[�g���A���̓����蔻��
			{
				TUTORIAL *dummy = GetDummy();

				// �����Ă�_�~�[�Ɠ����蔻�������
				if (dummy[0].use == TRUE)
				{
					BOOL ans = CollisionBB(g_Skilla[i].pos, g_Skilla[i].w/2, g_Skilla[i].h/2,
						dummy[0].pos, dummy[0].w, dummy[0].h);
					// �������Ă���H
					if (ans == TRUE)
					{
						// �����������̏���
						PlaySound(SOUND_LABEL_SE_hit);
						g_Skilla[i].hit = TRUE;
					}
				}
			}

			skillCounta++;
		}
	}

	int skillCountb = 0;				// ���������X�L���̐�

	for (int i = 0; i < SKILLB_MAX; i++)
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

			if (g_Skillb[i].countAnim == (ANIM_WAIT*ANIM_PATTERN_NUM_2*2))
			{
				g_Skillb[i].use = FALSE;
				g_Skillb[i].countAnim = 0;
			}

			// �����蔻�菈��
			{
				ENEMY *enemy = GetEnemy();

				// �G�l�~�[�̐��������蔻����s��
				for (int j = 0; j < ENEMY_MAX; j++)
				{
					// �����Ă�G�l�~�[�Ɠ����蔻�������
					if (enemy[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Skillb[i].pos, g_Skillb[i].w, g_Skillb[i].h,
							enemy[j].pos, enemy[j].w, enemy[j].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							if (enemy[j].hit == TRUE)
							{
								enemy[j].hitted = TRUE;
								enemy[j].HP -= g_Skillb[i].damage;
								if (enemy[j].HP < 1)
								{
									enemy[j].HP = 0;
									enemy[j].use = FALSE;
									enemy[j].pop = 0.0f;
								}
								PlaySound(SOUND_LABEL_SE_hit);
								D3DXVECTOR3 pos = enemy[j].damagepos;
								SetDamage(pos, g_Skillb[i].damage);
								enemy[j].hit = FALSE;		// �����蔻��t���O
							}
						}
					}
				}
			}

			// �`���[�g���A���̓����蔻��
			{
				TUTORIAL *dummy = GetDummy();

				// �����Ă�_�~�[�Ɠ����蔻�������
				if (dummy[1].use == TRUE)
				{
					BOOL ans = CollisionBB(g_Skillb[i].pos, g_Skillb[i].w, g_Skillb[i].h,
						dummy[1].pos, dummy[1].w, dummy[1].h);
					// �������Ă���H
					if (ans == TRUE)
					{
						// �����������̏���
						PlaySound(SOUND_LABEL_SE_hit);
						g_Skillb[i].hit = TRUE;
					}
				}
			}

			skillCountb++;
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSkill(void)
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

	for (int i = 0; i < SKILLA_MAX; i++)
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

	for (int i = 0; i < SKILLB_MAX; i++)
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

}


//=============================================================================
// �X�L���\���̂̐擪�A�h���X���擾
//=============================================================================
SKILL *GetSkillA(void)
{
	return &g_Skilla[0];
}

SKILL *GetSkillB(void)
{
	return &g_Skillb[0];
}

//=============================================================================
// �X�L��A�̎g�p�ݒ�
//=============================================================================
void SetSkillA(D3DXVECTOR3 pos, int damage)
{
	// �������g�p�̃X�L����������������g��Ȃ�( =����Ȏg���ĂȂ����Ď� )
	for (int i = 0; i < SKILLA_MAX; i++)
	{
		if (g_Skilla[i].use == FALSE)		// ���g�p��Ԃ̃X�L����������
		{
			g_Skilla[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Skilla[i].pos = pos;			// ���W���Z�b�g
			g_Skilla[i].damage = damage;
			PlaySound(SOUND_LABEL_SE_skill_a);
			
			ENEMY *enemy = GetEnemy();
			for (int j = 0; j < ENEMY_MAX; j++)
			{
				enemy[j].hit = TRUE;
			}
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

//=============================================================================
// �X�L��B�̎g�p�ݒ�
//=============================================================================
void SetSkillB(D3DXVECTOR3 pos, int damage)
{
	// �������g�p�̃X�L����������������g��Ȃ�( =����Ȏg���ĂȂ����Ď� )
	for (int i = 0; i < SKILLB_MAX; i++)
	{
		if (g_Skillb[i].use == FALSE)		// ���g�p��Ԃ̃X�L����������
		{
			g_Skillb[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Skillb[i].pos = pos;			// ���W���Z�b�g
			g_Skillb[i].damage = damage;
			PlaySound(SOUND_LABEL_SE_skill_b);
			
			ENEMY *enemy = GetEnemy();
			for (int j = 0; j < ENEMY_MAX; j++)
			{
				enemy[j].hit = TRUE;
			}
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

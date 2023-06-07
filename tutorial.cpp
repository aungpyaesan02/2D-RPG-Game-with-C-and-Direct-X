//=============================================================================
//

//
//=============================================================================
#include "sprite.h"
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "bg.h"
#include "sound.h"
#include "player.h"
#include "skill.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(150)		// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(150)		// (SCREEN_HEIGHT)	// �w�i�T�C�Y
#define TEXTURE_WIDTH_3				(840)		// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT_3			(3871)		// (SCREEN_HEIGHT)	// �w�i�T�C�Y

#define TEXTURE_MAX					(7)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(5)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TEXTURE_PATTERN_DIVIDE_X_3	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_3	(5)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_3			(TEXTURE_PATTERN_DIVIDE_X_3*TEXTURE_PATTERN_DIVIDE_Y_3)	// �A�j���[�V�����p�^�[����

#define ANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/mark.png",
	"data/TEXTURE/dummy.png",
	"data/TEXTURE/aurora.png",
	"data/TEXTURE/introduction.png",
	"data/TEXTURE/window_2.png",
	"data/TEXTURE/window_3.png",
	"data/TEXTURE/window_4.png",
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static TUTORIAL	g_Window[WINDOW_MAX];
static TUTORIAL	g_Mark;
static TUTORIAL	g_Dummy[DUMMY_MAX];
static TUTORIAL	g_Aurora;

static int g_time;		// ���Ԃɍ��킹�ĕ`�悳����p(�ŏ��̎�)
static int g_wait;		// ���Ԃɍ��킹�ĕ`�悳����p(����e�N�X�`����؂�ւ�鎞)
static int g_step;		// �`���[�g���A���̐i�s�i�K���Ǘ�����p
static int g_sound;		// SE��炷�t���O

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTutorial(void)
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


	// �ϐ��̏�����
	g_Aurora.use = TRUE;
	g_Aurora.w = 2200;
	g_Aurora.h = 1800;
	g_Aurora.pos = D3DXVECTOR3(700.0f, 400.0f, 0.0f);
	g_Aurora.rot = D3DXVECTOR3(0.0f, 0.0f, 3.14f);
	g_Aurora.texNo = 2;

	g_Aurora.countAnim = 0;
	g_Aurora.patternAnim = 0;


	g_Mark.use = FALSE;
	g_Mark.w = TEXTURE_WIDTH;
	g_Mark.h = TEXTURE_HEIGHT;
	g_Mark.pos = D3DXVECTOR3(700.0f, 600.0f, 0.0f);
	g_Mark.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Mark.texNo = 0;

	g_Mark.countAnim = 0;
	g_Mark.patternAnim = 0;

	for (int i = 0; i < DUMMY_MAX; i++)
	{
		g_Dummy[i].use = FALSE;
		g_Dummy[i].w = TEXTURE_WIDTH;
		g_Dummy[i].h = TEXTURE_HEIGHT;
		g_Dummy[i].pos = D3DXVECTOR3(500.0f*(i+1), 450.0f+(i*200.0f), 0.0f);
		g_Dummy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Dummy[i].texNo = 1;

		g_Dummy[i].countAnim = 0;
		g_Dummy[i].patternAnim = 0;
	}
	
	for (int i = 0; i < WINDOW_MAX; i++)
	{
		g_Window[i].use = FALSE;
		g_Window[i].w = TEXTURE_WIDTH_3;
		g_Window[i].h = TEXTURE_HEIGHT_3;
		g_Window[i].pos = D3DXVECTOR3(600.0f, 2435.5f, 0.0f);
		g_Window[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Window[i].texNo = 3;

		g_Window[i].countAnim = 0;
		g_Window[i].patternAnim = 0;
	}

	g_time = 0;
	g_wait = 0;
	g_sound = 0;
	g_step = 0;

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTutorial(void)
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
void UpdateTutorial(void)
{
	// �A�j���[�V����  
	{
		// �}�[�N�̏���
		g_Mark.countAnim++;
		if ((g_Mark.countAnim % ANIM_WAIT) == 0)
		{
			// �p�^�[���̐؂�ւ�
			g_Mark.patternAnim = (g_Mark.patternAnim + 1) % ANIM_PATTERN_NUM;
		}
		
		// �I�[�����̏���
		g_Aurora.countAnim++;
		if ((g_Aurora.countAnim % ANIM_WAIT) == 0)
		{
			// �p�^�[���̐؂�ւ�
			g_Aurora.patternAnim = (g_Aurora.patternAnim + 1) % ANIM_PATTERN_NUM_3;
		}
	}

	g_time++;
	//�������Ԃ����ĕ`�悳����
	if (g_time > 80 && g_step == 0)
	{
		for (int i = 0; i < WINDOW_MAX; i++)
		{
			g_Window[i].use = TRUE;
			g_sound = 0;		// SE��炷
		}
		g_step++;
	}

	// SE��炷����
	{
		if (g_time > 80)
		{
			g_sound++;
		}

		if (g_sound == 1)
		{
			PlaySound(SOUND_LABEL_SE_system_03);
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorial(void)
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


	// �`���[�g���A����ʂ̃E�B���h�E��`��
	{
		for (int i = 0; i < WINDOW_MAX; i++)
		{
			BG *bg = GetBG();

			if (g_Window[i].use == TRUE)
			{	// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Window[i].texNo]);

				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_Window[i].pos.x - bg->pos.x;	// �E�B���h�E�̕\���ʒuX
				float py = g_Window[i].pos.y - bg->pos.y;	// �E�B���h�E�̕\���ʒuY
				float pw = g_Window[i].w;		// �E�B���h�E�̕\����
				float ph = g_Window[i].h;		// �E�B���h�E�̕\������

				float tw = 1.0f;	// �e�N�X�`���̕�
				float th = 1.0f;	// �e�N�X�`���̍���
				float tx = 0.0f;	// �e�N�X�`���̍���X���W
				float ty = 0.0f;	// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					g_Window[i].rot.z);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}

	// �I�[������`��
	if (g_Aurora.use == TRUE)
	{
		BG *bg = GetBG();

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Aurora.texNo]);

		//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Aurora.pos.x - bg->pos.x;	// �E�B���h�E�̕\���ʒuX
		float py = g_Aurora.pos.y - bg->pos.y;	// �E�B���h�E�̕\���ʒuY
		float pw = g_Aurora.w;		// �E�B���h�E�̕\����
		float ph = g_Aurora.h;		// �E�B���h�E�̕\������

		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X_3;	// �e�N�X�`���̕�
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_3;	// �e�N�X�`���̍���
		float tx = (float)(g_Aurora.patternAnim % TEXTURE_PATTERN_DIVIDE_X_3) * tw;	// �e�N�X�`���̍���X���W
		float ty = (float)(g_Aurora.patternAnim / TEXTURE_PATTERN_DIVIDE_X_3) * th;	// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),
			g_Aurora.rot.z);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


}


//=============================================================================
// �`���[�g���A���\���̂̐擪�A�h���X���擾
//=============================================================================
TUTORIAL *GetDummy(void)
{
	return &g_Dummy[0];
}

TUTORIAL *GetMark(void)
{
	return &g_Mark;
}


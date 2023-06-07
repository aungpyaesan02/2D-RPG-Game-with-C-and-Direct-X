//=============================================================================
//

//
//=============================================================================
#include "sprite.h"
#include "ui.h"
#include "bg.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(270)	// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(137)	// (SCREEN_HEIGHT)	// �w�i�T�C�Y
#define TEXTURE_WIDTH_3				(166)	// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT_3			(19)	// (SCREEN_HEIGHT)	// �w�i�T�C�Y

#define TEXTURE_MAX					(4)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

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
	"data/TEXTURE/ui_1.png",
	"data/TEXTURE/ui_hp.png",
	"data/TEXTURE/ui_st.png",
	"data/TEXTURE/ui_2.png",
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static UI	g_UI[UI_MAX];


//=============================================================================
// ����������
//=============================================================================
HRESULT InitUI(void)
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
	for (int i = 0; i < UI_MAX; i++)
	{
		g_UI[i].use = TRUE;
		g_UI[i].w = TEXTURE_WIDTH;
		g_UI[i].h = TEXTURE_HEIGHT;
		g_UI[i].pos = D3DXVECTOR3(32.0f, 540.0f, 0.0f);
		g_UI[i].texNo = i;

		g_UI[i].pw = TEXTURE_WIDTH;

		g_UI[i].countAnim = 0;
		g_UI[i].patternAnim = 0;
	}
	
	g_UI[1].w = TEXTURE_WIDTH_3;
	g_UI[1].h = TEXTURE_HEIGHT_3;
	g_UI[1].pos = D3DXVECTOR3(108.5f, 601.76f, 0.0f);
	g_UI[1].pw = TEXTURE_WIDTH_3;

	g_UI[2].w = TEXTURE_WIDTH_3;
	g_UI[2].h = TEXTURE_HEIGHT_3;
	g_UI[2].pos = D3DXVECTOR3(108.5f, 634.0f, 0.0f);
	g_UI[2].pw = TEXTURE_WIDTH_3;

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUI(void)
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
void UpdateUI(void)
{
	for (int i = 0; i < UI_MAX; i++)
	{
		if (g_UI[i].use == TRUE)
		{
			PLAYER *player = GetPlayer();
	
			for (int j = 0; j < PLAYER_MAX; j++)
			{
				// ���O��HP�ɍ��킹�ĕ`��
				if (i == 1)
				{
					g_UI[i].pw = g_UI[i].w * ((float)player[j].HP / (float)player[j].HP_MAX);
				}
				
				// ���O��ST�ɍ��킹�ĕ`��
				if (i == 2)
				{
					g_UI[i].pw = g_UI[i].w * ((float)player[j].ST / (float)player[j].ST_MAX);
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUI(void)
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



	// �v���C���[�̃X�e�[�^�X��`��
	{
		for (int i = 0; i < UI_MAX; i++)
		{
			if (g_UI[i].use == TRUE)
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[i].texNo]);

				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_UI[i].pos.x;	// �E�B���h�E�̕\���ʒuX
				float py = g_UI[i].pos.y;	// �E�B���h�E�̕\���ʒuY
				float pw = g_UI[i].pw;		// �E�B���h�E�̕\����
				float ph = g_UI[i].h;		// �E�B���h�E�̕\������

				float tw = 1.0f;	// �e�N�X�`���̕�
				float th = 1.0f;	// �e�N�X�`���̍���
				float tx = 0.0f;	// �e�N�X�`���̍���X���W
				float ty = 0.0f;	// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteLTColor(g_VertexBuffer,
					px, py, pw, ph,
					tx, ty, tw, th,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}



}


//=============================================================================
// �`���[�g���A���\���̂̐擪�A�h���X���擾
//=============================================================================
UI *GetUI(void)
{
	return &g_UI[0];
}


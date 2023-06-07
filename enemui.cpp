//=============================================================================
//

//
//=============================================================================
#include "sprite.h"
#include "enemui.h"
#include "bg.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define UI_MAX				(ENEMY_MAX)		// �E�B���h�E�̐�

#define TEXTURE_WIDTH				(120)	// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(15)	// (SCREEN_HEIGHT)	// �w�i�T�C�Y

#define TEXTURE_MAX					(3)		// �e�N�X�`���̐�

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
	"data/TEXTURE/enemui_1.png",
	"data/TEXTURE/enemui_hp.png",
	"data/TEXTURE/enemui_2.png",
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static ENEMUI	g_UI_1[UI_MAX];
static ENEMUI	g_UI_hp[UI_MAX];
static ENEMUI	g_UI_2[UI_MAX];


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemUI(void)
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
	
	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateEnemUI(void)
{
	for (int i = 0; i < UI_MAX; i++)
	{
		ENEMY *enemy = GetEnemy();
		
		// hp�̈ʒu���Z�b�g����
		g_UI_1[i].pos = D3DXVECTOR3(enemy[i].pos.x - (enemy[i].w/2), enemy[i].pos.y - (enemy[i].h/2) - 20, 0.0f);
		g_UI_hp[i].pos = D3DXVECTOR3(enemy[i].pos.x - (enemy[i].w/2), enemy[i].pos.y - (enemy[i].h/2) - 20, 0.0f);
		g_UI_2[i].pos = D3DXVECTOR3(enemy[i].pos.x - (enemy[i].w/2), enemy[i].pos.y - (enemy[i].h/2) - 20, 0.0f);

		// HP�������Ă��鎞�����\��
		if (enemy[i].HP < enemy[i].HP_MAX)
		{
			g_UI_1[i].use = TRUE;
			g_UI_hp[i].use = TRUE;
			g_UI_2[i].use = TRUE;
		}
	
		// ���O��HP�ɍ��킹�ĕ`��
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
// �`�揈��
//=============================================================================
void DrawEnemUI(void)
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



	// �G�l�~�[��HP��`��
	{
		for (int i = 0; i < UI_MAX; i++)
		{
			BG *bg = GetBG();

			if (g_UI_1[i].use == TRUE)
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI_1[i].texNo]);

				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_UI_1[i].pos.x - bg->pos.x;	// �E�B���h�E�̕\���ʒuX
				float py = g_UI_1[i].pos.y - bg->pos.y;	// �E�B���h�E�̕\���ʒuY
				float pw = g_UI_1[i].pw;		// �E�B���h�E�̕\����
				float ph = g_UI_1[i].h;		// �E�B���h�E�̕\������

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
		

			if (g_UI_hp[i].use == TRUE)
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI_hp[i].texNo]);

				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_UI_hp[i].pos.x - bg->pos.x;	// �E�B���h�E�̕\���ʒuX
				float py = g_UI_hp[i].pos.y - bg->pos.y;	// �E�B���h�E�̕\���ʒuY
				float pw = g_UI_hp[i].pw;		// �E�B���h�E�̕\����
				float ph = g_UI_hp[i].h;		// �E�B���h�E�̕\������

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


			if (g_UI_2[i].use == TRUE)
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI_2[i].texNo]);

				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_UI_2[i].pos.x - bg->pos.x;	// �E�B���h�E�̕\���ʒuX
				float py = g_UI_2[i].pos.y - bg->pos.y;	// �E�B���h�E�̕\���ʒuY
				float pw = g_UI_2[i].pw;		// �E�B���h�E�̕\����
				float ph = g_UI_2[i].h;		// �E�B���h�E�̕\������

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


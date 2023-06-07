//=============================================================================
//

//
//=============================================================================
#include "bg.h"
#include "sprite.h"
#include "collision.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH_1				(SCREEN_WIDTH)		// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT_1			(SCREEN_HEIGHT)		// (SCREEN_HEIGHT)	// �w�i�T�C�Y
#define TEXTURE_WIDTH_2				(1400)		// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT_2			(4500)		// (SCREEN_HEIGHT)	// �w�i�T�C�Y

#define TEXTURE_MAX					(15)		// �e�N�X�`���̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/BG.png",
	"data/TEXTURE/block_bg.png",
	"data/TEXTURE/mountain_3.png",
	"data/TEXTURE/mountain_2.png",
	"data/TEXTURE/mountain_1.png",
	"data/TEXTURE/ruin.png",
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static BG	g_BG;
static BG	g_OBJ[OBJ_MAX];


// ����������
//=============================================================================
HRESULT InitBG(void)
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
	g_BG.w     = TEXTURE_WIDTH_1;
	g_BG.h     = TEXTURE_HEIGHT_1;
	g_BG.pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_BG.texNo = 0;

	g_OBJ[0].w = 200;
	g_OBJ[0].h = 200;
	g_OBJ[0].pos = D3DXVECTOR3(100.0f, 440.0f, 0.0f);
	g_OBJ[0].texNo = 1;

	g_OBJ[1].w = 200;
	g_OBJ[1].h = 200;
	g_OBJ[1].pos = D3DXVECTOR3(860.0f, 440.0f, 0.0f);
	g_OBJ[1].texNo = 1;

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBG(void)
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
void UpdateBG(void)
{
	int mode = GetMode();
	if (mode == MODE_TUTORIAL)
	{
		g_BG.texNo = 14;
		g_BG.w = TEXTURE_WIDTH_2;
		g_BG.h = TEXTURE_HEIGHT_2;
	}

	//for (int i = 0; i < OBJ_MAX; i++)
	//{// �v���C���[�Ƃ̓����蔻��
	//	
	//	PLAYER *player = GetPlayer();
	//	// ��Q���̐��������蔻����s��
	//	for (int j = 0; j < PLAYER_MAX; j++)
	//	{
	//		BOOL ans = CollisionBB(g_OBJ[i].pos, g_OBJ[i].w, g_OBJ[i].h,
	//			player[j].pos, player[j].w, player[j].h);
	//		// �������Ă���H
	//		if (ans == TRUE)
	//		{
	//			// �����������̏���
	//			player[j].pos = player[i].pos_old;
	//		}
	//	}
	//}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBG(void)
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


	// MAP�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BG.texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			-g_BG.pos.x, -g_BG.pos.y, g_BG.w, g_BG.h,
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// MAP�̏�Q����`��
	{
		int mode = GetMode();
		if (mode == MODE_GAME)
		{
			for (int i = 0; i < OBJ_MAX; i++)
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_OBJ[i].texNo]);

				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_OBJ[i].pos.x - g_BG.pos.x;		// ��Q���̕\���ʒuX
				float py = g_OBJ[i].pos.y - g_BG.pos.y;		// ��Q���̕\���ʒuY
				float pw = g_OBJ[i].w;		// ��Q���̕\����
				float ph = g_OBJ[i].h;		// ��Q���̕\������

				float tw = 1.0f;	// �e�N�X�`���̕�
				float th = 1.0f;	// �e�N�X�`���̍���
				float tx = 0.0f;	// �e�N�X�`���̍���X���W
				float ty = 0.0f;	// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					0.0f);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}


}


//=============================================================================
// BG�\���̂̐擪�A�h���X���擾
//=============================================================================
BG *GetBG(void)
{
	return &g_BG;
}

BG *GetOBJ(void)
{
	return &g_OBJ[0];
}





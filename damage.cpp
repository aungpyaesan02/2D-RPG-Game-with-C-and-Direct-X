//=============================================================================
//
//=============================================================================
#include "damage.h"
#include "sprite.h"
#include "bg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(DAMAGE_DW)	// �����̃T�C�Y
#define TEXTURE_HEIGHT				(DAMAGE_DH)	// 
#define TEXTURE_MAX					(1)			// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/TEXTURE/number16x32.png",
};

static BOOL		g_Load = FALSE;		// ���������s�������̃t���O
static DAMAGE	g_Damage[DAMAGE_MAX];


//=============================================================================
// ����������
//=============================================================================
HRESULT InitDamage(void)
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
	for (int i = 0; i < DAMAGE_MAX; i++)
	{
		g_Damage[i].w = TEXTURE_WIDTH;
		g_Damage[i].h = TEXTURE_HEIGHT;
		g_Damage[i].pos = D3DXVECTOR3(DAMAGE_DX, DAMAGE_DY, 0.0f);
		g_Damage[i].texNo = 0;
		g_Damage[i].use = FALSE;
		g_Damage[i].damage = 0;	// �_���[�W��������
		g_Damage[i].move = -1.0f;
		g_Damage[i].alpha = 1.0f;
		g_Damage[i].time = 0;
	}

	g_Load = TRUE;		// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
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
// �`�揈��
//=============================================================================
void DrawDamage(void)
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

	for (int j = 0; j < DAMAGE_MAX; j++)
	{
		if (g_Damage[j].use == TRUE)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Damage[j].texNo]);

			// ��������������
			int number = g_Damage[j].damage;

			for (int i = 0; i < DAMAGE_DIGIT; i++)
			{
				// ����\�����錅�̐���
				float x = (float)(number % 10);

				// ���̌���
				number /= 10;

				// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = (g_Damage[j].pos.x - bg->pos.x) - g_Damage[i].w*i;	// �X�R�A�̕\���ʒuX
				float py = g_Damage[j].pos.y - bg->pos.y;						// �X�R�A�̕\���ʒuY
				float pw = g_Damage[j].w;						// �X�R�A�̕\����
				float ph = g_Damage[j].h;						// �X�R�A�̕\������

				float tw = 1.0f / 10;					// �e�N�X�`���̕�
				float th = 1.0f / 1;					// �e�N�X�`���̍���
				float tx = x * tw;						// �e�N�X�`���̍���X���W
				float ty = 0.0f;						// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteLTColor(g_VertexBuffer,
					px, py, pw, ph,
					tx, ty, tw, th,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Damage[j].alpha));

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);

			}
		}
	}
}


//=============================================================================
// �X�R�A�����Z����
// ����:add :�ǉ�����_���B�}�C�i�X���\
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
// �_���[�W�̎g�p�ݒ�
//=============================================================================
void SetDamage(D3DXVECTOR3 pos, int damage)
{
	// �������g�p�̃_���[�W��������������g��Ȃ�( =����Ȏg���ĂȂ����Ď� )
	for (int i = 0; i < DAMAGE_MAX; i++)
	{
		if (g_Damage[i].use == FALSE)		// ���g�p��Ԃ̃_���[�W��������
		{
			g_Damage[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Damage[i].pos = pos;			// ���W���Z�b�g
			g_Damage[i].damage = damage;	// �_���[�W���Z�b�g
			
			return;							// �������Z�b�g�����̂ŏI������
		}
	}
}


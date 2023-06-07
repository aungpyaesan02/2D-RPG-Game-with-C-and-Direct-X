//=============================================================================
//

//
//=============================================================================
#include "result.h"
#include "sprite.h"
#include "input.h"
#include "fade.h"
#include "sound.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 

#define TEXTURE_WIDTH_2				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT_2			(2007)			// 

#define TEXTURE_WIDTH_3				(150)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT_3			(60)			// 

#define TEXTURE_MAX					(5)				// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/resultBG.png",
	"data/TEXTURE/resultWORD.png",
	"data/TEXTURE/gameclear.png",
	"data/TEXTURE/gameover.png",
	"data/TEXTURE/window_5.png",
};


static BOOL		g_Load = FALSE;		// ���������s�������̃t���O
static STAFF	g_Result;
static RESULTBG	g_ResultBG;
static RESULT	g_Clear;
static RESULT	g_Over;
static KEY		g_Key;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
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
	g_ResultBG.w = TEXTURE_WIDTH;
	g_ResultBG.h = TEXTURE_HEIGHT;
	g_ResultBG.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_ResultBG.texNo = 0;

	// �ϐ��̏�����
	g_Result.w     = TEXTURE_WIDTH_2;
	g_Result.h     = TEXTURE_HEIGHT_2;
	g_Result.pos   = D3DXVECTOR3(0.0f, 730.0f, 0.0f);	// �X�N���[�������߂����̍��W
	g_Result.pos2  = D3DXVECTOR3(0.0f, ((TEXTURE_HEIGHT/2)+30)-TEXTURE_HEIGHT_2, 0.0f);	// �X�N���[�����I��������̍��W
	g_Result.texNo = 1;
	g_Result.offset = 1.5f;

	// �ϐ��̏�����
	g_Clear.w = TEXTURE_WIDTH;
	g_Clear.h = TEXTURE_HEIGHT;
	g_Clear.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Clear.alpha = 0.0f;
	g_Clear.texNo = 2;
	g_Clear.bgmuse = 0;
	g_Clear.use = FALSE;

	// �ϐ��̏�����
	g_Over.w = TEXTURE_WIDTH;
	g_Over.h = TEXTURE_HEIGHT;
	g_Over.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Over.alpha = 0.0f;
	g_Over.texNo = 3;
	g_Over.bgmuse = 0;
	g_Over.use = FALSE;

	// �ϐ��̏�����
	g_Key.w = TEXTURE_WIDTH_3;
	g_Key.h = TEXTURE_HEIGHT_3;
	g_Key.pos = D3DXVECTOR3(800.0f, 470.0f, 0.0f);
	g_Key.texNo = 4;


	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
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
void UpdateResult(void)
{
	int mode = GetMode();
	if (mode == MODE_RESULT)
	{	//�������X�N���[�������鏈��
		g_Result.pos.y -= g_Result.offset;

		if (g_Result.pos.y < g_Result.pos2.y)
		{
			g_Result.pos.y = g_Result.pos2.y;
		}

		if (g_Result.pos == g_Result.pos2)
		{
			g_Result.wait++;
			
			if (g_Result.wait > 270)
			{
				SetFade(FADE_OUT, MODE_TITLE);
			}
		}
	
		// ENTER��������A�X�L�b�v����
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
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

	int mode = GetMode();
	if (mode == MODE_RESULT)
	{
		// ���U���g�̔w�i��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_ResultBG.texNo]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				g_ResultBG.pos.x, g_ResultBG.pos.y, g_ResultBG.w, g_ResultBG.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		// �����X�N���[����`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Result.texNo]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				g_Result.pos.x, g_Result.pos.y, g_Result.w, g_Result.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	
		// �L�[��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Key.texNo]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				g_Key.pos.x, g_Key.pos.y, g_Key.w, g_Key.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

	}

	if (mode == MODE_GAME)
	{
		// GAMECLEAR��`��
		if (g_Clear.use == TRUE)
		{
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Clear.texNo]);

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteLTColor(g_VertexBuffer,
					g_Clear.pos.x, g_Clear.pos.y, g_Clear.w, g_Clear.h,
					0.0f, 0.0f, 1.0f, 1.0f,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Clear.alpha));

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}
		}

		// GAMEOVER��`��
		if (g_Over.use == TRUE)
		{
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Over.texNo]);

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteLTColor(g_VertexBuffer,
					g_Over.pos.x, g_Over.pos.y, g_Over.w, g_Over.h,
					0.0f, 0.0f, 1.0f, 1.0f,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Over.alpha));

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}
}




//=============================================================================
// ���U���g�ŕ\������_���Z�b�g
//=============================================================================
void SetGAMECLEAR(void)
{
	if (g_Clear.use == TRUE)
	{
		g_Clear.wait++;
		g_Clear.alpha += 0.008f;
		g_Clear.bgmuse++;

		if (g_Clear.alpha > 1.0f)
		{
			g_Clear.alpha = 1.0f;
		}

		if (g_Clear.bgmuse == 10)
		{
			PlaySound(SOUND_LABEL_SE_gameclear);
		}

		if (g_Clear.wait > 300)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}



//=============================================================================
// ���U���g�ŕ\������_���Z�b�g
//=============================================================================
void SetGAMEOVER(void)
{
	if (g_Over.use == TRUE)
	{
		g_Over.wait++;
		g_Over.alpha += 0.008f;
		g_Over.bgmuse++;
		
		if (g_Over.alpha > 1.0f)
		{
			g_Over.alpha = 1.0f;
		}

		if (g_Over.bgmuse == 10)
		{
			PlaySound(SOUND_LABEL_SE_gameover1);
			PlaySound(SOUND_LABEL_SE_gameover1);
		}

		if (g_Over.wait > 300)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}


//=============================================================================
// �v���C���[�\���̂̐擪�A�h���X���擾
//=============================================================================
RESULT *GetGameClear(void)
{

	return &g_Clear;
}

//=============================================================================
// �v���C���[�\���̂̐擪�A�h���X���擾
//=============================================================================
RESULT *GetGameOver(void)
{

	return &g_Over;
}

//=============================================================================
//
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct STAFF
{
	D3DXVECTOR3		pos;		// �|���S���̍��W(�X�N���[���O)
	D3DXVECTOR3		pos2;		// �|���S���̍��W(�X�N���[����)
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
	float			offset;		// �e�N�X�`���̈ړ���
	int				wait;
};

struct RESULTBG
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
};

struct RESULT
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
	float			alpha;		// �g�p���Ă���e�N�X�`���ԍ�
	int				bgmuse;		// SE��炷�t���O
	BOOL			use;
	int				wait;
};

struct KEY
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

void SetGAMECLEAR(void);
void SetGAMEOVER(void);
RESULT *GetGameClear(void);
RESULT *GetGameOver(void);


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
#define DAMAGE_MAX			(100)		// �X�R�A�̍ő�l / �g����_���[�W�̐�
#define DAMAGE_DIGIT		(2)			// ����

#define	DAMAGE_DX			(600.0f)	// �\���ʒu
#define	DAMAGE_DY			(20.0f)		// 
#define	DAMAGE_DW			(30.0f)		// 
#define	DAMAGE_DH			(50.0f)		// 

struct DAMAGE
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	D3DXVECTOR3		rot;		// �|���S���̉�]��
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	BOOL			use;		// �_���[�W�\���t���O
	int				damage;		// �_���[�W

	float			move;		// �_���[�W�̈ړ���
	float			alpha;		// �_���[�W�̕s�����x
	int				time;		// �A�j���p
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDamage(void);
void UninitDamage(void);
void UpdateDamage(void);
void DrawDamage(void);

int GetDamage(void);
void AddDamage(int add, int no);
void SetDamage(D3DXVECTOR3 pos, int damage);



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
#define WINDOW_MAX				(1)		// �E�B���h�E�̐�
#define DUMMY_MAX				(2)		// �_�~�[�̐�

struct TUTORIAL
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	D3DXVECTOR3		rot;		// �|���S���̉�]��
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
	BOOL			use;

	int				countAnim;	// �A�j���[�V�����J�E���g
	int				patternAnim;// �A�j���[�V�����p�^�[���i���o�[
};

struct WINDOW
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	D3DXVECTOR3		rot;		// �|���S���̉�]��
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
	BOOL			use;

	int				countAnim;	// �A�j���[�V�����J�E���g
	int				patternAnim;// �A�j���[�V�����p�^�[���i���o�[
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

TUTORIAL *GetDummy(void);
TUTORIAL *GetMark(void);


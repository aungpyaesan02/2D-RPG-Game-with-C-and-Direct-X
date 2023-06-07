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
#define ENEMY_MAX		(17)		// �G�l�~�[��Max�l��

enum
{
	ENEM_STD_DOWN,
	ENEM_STD_LEFT,
	ENEM_STD_RIGHT,
	ENEM_STD_UP,
	ENEM_DIR_DOWN,
	ENEM_DIR_LEFT,
	ENEM_DIR_RIGHT,
	ENEM_DIR_UP,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct	LINEAR_INTERPOLATION		// ���`��ԃf�[�^�̍\����
{
	D3DXVECTOR3		pos;			// ���W
	D3DXVECTOR3		rot;			// ��]��
	D3DXVECTOR3		scl;			// �g�嗦
	float			time;			// �ړ�����
};


struct ENEMY
{
	D3DXVECTOR3		pos;			// �|���S���̍��W
	D3DXVECTOR3		pos_old;		// �ړ��O�̃|���S���̍��W
	D3DXVECTOR3		rot;			// �|���S���̉�]��
	D3DXVECTOR3		scl;			// �|���S���̊g�嗦
	BOOL			use;			// true:�g���Ă���  false:���g�p
	float			w, h;			// ���ƍ���
	int				countAnim;		// �A�j���[�V�����J�E���g
	int				patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int				texNo;			// �e�N�X�`���ԍ�
	D3DXVECTOR3		move;			// �ړ����x
	float			pop;			// �o�ꂳ����^�C�~���O
	BOOL			homing;			// true:�z�[�~���O����  false:���Ȃ�

	int				dir;			// �����i0:�� 1:�� 2:�E 3:�� 4:�� 5:�� 6:�E 7:��j
	BOOL			moving;			// �ړ����t���O
	int				shadowtexNo;	// �e�̃e�N�X�`���ԍ�
	
	float			time;			// ���`��ԗp
	int				moveTblNo;		// �s���f�[�^�̃e�[�u���ԍ�
	int				tblMax;			// ���̃e�[�u���̃f�[�^��

	int				HP, HP_MAX;		// HP,HP_MAX
	int				atk;			// �U����

	D3DXVECTOR3		skillpos;		// �X�L��A�̍��W
	float			skillmove;		// �X�L��A�̈ړ���
	int				PerSecond;		// �P�ʎ��ԓ�����ɍs��������p

	D3DXVECTOR3		damagepos;		// �_���[�W�̍��W
	float			damagemove;		// �_���[�W�̈ړ���

	BOOL			hit;			// TRUE:�����蔻����s��  FALSE:�s��Ȃ�

	float			alpha;			// �s�����x
	BOOL			hitted;			// �U�������t���O
	int				hittedtime;		// �A�j���[�V������`�掞�p
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);




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
#define PLAYER_MAX			(1)		// �v���C���[��Max�l��

#define	PLAYER_OFFSET_CNT	(10)	// 16���g

enum
{
	CHAR_STD_DOWN,
	CHAR_STD_LEFT,
	CHAR_STD_RIGHT,
	CHAR_STD_UP,
	CHAR_DIR_DOWN,
	CHAR_DIR_LEFT,
	CHAR_DIR_RIGHT,
	CHAR_DIR_UP,
};


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct PLAYER
{
	D3DXVECTOR3		pos;			// �|���S���̍��W
	D3DXVECTOR3 pos_old;			// �ړ��O�̃|���S���̍��W
	D3DXVECTOR3		rot;			// �|���S���̉�]��
	BOOL			use;			// true:�g���Ă���  false:���g�p
	float			w, h;			// ���ƍ���
	float			countAnim;		// �A�j���[�V�����J�E���g
	int				patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int				texNo;			// �e�N�X�`���ԍ�
	D3DXVECTOR3		move;			// �ړ����x

	int				dir;			// �����i0:�� 1:�� 2:�E 3:�� 4:�� 5:�� 6:�E 7:��j
	BOOL			moving;			// �ړ����t���O

	BOOL			dashing;		// �_�b�V�����t���O
	D3DXVECTOR3		offset[PLAYER_OFFSET_CNT];		// �c���|���S���̍��W

	int				shadowtexNo;	// �e�̃e�N�X�`���ԍ�

	D3DXVECTOR3		skillpos_a;		// �X�L��A(space�L�[/���{�^��)�̍��W
	float			skillmove_a;	// �X�L��A(space�L�[/���{�^��)�̈ړ���

	D3DXVECTOR3		skillpos_b;		// �X�L��B(m�L�[/���{�^��)�̍��W
	float			skillmove_b;	// �X�L��B(m�L�[/���{�^��)�̈ړ���
	int				skilltexNo;		// �X�L���̃e�N�X�`���ԍ�
	BOOL			skilluse;		// �X�L���`���[�W���t���O
	int				skillusing;		// �g�p���̃X�L�����v�Z����p

	int				spellcouAnim;	// �A�j���[�V�����J�E���g
	int				spellpatAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int				spelltexNo;		// �e�N�X�`���ԍ�


	int				HP,HP_MAX;		// HP,HP_MAX
	int				ST,ST_MAX;		// �X�^�~�i,�X�^�~�i_MAX
	int				atk;			// �U����
	int				PerSecond;		// �P�ʎ��ԓ�����ɍs��������p

	D3DXVECTOR3		damagepos;		// �_���[�W�̍��W
	float			damagemove;		// �_���[�W�̈ړ���

	float			alpha;			// �s�����x
	BOOL			hitted;			// �U�������t���O
	int				hittedtime;		// �A�j���[�V������`�掞�p
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);
BOOL GetGoal(void);




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
#define ENEMYSKILLA_MAX		(100)			// �X�L��A��Max��
#define ENEMYSKILLB_MAX		(300)			// �X�L��B��Max��
#define ENEMYSKILLC_MAX		(100)			// �X�L��C��Max��


// �X�L���\����
struct ENEMYSKILL
{
	BOOL					use;					// true:�g���Ă���  false:���g�p
	BOOL					hituse;					// true:�����蔻��s��  false:�s��Ȃ�
	float					w, h;					// ���ƍ���
	D3DXVECTOR3				pos;					// �X�L���̍��W
	D3DXVECTOR3				rot;					// �X�L���̉�]��
	D3DXVECTOR3				move;					// �X�L���̈ړ���
	int						countAnim;				// �A�j���[�V�����J�E���g
	int						patternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	int						texNo;					// ���Ԗڂ̃e�N�X�`���[���g�p����̂�

	int						damage;
	int						time;					// �X�L��B�̑��ݎ���
	float					angle;					// �X�L��B�̋O���̊p�x
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemySkill(void);
void UninitEnemySkill(void);
void UpdateEnemySkill(void);
void DrawEnemySkill(void);

ENEMYSKILL *GetEnemySkillA(void);
ENEMYSKILL *GetEnemySkillB(void);
void SetEnemySkillA(D3DXVECTOR3 pos, int damage);
void SetEnemySkillB(D3DXVECTOR3 pos, int damage);
void SetEnemySkillC(D3DXVECTOR3 pos, int damage);


//=============================================================================
//
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_title,		// �^�C�g��BGM
	SOUND_LABEL_BGM_tutorial,	// �`���[�g���A��BGM
	SOUND_LABEL_BGM_map,		// �}�b�vBGM
	SOUND_LABEL_BGM_bossbattle,	// �{�X��BGM
	SOUND_LABEL_BGM_ending,		// �G���f�B���O��
	SOUND_LABEL_SE_system_01,	// �V�X�e�����ʉ�01
	SOUND_LABEL_SE_system_02,	// �V�X�e�����ʉ�02
	SOUND_LABEL_SE_system_03,	// �V�X�e�����ʉ�03
	SOUND_LABEL_SE_skill_a,		// �v���C���[�X�L��A
	SOUND_LABEL_SE_skill_b	,	// �v���C���[�X�L��B
	SOUND_LABEL_SE_enemyskill_a,// �G�l�~�[�X�L��A
	SOUND_LABEL_SE_enemyskill_b,// �G�l�~�[�X�L��B
	SOUND_LABEL_SE_enemyskill_c1,// �G�l�~�[�X�L��C1
	SOUND_LABEL_SE_enemyskill_c2,// �G�l�~�[�X�L��C2
	SOUND_LABEL_SE_enemyskill_c3,// �G�l�~�[�X�L��C3
	SOUND_LABEL_SE_object,		// �I�u�W�F�N�g���ʉ�
	SOUND_LABEL_SE_hit,			// �q�b�g���ʉ�
	SOUND_LABEL_SE_gameclear,	// GAMECLEAR���ʉ�
	SOUND_LABEL_SE_gameover1,	// GAMEOVER���ʉ�1
	SOUND_LABEL_SE_gameover2,	// GAMEOVER���ʉ�2
	SOUND_LABEL_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);


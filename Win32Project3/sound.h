//=============================================================================
//
// �T�E���h���� [sound.h]
// 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_
#include < XAudio2.h >
#include <XAudio2fx.h>
#include <XAPO.h>
//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
// �T�E���h�t�@�C��
enum SOUND_LABEL
{
	BUTTON,
	GAME_BGM,
	TITLE_BGM,
	RESULT_BGM,
	GAMEOVER_BGM,
	SOUND_LABEL_MAX,
};
//**********************************************************************
//
// �\���̐錾
//
//**********************************************************************

// �p�����[�^
typedef struct
{
	char *pFilename;	// �t�@�C����
	int nCntLoop;		// ���[�v�J�E���g
} SOUNDPARAM;

struct MyEffectParameter
{
	//ReflectionDelay:�_�C���N�g�p�X�ɑ΂��鏉�����˂̒x�����ԁ@�w��͈͉\�͈́@0 ~ 300�~���b
	int ReflectionsDelay;
	//ReverbDelay:�������˂ɑ΂��郊�o�[�u�̒x���A�w��͈͂�0�`85�~���b
	int ReverbDelay;
	//RearDelay:������o�͋y�щE����o�͂̒x���@�w��\�͈� 0�`85�~���b
	int RearDelay;
	//PositionLeft:�V�~�����[�V������Ԃɂ����鎋���҂ɑ΂��鍶���͂̈ʒu�B�@�w��\�͈͂� 0 ~ 30�@�i�P�ʂȂ��j
	int PositionLeft;
	//PositionRight: PositionLeft�Ɠ��l�E���͂ɂ̂݉e�����܂��B�@�w��\�͈͂� 0 ~ 30(�P�ʂȂ��j
	int PositionRight;
	//PositionMatrixRight&Left:�������王���҂܂ŋ����ɂ��C���v���b�V�����𑝌�������B�@�w��\�͈͂� 0 ~ 30�i�P�ʂȂ��j
	int PositionMatrixLeft;
	int PositionMatrixRight;
	//EarlyDiffusion:�X�̕ǂ̔��˓����𐧌䂵�܂��B��������ȕ\�ʃV�~�����[�g����ɂ͏����Ȓl��ݒ肵�A
	//               �U�����̕\�ʂ��V�~�����[�g����ɂ͑傫�Ȓl��ݒ�B�@�w��\�͈� 0 ~15(�P�ʂȂ��j
	int EarlyDiffusion;
	//LateDiffusion:�X�̕ǂ̃��o�[�u�����𐧌�B�w��\�͈� 0 ~ 15(�P�ʂȂ��j
	int LateDiffusion;
	//LowEQGain: 1kHz�ɂ����錸�����Ԃ���ɂ��Ē���g���̌������Ԃ𒲐�. �w��\�͈� 0 ~ 12(�P�ʂȂ�)
	int LowEQGain;
	//LowEQCutoff:�p�����[�^�[�ɂ�萧�䂳��郍�[�p�X�t�B���^�[�̐ܓ_���g����ݒ�B�w�芿���͈� 0 ~ 9 (�P�ʂȂ�)
	int LowEQCutoff;
	//HighEQGain:�������Ԃ���ɂ��č����g���̌������Ԃ𒲐��B 0 �̏ꍇ�@�����g���̉���1khz�̏ꍇ�Ɠ��������Ō������܂��B�@�w��\�͈� 0 ~ 8(�P�ʂȂ�)
	int HighEQGain;
	//HighEQCutoff: HighEQGain�p�����[�^�[�ɂ�萧�䂳���n�C�p�X�t�B���^�[�̐ړ_���g����ݒ�B�@�w��\�͈� 0 ~ 14(�P�ʂȂ��j
	int HighEQCutoff;
	//RoomFilterFreq:�����G�t�F�N�g�̃��[�p�X�t�B���^�[�̐ړ_���g����ݒ�B�@�w��\�͈͂� 20 ~ 20,000Hz
	float RoomFilterFreq;
	//RoomFilterMain:�������˂ƌ���t�B�[���h�c���̗����ɓK�p����郍�[�p�X�t�B���^�[�̃p�X �o���h���x���x����ݒ�B�@�w��\�͈� -100 ~ 0dB
	float RoomFilterMain;
	//RoomfiletHF:�ړ_���g���ł̏������˂ƌ���t�B�[���h�c���̗����ɓK�p����郍�[�p�X�t�B���^�[�̃p�X�o���h���x���x����ݒ�B�w��\�͈� -100 ~ 0dB
	float RoomFilterHF;
	//ReflectionGain:�������˂̋��x/���x���𒲐����܂��B�w��\�͈͂�-100~20dB�B
	float ReflectionsGain;
	//ReverbGain:���o�[�u�̋��x/���x���𒲐����܂��B�w��\�͈͂� -100 ~ 20dB
	float ReverbGain;
	//DecayTime:1kHz�ɂ�����c���������Ԃł��B����́A�t���X�P�[���̓��͐M����60dB��������܂ł̎��Ԃł��B�w��\�͈͂�0.1 ~ �����i�b�j�ł��B
	float DecayTime;
	//����t�B�[���h�c���̃��[�h���x�𐧌䂵�܂��B���F (colorless) �̋�Ԃł́ADensity ���ő�l (100) �ɐݒ肷��K�v������܂��BDensity ������������ƁA
	//�T�E���h�͂����������� (�����`�t�B���^�[���K�p���ꂽ��) �ɂȂ�܂��B�T�C�����V�~�����[�V��������Ƃ��ɗL���ȃG�t�F�N�g�ł��B�w��\�͈͂� 0 �` 100 �ŁA�P�ʂ̓p�[�Z���g�ł��B
	float Density;
	//RoomSize:������Ԃ̌�������̃T�C�Y�ł��B�@�w��\�͈͂� 1(30.48cm) ~ 100�t�B�[�g(30.48m)
	float RoomSize;
	//WetDryMix:���̕ύX����ĂȂ��T�E���h�ƂȂ�o�͂̊����@���e�l�@0 ~ 100;
	float WetDryMix;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

class CSound
{
public:
	CSound(){}
	~CSound(){}
public:
	HRESULT InitSound(HWND hWnd);
	MyEffectParameter InitEffectSound(void);
	void UninitSound(void);
	HRESULT EffectSound(SOUND_LABEL label, MyEffectParameter Effectinfo);
	HRESULT PlaySound(SOUND_LABEL label);
	HRESULT SoundVolume(SOUND_LABEL label, float Volume);
	HRESULT DirectionSound(SOUND_LABEL label, float LeftVolume, float RightVolume);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

	//================================================================================
	//
	// [ �`�����N�̃`�F�b�N ]
	//
	// � ���� �
	//
	// �hFile              : �t�@�C���n���h��
	// �format             : �t�@�C���t�H�[�}�b�g
	// �pChunkSize         : �`�����N�T�C�Y
	// �pChunkDataPosition : �`�����N�f�[�^�̏ꏊ
	//
	// � �߂�l �
	//
	// �HRESULT�^ : �G���[�`�F�b�N�p
	//
	//�y �ڍ� �z
	//
	// ��`�����N�f�[�^���`�F�b�N����
	//
	//================================================================================

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);



	//================================================================================
	//
	// [ �`�����N�̓ǂݍ��� ]
	//
	// � ���� �
	//
	// �hFile          : �t�@�C���n���h��
	// �pBuffer        : �ǂݍ��ݐ�o�b�t�@
	// �dwBuffersize   : �ǂݍ��ݐ�o�b�t�@�̃T�C�Y
	// �dwBufferoffset : �ǂݍ��ݐ�o�b�t�@�̃I�t�Z�b�g
	//
	// � �߂�l �
	//
	// �HRESULT�^ : �G���[�`�F�b�N�p
	//
	//�y �ڍ� �z
	//
	// ��`�����N�f�[�^��ǂݍ���
	//
	//================================================================================

	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//================================================================================
	//
	// [ �G�t�F�N�g�����֐�( �ʎw�� ) ]
	//
	// � ���� �
	//
	// �label : �w�b�_�Œ�`���ꂽ�T�E���h�t�@�C���̔ԍ�
	//
	// � �߂�l �
	//
	// ��Ȃ�
	//
	//�y �ڍ� �z
	//
	// ��G�t�F�N�g���ʂɖ����ɂ�����B
	//
	//================================================================================
	HRESULT DisableSoundEffect(SOUND_LABEL label);

	//================================================================================
	//
	// [ �G�t�F�N�g�L���֐�( �ʎw�� ) ]
	//
	// � ���� �
	//
	// �label : �w�b�_�Œ�`���ꂽ�T�E���h�t�@�C���̔ԍ�
	//
	// � �߂�l �
	//
	// ��Ȃ�
	//
	//�y �ڍ� �z
	//
	// ��G�t�F�N�g���ʂɗL���ɂ�����B
	//
	//================================================================================
	HRESULT EnableSoundEffect(SOUND_LABEL label);
private:
	IXAudio2 *g_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *g_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD g_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
	IUnknown * g_pXAPO;											//Effect�ݒ�pXAPO
	XAUDIO2_EFFECT_DESCRIPTOR g_descriptor;						//Effect�ݒ�
	XAUDIO2_EFFECT_CHAIN g_chain;								//Effect�ݒ�`�F�C��
																// �e���f�ނ̃p�����[�^
																// �e���f�ނ̃p�����[�^


	SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
	{
		{ "resource/sound/Button02.wav",0},
		{ "resource/sound/GameBGM.wav",1 },
		{ "resource/sound/TitleBGM.wav",1 },
		{ "resource/sound/ResultBGM.wav",1},
		{ "resource/sound/GameOverBGM.wav",1},
	};
};

#endif
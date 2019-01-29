//=============================================================================
//
// �T�E���h���� [sound.cpp]
// 
//
//=============================================================================


//**********************************************************************
//
// �C���N���[�h��
//
//**********************************************************************
#include "Sound.h"


//================================================================================
//
// [ �T�E���h�������֐� ]
//
// � ���� �
//
// �hWnd : �E�B���h�E�n���h��
//
// � �߂�l �
//
// �HRESULT�^ : �G���[�`�F�b�N�p
//
//�y �ڍ� �z
//
// ��T�E���h���g�p���邽�߂̏�����
//
//================================================================================

HRESULT CSound::InitSound(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (g_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �\�[�X�{�C�X�̐���
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aParam[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
		hr = XAudio2CreateVolumeMeter(&g_pXAPO);
		//�G�t�F�N�g�쐬
		hr = XAudio2CreateReverb(&g_pXAPO);


		g_descriptor.InitialState = true;
		/*output�̏ꏊ�������Ă���ׁA��������������������܂��������^��*/
		g_descriptor.OutputChannels = 2;
		g_descriptor.pEffect = g_pXAPO;


		g_chain.EffectCount = 1;
		g_chain.pEffectDescriptors = &g_descriptor;

		g_apSourceVoice[nCntSound]->SetEffectChain(&g_chain);
		g_pXAPO->Release();

	}

	return S_OK;
}



//================================================================================
//
// [ �T�E���h�I�������֐� ]
//
// � ���� �
//
// ��Ȃ�
//
// � �߂�l �
//
// ��Ȃ�
//
//�y �ڍ� �z
//
// ��T�E���h�̎g�p���I������Ƃ��̏���
//
//================================================================================

void CSound::UninitSound(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);

			// �\�[�X�{�C�X�̔j��
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;

			// �I�[�f�B�I�f�[�^�̊J��
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}

	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;

	if (g_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}

	// COM���C�u�����̏I������
	CoUninitialize();
}
//================================================================================
//
// InitEffectSound
//
// �G�t�F�N�g�������֐�
//
//================================================================================
MyEffectParameter CSound::InitEffectSound(void)
{
	MyEffectParameter Effectinfo;
	Effectinfo.ReflectionsDelay = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY;
	Effectinfo.ReverbDelay = XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY;
	Effectinfo.RearDelay = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY;
	Effectinfo.PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	Effectinfo.PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	Effectinfo.PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	Effectinfo.PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	Effectinfo.EarlyDiffusion = XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION;
	Effectinfo.LateDiffusion = XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION;
	Effectinfo.LowEQGain = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN;
	Effectinfo.LowEQCutoff = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF;
	Effectinfo.HighEQGain = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN;
	Effectinfo.HighEQCutoff = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF;
	Effectinfo.RoomFilterFreq = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ;
	Effectinfo.RoomFilterMain = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN;
	Effectinfo.RoomFilterHF = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF;
	Effectinfo.ReflectionsGain = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN;
	Effectinfo.ReverbGain = XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN;
	Effectinfo.DecayTime = XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME;
	Effectinfo.Density = XAUDIO2FX_REVERB_DEFAULT_DENSITY;
	Effectinfo.RoomSize = XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE;
	Effectinfo.WetDryMix = XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX;

	return Effectinfo;
}
//================================================================================
//
// EffectSound
//
// ������ : SoundLabel
// ������ : OutputNumber
// 
// �����I�ɂƂ��ƌ����Ă��邪�Aupdate�֐����œǂݍ��܂���Ɩ��񉹂̏��ɏ��������邽�߃o�O��B
// �Ȃ̂ŁA������Init�֐����ŏ�����������̃^�C�~���O�ɂ��̂��ǂ��B
//================================================================================
HRESULT CSound::EffectSound(SOUND_LABEL label, MyEffectParameter Effectinfo)
{
	HRESULT hr;

	XAUDIO2FX_REVERB_PARAMETERS reverbParameters;

	reverbParameters.ReflectionsDelay = Effectinfo.ReflectionsDelay;
	reverbParameters.ReverbDelay = Effectinfo.ReverbDelay;
	reverbParameters.RearDelay = Effectinfo.RearDelay;
	reverbParameters.PositionLeft = Effectinfo.PositionLeft;
	reverbParameters.PositionRight = Effectinfo.PositionRight;
	reverbParameters.PositionMatrixLeft = Effectinfo.PositionMatrixLeft;
	reverbParameters.PositionMatrixRight = Effectinfo.PositionMatrixRight;
	reverbParameters.EarlyDiffusion = Effectinfo.EarlyDiffusion;
	reverbParameters.LateDiffusion = Effectinfo.LateDiffusion;
	reverbParameters.LowEQGain = Effectinfo.LowEQGain;
	reverbParameters.LowEQCutoff = Effectinfo.LowEQCutoff;
	reverbParameters.HighEQGain = Effectinfo.HighEQGain;
	reverbParameters.HighEQCutoff = Effectinfo.HighEQCutoff;
	reverbParameters.RoomFilterFreq = Effectinfo.RoomFilterFreq;
	reverbParameters.RoomFilterMain = Effectinfo.RoomFilterMain;
	reverbParameters.RoomFilterHF = Effectinfo.RoomFilterHF;
	reverbParameters.ReflectionsGain = Effectinfo.ReflectionsGain;
	reverbParameters.ReverbGain = Effectinfo.ReverbGain;
	reverbParameters.DecayTime = Effectinfo.DecayTime;
	reverbParameters.Density = Effectinfo.Density;
	reverbParameters.RoomSize = Effectinfo.RoomSize;
	reverbParameters.WetDryMix = Effectinfo.WetDryMix;

	hr = g_apSourceVoice[label]->SetEffectParameters(0, &reverbParameters, sizeof(reverbParameters));

	return S_OK;
}
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
HRESULT CSound::DisableSoundEffect(SOUND_LABEL label)
{
	HRESULT hr;
	/*DisableEffect �G�t�F�N�g����*/
	hr = g_apSourceVoice[label]->DisableEffect(0);

	return S_OK;
}
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
HRESULT CSound::EnableSoundEffect(SOUND_LABEL label)
{
	HRESULT hr;
	/*EnableEffect  �G�t�F�N�g�L��*/
	hr = g_apSourceVoice[label]->EnableEffect(0);

	return S_OK;
}
//================================================================================
//
// SoundVolume
//
// ������ : SoundLabel
// ������ : Volume
// ��Volume �́@-XAUDIO2_MAX_VOLUME_LEVEL  ~ XAUDIO2_MAX_VOLUME_LEVEL  -2^24 ~ 2^24�܂�
// ����`�͏�L�̐ݒ萔�l�ł��邪���ۉ���������݂���̂Ŏ����Œ������Č��E��m��ق����ǂ��B
//================================================================================
HRESULT CSound::SoundVolume(SOUND_LABEL label, float Volume)
{
	//Volume�̑傫��
	//-XAUDIO2_MAX_VOLUME_LEVEL  ~ XAUDIO2_MAX_VOLUME_LEVEL  -2^24 ~ 2^24�܂�
	//�����ꂷ��̂ŁI�m�F���Ȃ���Z�b�g����
	g_apSourceVoice[label]->SetVolume(Volume);

	return S_OK;
}
//================================================================================
//
// DirectionSound
// 
// ������ : SoundLabel
// ������ : LeftVolume
// ��O���� : RightVolume
//  
// �������ŉ����o�������ꍇ�@LeftVolume = 1.f RightVolume = 0.f
// ���E���ŉ����o�������ꍇ�@LeftVolume = 0.f RightVolume = 0.f
//================================================================================
HRESULT CSound::DirectionSound(SOUND_LABEL label, float LeftVolume, float RightVolume)
{
	/*Left,RightSoundSet*/
	float outputMatrix[2] = { LeftVolume,RightVolume };
	/* Setting */
	g_apSourceVoice[label]->SetChannelVolumes(3, outputMatrix);
	return S_OK;
}

//================================================================================
//
// [ �Z�O�����g�Đ�or��~�֐� ]
//
// � ���� �
//
// �label : �w�b�_�Œ�`���ꂽ�T�E���h�t�@�C���̔ԍ�
//
// � �߂�l �
//
// �HRESULT�^ : �G���[�`�F�b�N�p
//
//�y �ڍ� �z
//
// ��Z�O�����g�̍Đ�������
// ��Đ����̏ꍇ�͒�~����
//
//================================================================================

HRESULT CSound::PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aParam[label].nCntLoop;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	g_apSourceVoice[label]->Start(0);

	return S_OK;
}



//================================================================================
//
// [ �Z�O�����g��~�֐�( �ʎw�� ) ]
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
// ��Z�O�����g���ʂɒ�~����
//
//================================================================================

void CSound::StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}



//================================================================================
//
// [ �Z�O�����g��~�֐�( �S�� ) ]
//
// � ���� �
//
// ��Ȃ�
//
// � �߂�l �
//
// ��Ȃ�
//
//�y �ڍ� �z
//
// ��S�ẴZ�O�����g���~����
//
//================================================================================

void CSound::StopSound(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}



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

HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}



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

HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}



//=============================================================================
//
// サウンド処理 [sound.cpp]
// 
//
//=============================================================================


//**********************************************************************
//
// インクルード文
//
//**********************************************************************
#include "Sound.h"


//================================================================================
//
// [ サウンド初期化関数 ]
//
// ｢ 引数 ｣
//
// ･hWnd : ウィンドウハンドル
//
// ｢ 戻り値 ｣
//
// ･HRESULT型 : エラーチェック用
//
//【 詳細 】
//
// ･サウンドを使用するための初期化
//
//================================================================================

HRESULT CSound::InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (g_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aParam[nCntSound].nCntLoop;

		// オーディオバッファの登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
		hr = XAudio2CreateVolumeMeter(&g_pXAPO);
		//エフェクト作成
		hr = XAudio2CreateReverb(&g_pXAPO);


		g_descriptor.InitialState = true;
		/*outputの場所を示している為、音響がおかしかったらまずここを疑う*/
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
// [ サウンド終了処理関数 ]
//
// ｢ 引数 ｣
//
// ･なし
//
// ｢ 戻り値 ｣
//
// ･なし
//
//【 詳細 】
//
// ･サウンドの使用を終了するときの処理
//
//================================================================================

void CSound::UninitSound(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;

			// オーディオデータの開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;

	if (g_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}
//================================================================================
//
// InitEffectSound
//
// エフェクト初期化関数
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
// 第一引数 : SoundLabel
// 第二引数 : OutputNumber
// 
// ※動的にとれると言っているが、update関数内で読み込ませると毎回音の情報に書き加えるためバグる。
// なので、推奨はInit関数内で初期化した後のタイミングにやるのが良い。
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
// [ エフェクト無効関数( 個別指定 ) ]
//
// ｢ 引数 ｣
//
// ･label : ヘッダで定義されたサウンドファイルの番号
//
// ｢ 戻り値 ｣
//
// ･なし
//
//【 詳細 】
//
// ･エフェクトを個別に無効にさせる。
//
//================================================================================
HRESULT CSound::DisableSoundEffect(SOUND_LABEL label)
{
	HRESULT hr;
	/*DisableEffect エフェクト無効*/
	hr = g_apSourceVoice[label]->DisableEffect(0);

	return S_OK;
}
//================================================================================
//
// [ エフェクト有効関数( 個別指定 ) ]
//
// ｢ 引数 ｣
//
// ･label : ヘッダで定義されたサウンドファイルの番号
//
// ｢ 戻り値 ｣
//
// ･なし
//
//【 詳細 】
//
// ･エフェクトを個別に有効にさせる。
//
//================================================================================
HRESULT CSound::EnableSoundEffect(SOUND_LABEL label)
{
	HRESULT hr;
	/*EnableEffect  エフェクト有効*/
	hr = g_apSourceVoice[label]->EnableEffect(0);

	return S_OK;
}
//================================================================================
//
// SoundVolume
//
// 第一引数 : SoundLabel
// 第二引数 : Volume
// ※Volume は　-XAUDIO2_MAX_VOLUME_LEVEL  ~ XAUDIO2_MAX_VOLUME_LEVEL  -2^24 ~ 2^24まで
// ※定義は上記の設定数値であるが実際音割れも存在するので自分で調整して限界を知るほうが良い。
//================================================================================
HRESULT CSound::SoundVolume(SOUND_LABEL label, float Volume)
{
	//Volumeの大きさ
	//-XAUDIO2_MAX_VOLUME_LEVEL  ~ XAUDIO2_MAX_VOLUME_LEVEL  -2^24 ~ 2^24まで
	//音割れするので！確認しながらセットする
	g_apSourceVoice[label]->SetVolume(Volume);

	return S_OK;
}
//================================================================================
//
// DirectionSound
// 
// 第一引数 : SoundLabel
// 第二引数 : LeftVolume
// 第三引数 : RightVolume
//  
// ※左側で音を出したい場合　LeftVolume = 1.f RightVolume = 0.f
// ※右側で音を出したい場合　LeftVolume = 0.f RightVolume = 0.f
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
// [ セグメント再生or停止関数 ]
//
// ｢ 引数 ｣
//
// ･label : ヘッダで定義されたサウンドファイルの番号
//
// ｢ 戻り値 ｣
//
// ･HRESULT型 : エラーチェック用
//
//【 詳細 】
//
// ･セグメントの再生をする
// ･再生中の場合は停止する
//
//================================================================================

HRESULT CSound::PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aParam[label].nCntLoop;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoice[label]->Start(0);

	return S_OK;
}



//================================================================================
//
// [ セグメント停止関数( 個別指定 ) ]
//
// ｢ 引数 ｣
//
// ･label : ヘッダで定義されたサウンドファイルの番号
//
// ｢ 戻り値 ｣
//
// ･なし
//
//【 詳細 】
//
// ･セグメントを個別に停止する
//
//================================================================================

void CSound::StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}



//================================================================================
//
// [ セグメント停止関数( 全て ) ]
//
// ｢ 引数 ｣
//
// ･なし
//
// ｢ 戻り値 ｣
//
// ･なし
//
//【 詳細 】
//
// ･全てのセグメントを停止する
//
//================================================================================

void CSound::StopSound(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}



//================================================================================
//
// [ チャンクのチェック ]
//
// ｢ 引数 ｣
//
// ･hFile              : ファイルハンドル
// ･format             : ファイルフォーマット
// ･pChunkSize         : チャンクサイズ
// ･pChunkDataPosition : チャンクデータの場所
//
// ｢ 戻り値 ｣
//
// ･HRESULT型 : エラーチェック用
//
//【 詳細 】
//
// ･チャンクデータをチェックする
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
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
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
// [ チャンクの読み込み ]
//
// ｢ 引数 ｣
//
// ･hFile          : ファイルハンドル
// ･pBuffer        : 読み込み先バッファ
// ･dwBuffersize   : 読み込み先バッファのサイズ
// ･dwBufferoffset : 読み込み先バッファのオフセット
//
// ｢ 戻り値 ｣
//
// ･HRESULT型 : エラーチェック用
//
//【 詳細 】
//
// ･チャンクデータを読み込む
//
//================================================================================

HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}



//=============================================================================
//
// サウンド処理 [sound.h]
// 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_
#include < XAudio2.h >
#include <XAudio2fx.h>
#include <XAPO.h>
//*****************************************************************************
// サウンドファイル
//*****************************************************************************
// サウンドファイル
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
// 構造体宣言
//
//**********************************************************************

// パラメータ
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
} SOUNDPARAM;

struct MyEffectParameter
{
	//ReflectionDelay:ダイレクトパスに対する初期反射の遅延時間　指定範囲可能範囲　0 ~ 300ミリ秒
	int ReflectionsDelay;
	//ReverbDelay:初期反射に対するリバーブの遅延、指定範囲は0～85ミリ秒
	int ReverbDelay;
	//RearDelay:左後方出力及び右後方出力の遅延　指定可能範囲 0～85ミリ秒
	int RearDelay;
	//PositionLeft:シミュレーション空間における視聴者に対する左入力の位置。　指定可能範囲は 0 ~ 30　（単位なし）
	int PositionLeft;
	//PositionRight: PositionLeftと同様右入力にのみ影響します。　指定可能範囲は 0 ~ 30(単位なし）
	int PositionRight;
	//PositionMatrixRight&Left:音源から視聴者まで距離によるインプレッションを増減させる。　指定可能範囲は 0 ~ 30（単位なし）
	int PositionMatrixLeft;
	int PositionMatrixRight;
	//EarlyDiffusion:個々の壁の反射特性を制御します。堅く平らな表面シミュレートするには小さな値を設定し、
	//               散乱性の表面をシミュレートするには大きな値を設定。　指定可能範囲 0 ~15(単位なし）
	int EarlyDiffusion;
	//LateDiffusion:個々の壁のリバーブ特性を制御。指定可能範囲 0 ~ 15(単位なし）
	int LateDiffusion;
	//LowEQGain: 1kHzにおける減衰時間を基準にして低周波数の減衰時間を調整. 指定可能範囲 0 ~ 12(単位なし)
	int LowEQGain;
	//LowEQCutoff:パラメーターにより制御されるローパスフィルターの折点周波数を設定。指定漢方範囲 0 ~ 9 (単位なし)
	int LowEQCutoff;
	//HighEQGain:減衰時間を基準にして高周波数の減衰時間を調整。 0 の場合　高周波数の音が1khzの場合と同じ割合で減衰します。　指定可能範囲 0 ~ 8(単位なし)
	int HighEQGain;
	//HighEQCutoff: HighEQGainパラメーターにより制御されるハイパスフィルターの接点周波数を設定。　指定可能範囲 0 ~ 14(単位なし）
	int HighEQCutoff;
	//RoomFilterFreq:室内エフェクトのローパスフィルターの接点周波数を設定。　指定可能範囲は 20 ~ 20,000Hz
	float RoomFilterFreq;
	//RoomFilterMain:初期反射と後期フィールド残響の両方に適用されるローパスフィルターのパス バンド強度レベルを設定。　指定可能範囲 -100 ~ 0dB
	float RoomFilterMain;
	//RoomfiletHF:接点周波数での初期反射と後期フィールド残響の両方に適用されるローパスフィルターのパスバンド強度レベルを設定。指定可能範囲 -100 ~ 0dB
	float RoomFilterHF;
	//ReflectionGain:初期反射の強度/レベルを調整します。指定可能範囲は-100~20dB。
	float ReflectionsGain;
	//ReverbGain:リバーブの強度/レベルを調整します。指定可能範囲は -100 ~ 20dB
	float ReverbGain;
	//DecayTime:1kHzにおける残響減衰時間です。これは、フルスケールの入力信号が60dB減衰するまでの時間です。指定可能範囲は0.1 ~ 無限（秒）です。
	float DecayTime;
	//後期フィールド残響のモード密度を制御します。無色 (colorless) の空間では、Density を最大値 (100) に設定する必要があります。Density を小さくすると、
	//サウンドはくぐもった音 (くし形フィルターが適用された音) になります。サイロをシミュレーションするときに有効なエフェクトです。指定可能範囲は 0 ～ 100 で、単位はパーセントです。
	float Density;
	//RoomSize:音響空間の見かけ上のサイズです。　指定可能範囲は 1(30.48cm) ~ 100フィート(30.48m)
	float RoomSize;
	//WetDryMix:元の変更されてないサウンドとなる出力の割合　許容値　0 ~ 100;
	float WetDryMix;
};

//*****************************************************************************
// プロトタイプ宣言
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

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);



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

	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

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
	HRESULT DisableSoundEffect(SOUND_LABEL label);

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
	HRESULT EnableSoundEffect(SOUND_LABEL label);
private:
	IXAudio2 *g_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *g_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD g_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
	IUnknown * g_pXAPO;											//Effect設定用XAPO
	XAUDIO2_EFFECT_DESCRIPTOR g_descriptor;						//Effect設定
	XAUDIO2_EFFECT_CHAIN g_chain;								//Effect設定チェイン
																// 各音素材のパラメータ
																// 各音素材のパラメータ


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
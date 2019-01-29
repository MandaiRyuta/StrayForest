#include "gamemanager.h"
#include "renderer.h"
#include <time.h>
#include "colision.h"
#include "Number.h"
#include "CModeGame.h"
#include "CModeTitle.h"
#include "CModeChutorial.h"
#include "CModeResult.h"
#include "CModeGameOver.h"

//YPRCamera *CManager::Gamera;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;		// キーボードへのポインタ
CInputMouse *CManager::m_pInputMouse = NULL;			// マウスへのポインタ
CInputJoypad *CManager::m_pInputJoypad = NULL;			// ジョイパッドへのポインタ
CMode *CManager::m_Mode = NULL;
CSound *CManager::sound_ = NULL;
CManager::CManager(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	// キーボード処理初期化
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	// マウス処理初期化
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);

	// ジョイパッド処理初期化
	m_pInputJoypad = new CInputJoypad;
	m_pInputJoypad->Init(hInstance, hWnd);

	if (FAILED(CRendere::Init(hWnd, bWindow, nWindowWidth, nWindowHeight))) 
	{
		PostQuitMessage(0);
	}
	sound_ = new CSound;
	sound_->InitSound(hWnd);
	sound_->InitEffectSound();
}

void CManager::Init()
{
	//SetMode(new CModeGameOver);
	//SetMode(new CModeResult);
	SetMode(new CModeTitle);
	//SetMode(new CModeGame);
	//SetMode(new CModeChutorial);
}

void CManager::Uninit()
{
	if (m_Mode != NULL)
	{
		m_Mode->Uninit();
		delete m_Mode;
		m_Mode = NULL;
	}

	// キーボードの開放処理
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// マウスの開放処理
	if (m_pInputMouse)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// ジョイパッドの開放処理
	if (m_pInputJoypad)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}
}

void CManager::Update()
{
	/*
		シェーダを使うためにg_SceneInfo[2]->Set関数を呼び出しているが、
		この状態では行数が増える一方である、もっと簡略化させるためには
		シェーダークラスを作成しその中でWorldとViewとProjを掛け合わせた
		物を求めてHLSL側に命令させるような方式のほうが行数は減る。
	*/
	// キーボード更新処理
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Update();
	}

	// マウス更新処理
	if (m_pInputMouse)
	{
		m_pInputMouse->Update();
	}

	// ジョイパッド更新処理
	if (m_pInputJoypad)
	{
		m_pInputJoypad->Update();
	}

	CScene::UpdateAll();
	if (m_Mode != NULL)
		m_Mode->Update(); //ポリモフィズム
}

void CManager::Draw()
{
	CRendere::DrawBegin();
	CScene::DrawAll();
	if (m_Mode != NULL)
		m_Mode->Draw(); //ポリモフィズム
	CRendere::DrawEnd();
}

void CManager::SetMode(CMode * Mode)
{
	if (m_Mode != NULL) {
		m_Mode->Uninit();
		delete m_Mode;
		m_Mode = NULL;
	}

	m_Mode = Mode;

	if (m_Mode != NULL) {
		m_Mode->Init();
	}
}




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
CInputKeyboard *CManager::m_pInputKeyboard = NULL;		// �L�[�{�[�h�ւ̃|�C���^
CInputMouse *CManager::m_pInputMouse = NULL;			// �}�E�X�ւ̃|�C���^
CInputJoypad *CManager::m_pInputJoypad = NULL;			// �W���C�p�b�h�ւ̃|�C���^
CMode *CManager::m_Mode = NULL;
CSound *CManager::sound_ = NULL;
CManager::CManager(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	// �L�[�{�[�h����������
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	// �}�E�X����������
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);

	// �W���C�p�b�h����������
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

	// �L�[�{�[�h�̊J������
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �}�E�X�̊J������
	if (m_pInputMouse)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// �W���C�p�b�h�̊J������
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
		�V�F�[�_���g�����߂�g_SceneInfo[2]->Set�֐����Ăяo���Ă��邪�A
		���̏�Ԃł͍s�������������ł���A�����Ɗȗ��������邽�߂ɂ�
		�V�F�[�_�[�N���X���쐬�����̒���World��View��Proj���|�����킹��
		�������߂�HLSL���ɖ��߂�����悤�ȕ����̂ق����s���͌���B
	*/
	// �L�[�{�[�h�X�V����
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Update();
	}

	// �}�E�X�X�V����
	if (m_pInputMouse)
	{
		m_pInputMouse->Update();
	}

	// �W���C�p�b�h�X�V����
	if (m_pInputJoypad)
	{
		m_pInputJoypad->Update();
	}

	CScene::UpdateAll();
	if (m_Mode != NULL)
		m_Mode->Update(); //�|�����t�B�Y��
}

void CManager::Draw()
{
	CRendere::DrawBegin();
	CScene::DrawAll();
	if (m_Mode != NULL)
		m_Mode->Draw(); //�|�����t�B�Y��
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




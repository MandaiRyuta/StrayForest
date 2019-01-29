#include "CModeChutorial.h"
#include "gamemanager.h"
#include "CModeGame.h"
CScene2D *CModeChutorial::background_;
CScene2D *CModeChutorial::chutorialfade_;
void CModeChutorial::Init()
{
	POLYGONSIZE polygonsize = {};
	polygonsize.dx_ = 0.0f;
	polygonsize.dy_ = 0.0f;
	polygonsize.dw_ = windows_rect::SCREEN_WIDTH + 24;
	polygonsize.dh_ = windows_rect::SCREEN_HEIGHT + 54;
	polygonsize.tcx_ = 0;
	polygonsize.tcy_ = 0;
	polygonsize.tcw_ = 1280;
	polygonsize.tch_ = 1024;
	polygonsize.affine_ = true;
	polygonsize.color_ = D3DCOLOR_RGBA(155, 155, 155, 255);
	background_ = CScene2D::Create(true, polygonsize, TEXTURERS::BACKGROUND, 1280, 1024, false);
	polygonsize.color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
	CScene2D::Create(true, polygonsize, TEXTURERS::CHUTORIAL, 1280, 1024, false);
	polygonsize.dx_ = 0.0f;
	polygonsize.dy_ = 0.0f;
	polygonsize.dw_ = windows_rect::SCREEN_WIDTH + 24;
	polygonsize.dh_ = windows_rect::SCREEN_HEIGHT + 54;
	polygonsize.tcx_ = 0;
	polygonsize.tcy_ = 0;
	polygonsize.tcw_ = 1280;
	polygonsize.tch_ = 1024;
	polygonsize.affine_ = true;
	polygonsize.color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
	chutorialfade_ = CScene2D::Create(true, polygonsize, TEXTURERS::Fade, 1280, 1024, false);
	fadeoutcount_ = 255;
	fadeincount_ = 0;
	fademode_ = false;
	alpha = 0;
}

void CModeChutorial::Uninit()
{
	CScene::ReleaseAll();
}

void CModeChutorial::Update()
{
	if (fademode_ == false)
	{
		if (fadeoutcount_ > 0)
		{
			fadeoutcount_ -= 3;
			chutorialfade_->ColorSet(D3DCOLOR_RGBA(255, 255, 255, fadeoutcount_));
		}
	}
	if (fademode_ == true)
	{
		if (fadeincount_ < 255)
		{
			fadeincount_+= 3;
			chutorialfade_->ColorSet(D3DCOLOR_RGBA(255, 255, 255, fadeincount_));
		}
		else
		{
			CManager::SetMode(new CModeGame());
		}
	}
	if (alpha > 30)
	{
		alpha = 0;
	}
	background_->ColorSet(D3DCOLOR_RGBA(125 + alpha, 125 + alpha, 125 + alpha, 255));
	CInputKeyboard* pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))
	{
		CManager::GetSound()->PlaySound(BUTTON);
		fademode_ = true;
	}

	chutorialcount_++;
	if (chutorialcount_ >= 10)
	{
		alpha += 1;
		chutorialcount_ = 0;
	}
}

void CModeChutorial::Draw()
{
}

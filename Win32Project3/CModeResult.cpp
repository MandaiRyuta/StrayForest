#include "CModeResult.h"
#include "gamemanager.h"
#include "CModeTitle.h"

CScene2D *CModeResult::gameclear_;
CScene2D *CModeResult::gameclearfade_;
void CModeResult::Init()
{
	CManager::GetSound()->StopSound();
	CManager::GetSound()->PlaySound(RESULT_BGM);
	POLYGONSIZE polygonsize;
	polygonsize = {};
	polygonsize.dx_ = 0.0f;
	polygonsize.dy_ = 0.0f;
	polygonsize.dw_ = windows_rect::SCREEN_WIDTH + 24;
	polygonsize.dh_ = windows_rect::SCREEN_HEIGHT + 54;
	polygonsize.tcx_ = 0;
	polygonsize.tcy_ = 0;
	polygonsize.tcw_ = 1280;
	polygonsize.tch_ = 1024;
	polygonsize.color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
	polygonsize.affine_ = true;
	CScene2D::Create(true,polygonsize, TEXTURERS::RESULTBACKGROUND,1280,1024,false);
	polygonsize.dx_ = 100.0f;
	polygonsize.dy_ = 75.0f;
	polygonsize.dw_ = 400 + 24;
	polygonsize.dh_ = 300 + 54;
	polygonsize.tch_ = 400;
	polygonsize.tcx_ = 0;
	polygonsize.tcy_ = 0;
	polygonsize.tcw_ = 1280;
	gameclear_ = CScene2D::Create(true, polygonsize, TEXTURERS::GAMECLEAR, 1280, 400, false);
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
	gameclearfade_ = CScene2D::Create(true, polygonsize, TEXTURERS::Fade, 1280, 1024, false);
	fadeoutcount_ = 255;
	fadeincount_ = 0;
	fademode_ = false;
	this->blue = 5;
	this->red = 10;
	this->green = 15;
}

void CModeResult::Uninit()
{
	CScene::ReleaseAll();
}

void CModeResult::Update()
{
	if (red >= 255)
	{
		red = 0;
	}
	if (green >= 255)
	{
		green = 0;
	}
	if (blue >= 255)
	{
		blue = 0;
	}
	if (fademode_ == false)
	{
		if (fadeoutcount_ > 0)
		{
			fadeoutcount_ -= 3;
			gameclearfade_->ColorSet(D3DCOLOR_RGBA(255, 255, 255, fadeoutcount_));
		}
	}
	if (fademode_ == true)
	{
		if (fadeincount_ < 255)
		{
			fadeincount_ += 3;
			gameclearfade_->ColorSet(D3DCOLOR_RGBA(255, 255, 255, fadeincount_));
		}
		else
		{
			CManager::SetMode(new CModeTitle());
		}
	}
	CInputKeyboard* pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();
	if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))
	{
		CManager::GetSound()->PlaySound(BUTTON);
		fademode_ = true;
	}

	gameclear_->ColorSet(D3DCOLOR_RGBA(red, green, blue, 255));

	red += 10;
	green += 15;
	blue += 20;
}

void CModeResult::Draw()
{
}

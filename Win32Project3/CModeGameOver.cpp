#include "CModeGameOver.h"
#include "gamemanager.h"
#include "CModeTitle.h"

CScene2D *CModeGameOver::gameover_;
CScene2D *CModeGameOver::gameoverfade_;
void CModeGameOver::Init()
{
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
	CScene2D::Create(true, polygonsize, TEXTURERS::RESULTBACKGROUND, 1280, 1024, false);
	polygonsize.dx_ = 100.0f;
	polygonsize.dy_ = 75.0f;
	polygonsize.dw_ = 400 + 24;
	polygonsize.dh_ = 300 + 54;
	polygonsize.tch_ = 400;
	gameover_ = CScene2D::Create(true, polygonsize, TEXTURERS::GAMEOVER, 1280, 400, false);
	this->alpha = 0;
	this->blue = 5;
	this->red = 10;
	this->green = 15;
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
	gameoverfade_ = CScene2D::Create(true, polygonsize, TEXTURERS::Fade, 1280, 1024, false);
	fadeoutcount_ = 255;
	fadeincount_ = 0;
	fademode_ = false;
	fadeincount_ = 0;
	CManager::GetSound()->PlaySound(GAMEOVER_BGM);
}

void CModeGameOver::Uninit()
{
	CScene::ReleaseAll();
}

void CModeGameOver::Update()
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
			gameoverfade_->ColorSet(D3DCOLOR_RGBA(255, 255, 255, fadeoutcount_));
		}
	}
	if (fademode_ == true)
	{
		if (fadeincount_ < 255)
		{
			fadeincount_ += 3;
			gameoverfade_->ColorSet(D3DCOLOR_RGBA(255, 255, 255, fadeincount_));
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

	gameover_->ColorSet(D3DCOLOR_RGBA(red, green, blue, alpha));

	if (alpha < 255)
	{
		alpha++;
	}
	red += 1;
	green += 2;
	blue += 3;
}

void CModeGameOver::Draw()
{
}

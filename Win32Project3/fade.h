#pragma once
#include "cscene.h"
#include "main.h"
struct FADE_VERTEX
{
	float x, y, z;
	DWORD color;
};

class Fade : public CScene
{
public:
	Fade() : CScene(1){}
	~Fade() {}
public:
	void Init() override;
	void Uninit() override;
	void Draw() override;
	void Update() override;

	void BlackFadeIn();
	void BlackFadeOut();
	void WhiteFadeIn();
	void WhiteFadeOut();
	
	void FadeSetNumber(int fade);
public:
	static Fade* Create();
private:
	int FadeIn;
	int FadeOut;
	int FadeSet_;
};
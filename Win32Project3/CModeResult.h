#pragma once
#include "SceneMode.h"
#include "scene2d.h"
#include "fade.h"

class CModeResult : public CMode
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	static CScene2D* gameclearfade_;
	static CScene2D* gameclear_;
	int fadeincount_;
	int fadeoutcount_;
	bool fademode_;
	int alpha;
	int red;
	int blue;
	int green;
};
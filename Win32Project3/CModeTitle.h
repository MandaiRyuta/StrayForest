#pragma once
#include "SceneMode.h"
#include "scene2d.h"

class CModeTitle : public CMode
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	int fadeincount_;
	int fadeoutcount_;
	bool fademode_;
	static CScene2D* titlefade_;
	static CScene2D* presskey_;
	int alpha;
};
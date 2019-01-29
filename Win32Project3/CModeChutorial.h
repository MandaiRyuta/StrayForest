#pragma once
#include "SceneMode.h"
#include "scene2d.h"

class CModeChutorial : public CMode
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	static CScene2D* background_;
	static CScene2D* chutorialfade_;
	int fadeincount_;
	int fadeoutcount_;
	bool fademode_;
	int alpha;
	int chutorialcount_;
};
#pragma once
#include "SceneMode.h"
#include "scene2d.h"

class CModeGameOver : public CMode
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	static CScene2D* gameoverfade_;
	static CScene2D* gameover_;
	int fadeincount_;
	int fadeoutcount_;
	bool fademode_;
	int alpha;
	int red;
	int blue;
	int green;
};
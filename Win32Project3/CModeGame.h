#pragma once
#include "cscene.h"
#include "SceneMode.h"
#include "scene2d.h"
#include "scene3d.h"
#include "cscenemodel.h"
#include "billboard.h"
#include "meshfiled.h"
#include "player.h"
#include "skydome.h"
#include "camera.h"
#include "light.h"
#include "Number.h"
#include "enemy.h"
#include "modelInstancing.h"
#include "fade.h"
#include "CBox.h"
#include "fireplace.h"
#include "playerhelth.h"
#include "sign.h"

class CModeGame : public CMode
{
private:
	static Fade* SetFade;
	static CCamera*   g_camera;
	static CLight*   g_light;
	static MeshFiled* filed;
	static CPlayer* player;
	//static Counter* count;
	static CEnemy* enemy[80];
	//static CModel* sword;
	static CModelInstancing* instancemodel;
	static CScene2D* Stamina[3];
	static CScene2D* Pause;
	static CBox* box;
	static CPlayerHelth* player_helth[10];
	static CFirePlace* fireplace_;
	//int GameTimer; //Œã‚Å’¼‚·
	int fadeCount;
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static Fade* GetFadeMode(void) { return SetFade; }
	//static CModel *GetSword(void) { return sword; };
	static MeshFiled *GetFiled(void) { return filed; }
	static CEnemy* GetEnemy(int num) { return enemy[num]; }
	static CPlayer *GetPlayer(void) { return player; }
	static CModelInstancing *GetInstanceModel(void) { return instancemodel; }
	static CPlayerHelth* GetPlayerHelth(int i) { return player_helth[i]; }
	static CScene2D* GetStaminaInfo(void) { return Stamina[2]; }
	static CBox* GetBox(void) { return box; }
	static CFirePlace* GetFirePlace(void) { return fireplace_; }
};
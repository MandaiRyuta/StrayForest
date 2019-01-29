#pragma once
#include "main.h"
#include <map>

struct POLYGONSIZE
{
	float        dx_;
	float        dy_;
	float        dw_;
	float        dh_;
	int          tcx_;
	int          tcy_;
	unsigned int tcw_;
	unsigned int tch_;
	D3DCOLOR	 color_;
	bool         affine_;
};
struct Area {
	D3DXVECTOR3 max;
	D3DXVECTOR3 min;
};
enum SCENE_TYPE {
	SCENE_TYPE_NONE,
	SCENE_TYPE_2D,
	SCENE_TYPE_3D,
	SCENE_TYPE_PLAYER,
	SCENE_TYPE_ENEMY,
	SCENE_TYPE_BULLET,
};
struct SphereInfo
{
	D3DXVECTOR3 modelpos;
	float r;
};
struct AABB {
	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
};
//�ꎞ�I�Ɂ@�{���̓N���X��
struct VERTEX_2D
{
	D3DXVECTOR4 pos;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
};
class CScene
{
protected:
	struct VERTEX3D
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DCOLOR   color;
		D3DXVECTOR2 texcoord;
	};

public:
	enum SceneSortNumber 
	{
		SCENE_3D_XMODEL,
		SCENE_3D_POLYGON,
		SCENE_2D_POLYGON,
	};
public:
	CScene(int Priority/*, SCENE_TYPE Scene_Type*/);
	virtual ~CScene(){}
	virtual void Init() {};
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw()   = 0;
	SCENE_TYPE GetType();

	void Release();
	virtual void SetMtxView(D3DXMATRIX view) {}
	virtual void SetMtxProj(D3DXMATRIX proj) {}
	virtual void SetMtxModel(D3DXMATRIX model) {}
	virtual void SetMtxRot(D3DXMATRIX Rot) {}
	virtual void PlayerDmg(int dmg) {}
	virtual void EnemyDmg(int dmg) {}
	virtual void Playerheal(int heal) {}
	virtual void helthstate(int i) {}
	static bool GameClear() { return false; };
	static void UpdateAll();
	static void DrawAll();
	static void ReleaseAll();
	static void SetMtxViewAll(D3DXMATRIX view);
	static void SetMtxProjAll(D3DXMATRIX proj);
	static void Pause();
	static int PauseStateMode();
	POLYGONSIZE SetPolygonsize();
private:
	static CScene* m_Scene[3][200];
	static int PauseState;
//	SCENE_TYPE m_Type;
};
#pragma once
#include "main.h"
#include "cscene.h"
#include "colision.h"
#include "cskinmeshanim.h"
enum ENEMYACTIONMODE
{
	MOVE,
	ATTACK,
	DAMAGE,
	DOWN,
};
enum ENEMYMOVETYPE
{
	SIDEMOVE,
	LENGTHMOVE,
	CIRCLEMOVE,
	INVCIRCLEMOVE,
	SPHERE,
};
class CEnemy : public CScene {
public:
	CEnemy(int Priority,int MovePattern,Area MoveArea ,MATRIX3DMANAGER::Model* model, std::string filename) : MovePattern_(MovePattern),enemy_model_(model), MoveArea_(MoveArea), filename_(filename), CScene(Priority/*, SCENE_TYPE_ENEMY*/)
	{
	
		front_ = { 0.0f,0.0f,1.0f };
		right_ = { 1.0f,0.0f,0.0f };
	}
	~CEnemy() {}
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;
	static CEnemy* Create(int MovePattern,MATRIX3DMANAGER::Model* model, Area MoveArea, std::string filename);
	SphereInfo Set();
	D3DXVECTOR3 SetPosition();
	void EnemyDmg(int dmg) override;
private:
	bool rotflag;
	int MovePattern_;
	int ActionPattern_;
	//ìGÇÃìÆÇ´ÇÉpÉ^Å[ÉìâªÇ≥ÇπÇÈÅBá@ëOå„Å@áAç∂âEÅ@áBãÖÇÃîÕàÕì‡Ç≈úpúj
	//D3DXVECTOR3 firstposition_;
	Area MoveArea_;
	int	Helth_;
	float movespeed_;
	SphereInfo enemycolision_;
	MATRIX3DMANAGER::Model* enemy_model_;
	D3DXVECTOR3 front_;
	D3DXVECTOR3 right_;
	D3DXVECTOR3 firstposition_;
	static D3DXMATRIX view_;
	static D3DXMATRIX proj_;
	D3DXMATRIX mtx_rotation_;
	D3DXMATRIX mtx_scale_;
	D3DXMATRIX mtx_translation_;
	D3DXMATRIX mtx_world_;
	float rotation_;
	std::string filename_;
	CSkinMesh* EnemySkinMesh_;
	int nTimeCount;
	float rotatecount_;
	float invrotatecount_;
};
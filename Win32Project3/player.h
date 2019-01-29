#pragma once
#include "main.h"
#include "cscene.h"
#include "scene2d.h"
#include "colision.h"
#include "cskinmeshanim.h"
#include "playerhelth.h"
#include "PlayerItem.h"
/*プレイヤー＆インプット操作可能にさせる*/
/*敵＆弾の生成*/

class CPlayer : public CScene {
public:
	CPlayer(int Priority, MATRIX3DMANAGER::Model* model, std::string filename)
		: player_model_(model), filename_(filename), CScene(SCENE_3D_XMODEL) {
		this->front_ = { 0.0f,0.0f,1.0f };
		this->right_ = { 1.0f,0.0f,0.0f };
		this->up_ = { 0.0f,1.0f,0.0f };
	}
	~CPlayer() {}
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;
	void SetMtxView(D3DXMATRIX view) override;
	void SetMtxProj(D3DXMATRIX proj) override;
	float SetRotation();
	bool IsPlayer();

	static CPlayer* Create(MATRIX3DMANAGER::Model* model, std::string filename);
	D3DXVECTOR3 PlayertoCamera(int type);
	D3DXMATRIX PlayerMtxtoCameraMtx(int type);
	float PlayerRotation();
	SphereInfo Set() { return modelinfo_; }
	static D3DXVECTOR3 GetPosition();
	void PlayerDmg(int dmg) override;
	void Playerheal(int heal) override;
	bool GameClear();
private:
	void PlayerControl();
	void PlayerHelth(int helth);
	void PlayerAnimation();
private:
	enum PLAYERMOTION
	{
		WALK,
		RUN,
		ATTACK,
		SKILL,
		DIFFENCE,
		DIFFENCIVE,
		DAMAGE,
		DOWN,
		IDLE,
	};
private:
	int healcooltime_;
	bool gameclear_;
	int AttackCount_;
	bool IsPlayer_;
	int Helth_;
	float Stamina_;
	bool Running_;
	int nTimeCount_;
	int nAttackCount_;
	int ActionPattern_;
	bool MoveState;
	D3DXVECTOR3 hit_;
	std::string filename_;
	MATRIX3DMANAGER::Model* player_model_;
	D3DXVECTOR3 front_;
	D3DXVECTOR3 right_;
	D3DXVECTOR3 up_;
	D3DXVECTOR3 MoveSpeed;
	D3DXVECTOR3 oldposition_;
	//当たり判定用
	static D3DXVECTOR3 SetPosition;
	static bool onHit_;
	SphereInfo modelinfo_;
	SphereInfo modelAttackinfo_;
	static D3DXVECTOR3 pos_;
	float rotation_;
	D3DXMATRIX mtx_world_;
	D3DXMATRIX mtx_view_;
	D3DXMATRIX mtx_proj_;
	D3DXMATRIX mtx_translation;
	D3DXMATRIX mtx_rotation_;
	D3DXMATRIX mtx_scale_;
	D3DXVECTOR3 position_;
	LPDIRECT3DVERTEXBUFFER9 vertexbuffer_;
	LPDIRECT3DINDEXBUFFER9 indexbuffer_;
	LPDIRECT3DTEXTURE9 texture_;
	CSkinMesh* SkinMeshPlayerModel;
	ItemList item_;
	CPlayerHelth* helthmanager[10];
};
#include "player.h"
#include "gamemanager.h"
#include "input.h"
#include "meshfiled.h"
#include "CModeGame.h"
#include "CModeResult.h"
#include "fireplace.h"

//#include "CBox.h"
/*
	手のボーンの配置をもう少し小さくしてみる＆肌の中にいれる
	アニメーションを一回にする
*/
D3DXVECTOR3 CPlayer::SetPosition;

void CPlayer::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	SkinMeshPlayerModel = new CSkinMesh();
	SkinMeshPlayerModel->Init(pDevice, "resource/xmodel/Player.x");
	
	D3DXMatrixIdentity(&this->mtx_rotation_);
	this->position_ = player_model_->position_;
	this->oldposition_ = player_model_->position_;
	rotation_ = 3.22f;
	nAttackCount_ = 0;
	modelinfo_.r = 1.0f;
	modelAttackinfo_.r = 3.0f;
	D3DXMatrixIdentity(&mtx_rotation_);
	ActionPattern_ = 0;
	nTimeCount_ = 0;
	ActionPattern_ = IDLE;
	SkinMeshPlayerModel->MyChangeAnim(0.15);
	SkinMeshPlayerModel->SetAnimSpeed(0.01f);
	Helth_ = 40;
	Stamina_ = 345.0f;
	Running_ = true;
	IsPlayer_ = false;
	AttackCount_ = 0;
	gameclear_ = false;
	healcooltime_ = 0;
	MoveState = true;

	item_.add(new Sword, new Shild);
	item_.Init(pDevice);
	for (int i = 0; i < 10; i++)
	{
		helthmanager[i] = CModeGame::GetPlayerHelth(i);
	}
}

void CPlayer::Update()
{
	AABB player;
	AABB cbox;
	//AABB aenemy;
	CEnemy* enemy[80];
	bool check[80];
	bool bAttackFlag[80];
	bool AttackSet = true;
	CFirePlace* fireplace = CModeGame::GetFirePlace();
	MeshFiled* filed = CModeGame::GetFiled();
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();
	CModelInstancing* modelinstance = CModeGame::GetInstanceModel();
	CBox* GetBox = CModeGame::GetBox();
	SkinMeshPlayerModel->Update(this->player_model_->world_[this->player_model_->index_]);
	
	item_.Update();
	//D3DXMatrixRotationY(&SwordrotY, rotation_ + D3DXToRadian(-45.0f));
	//D3DXMatrixRotationX(&SwordrotX, D3DXToRadian(-45.0f));
	//D3DXMatrixRotationZ(&SwordrotZ, D3DXToRadian(25.0f));
	//CModel* GetSword = CModeGame::GetSword();
	D3DXMATRIX SwordrotY, SwordrotX, SwordrotZ, Swordrot;
	D3DXMatrixRotationY(&SwordrotY, rotation_ + D3DXToRadian(-90.0f));
	D3DXMatrixRotationX(&SwordrotX, D3DXToRadian(45.0f));
	D3DXMatrixRotationZ(&SwordrotZ, D3DXToRadian(115.0f));
	SetPosition = position_;
	Swordrot = SwordrotZ * SwordrotX * SwordrotY;
	//GetSword->SetMtxModel(Swordset);
	//GetSword->SetMtxRot(Swordrot);

	modelinfo_.modelpos = position_;
	modelAttackinfo_.modelpos = position_;

	position_.y = filed->GetHeight(position_);
	oldposition_ = position_;
	//判定用
	bool check2[600] = {};

	if (AttackCount_ <= 0)
	{
		if (pInputKeyboard->GetKeyTrigger(DIK_I)) {
			if (!MoveState)
			{
				MoveState = false;
				nTimeCount_ = 0;
				ActionPattern_ = ATTACK;
				SkinMeshPlayerModel->SetAnimSpeed(0.0125f);
				SkinMeshPlayerModel->MyChangeAnim(0.015);
			}
		}
	}
	else
	{
		if (AttackCount_ >= 30)
		{
			MoveState = true;
			if (!pInputKeyboard->GetKeyTrigger(DIK_I))
			{
				if (pInputKeyboard->GetKeyTrigger(DIK_W))
				{
					AttackCount_ = 0;
					nTimeCount_ = 0;
					ActionPattern_ = WALK;
					SkinMeshPlayerModel->SetAnimSpeed(0.0125f);
					SkinMeshPlayerModel->MyChangeAnim(0);
				}
			}
		}
	}
	if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(nullptr, modelinfo_, GetBox->Set()))
	{
		gameclear_ = true;
	}
	for (int i = 0; i < 600; i++)
	{
		check2[i] = CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hit_, modelinfo_, modelinstance->GetInstancingModelColision(i));
		if (!check2[i])
		{
			position_ += hit_;
		}
	}
	for (int i = 0; i < 80; i++)
	{
		enemy[i] = CModeGame::GetEnemy(i);
		check[i] = CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hit_, modelinfo_, enemy[i]->Set());
		bAttackFlag[i] = CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hit_, modelAttackinfo_, enemy[i]->Set());
		if (!check[i])
		{
			position_ += hit_;
		}
		if (!bAttackFlag[i]) {
			if (AttackCount_ <= 0)
			{
				if (pInputKeyboard->GetKeyTrigger(DIK_I)) {
					enemy[i]->EnemyDmg(1);
					if (!MoveState)
					{
						MoveState = false;
						nTimeCount_ = 0;
						ActionPattern_ = ATTACK;
						SkinMeshPlayerModel->SetAnimSpeed(0.0125f);
						SkinMeshPlayerModel->MyChangeAnim(0.015);
					}
				}
			}
			else
			{
				if (AttackCount_ >= 30)
				{
					MoveState = true;
					if (!pInputKeyboard->GetKeyTrigger(DIK_I))
					{
						if (pInputKeyboard->GetKeyTrigger(DIK_W))
						{
							AttackCount_ = 0;
							nTimeCount_ = 0;
							ActionPattern_ = WALK;
							SkinMeshPlayerModel->SetAnimSpeed(0.0125f);
							SkinMeshPlayerModel->MyChangeAnim(0);
						}
					}
				}
			}
		}
	}
	if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(nullptr, modelinfo_, fireplace->Set()))
	{
		healcooltime_++;
		if (healcooltime_ >= 120)
		{
			Playerheal(1);
			healcooltime_ = 0;
		}
	}

	if (pInputKeyboard->GetKeyTrigger(DIK_P))
	{
		SkinMeshPlayerModel->SetAnimSpeed(0.0f);
	}
	if (Helth_ >= 0)
	{
		PlayerHelth(Helth_);
	}
	if (Helth_ == 0)
	{
		nTimeCount_ = 0;
		ActionPattern_ = DOWN;
		SkinMeshPlayerModel->MyChangeAnim(0.08);
		SkinMeshPlayerModel->SetAnimSpeed(0.01f);
		Helth_ = -1;
	}
	if (MoveState)
	{
		PlayerControl();
	}
	
	PlayerAnimation();

	//行列にプレイヤーの位置を格納
	D3DXMatrixTranslation(&this->mtx_translation, position_.x, position_.y, position_.z);

	D3DXMatrixRotationY(&mtx_rotation_, rotation_);
	//姿勢ベクトルの回転
	D3DXVec3TransformNormal(&modelinfo_.modelpos, &modelinfo_.modelpos, &mtx_rotation_);
	/*ベクトルの正規化(1)*/
	D3DXVec3Normalize(&this->front_, &this->front_);
	D3DXVec3Normalize(&this->right_, &this->right_);
	
	D3DXMatrixScaling(&mtx_scale_, this->player_model_->scall_.x, this->player_model_->scall_.y, this->player_model_->scall_.z);
	
	this->player_model_->world_[this->player_model_->index_] = this->mtx_scale_ * this->mtx_rotation_ * this->mtx_translation;
	
	//this->player_model_->world_[this->player_model_->index_] = this->player_model_->world_[this->player_model_->index_];
}

void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// レンダリング
	pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	pDevice->SetRenderState(D3DRS_ZENABLE, true);

	D3DXMATRIX Swordset = SkinMeshPlayerModel->GetBoneMatrix("Bip001_Bip001_R_Finger01");
	item_.WeaponDraw(pDevice, Swordset);
	D3DXMATRIX Shildset = SkinMeshPlayerModel->GetBoneMatrix("Bip001_Bip001_L_Forearm");
	item_.ArmorDraw(pDevice, Shildset);

	pDevice->SetTransform(D3DTS_WORLD, &this->player_model_->world_[this->player_model_->index_]);

	SkinMeshPlayerModel->Draw(pDevice);

}
float CPlayer::PlayerRotation()
{
	return rotation_;
}
D3DXVECTOR3 CPlayer::GetPosition()
{
	return SetPosition;
}
void CPlayer::PlayerDmg(int dmg)
{
	if (Helth_ > 0)
	{
		Helth_ = Helth_ - dmg;
	}
}
void CPlayer::Playerheal(int heal)
{
	if (Helth_ < 40)
	{
		Helth_ = Helth_ + heal;
	}
}
bool CPlayer::GameClear()
{
	return this->gameclear_;
}
void CPlayer::PlayerControl(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();
	CScene2D* stamina = CModeGame::GetStaminaInfo();

	if (pInputKeyboard->GetKeyTrigger(DIK_I)) {
		MoveState = false;
	}
	if (!Running_)
	{
		if (Stamina_ <= 345.0f)
		{
			stamina->PolygonWidthSet(Stamina_);
			Stamina_ += 1.0f;
		}
		else
		{
			Running_ = true;
		}
	}
	if (pInputKeyboard->GetKeyTrigger(DIK_W))
	{
		nTimeCount_ = 0;
		ActionPattern_ = WALK;
		SkinMeshPlayerModel->SetAnimSpeed(0.0125f);
		SkinMeshPlayerModel->MyChangeAnim(0);
	}
	if (pInputKeyboard->GetKeyPress(DIK_W))
	{
		if (Running_)
		{
			if (pInputKeyboard->GetKeyPress(DIK_K) && pInputKeyboard->GetKeyPress(DIK_W))
			{
				ActionPattern_ = RUN;
			}
			else
			{
				ActionPattern_ = WALK;
			}
		}
		else
		{
			ActionPattern_ = WALK;
		}

		if (this->position_.y < -50.f)
		{
			MoveSpeed = D3DXVECTOR3(0.f, 0.f, -0.15f);
			D3DXVec3TransformNormal(&MoveSpeed, &MoveSpeed, &mtx_rotation_);

			position_ = position_ + MoveSpeed;
		}
		else
		{
			position_ = position_ - MoveSpeed;
		}

		AABB Colision;
		//player.min = position_;
		if (Running_)
		{
			if (pInputKeyboard->GetKeyTrigger(DIK_K))
			{
				nTimeCount_ = 0;
				ActionPattern_ = RUN;
				SkinMeshPlayerModel->SetAnimSpeed(0.0125f);
				SkinMeshPlayerModel->MyChangeAnim(0.008);
			}
			if (pInputKeyboard->GetKeyPress(DIK_K))
			{
				if (Stamina_ <= 0.0f)
				{
					Running_ = false;
				}
				Stamina_ -= 1.0f;
				stamina->PolygonWidthSet(Stamina_);
				ActionPattern_ = RUN;

				if (this->position_.y < -50.f)
				{
					MoveSpeed = D3DXVECTOR3(0.f, 0.f, -0.25f);
					D3DXVec3TransformNormal(&MoveSpeed, &MoveSpeed, &mtx_rotation_);
					position_ = position_ + MoveSpeed;
				}
				else
				{
					position_ = position_ - MoveSpeed;
				}
				AABB Colision;
				ActionPattern_ = RUN;

			}
			if (pInputKeyboard->GetKeyRelease(DIK_K))
			{
				nTimeCount_ = 0;
				ActionPattern_ = WALK;
				SkinMeshPlayerModel->SetAnimSpeed(0.0125f);
				SkinMeshPlayerModel->MyChangeAnim(0);
			}
		}
	}
	if (pInputKeyboard->GetKeyRelease(DIK_W))
	{
		nTimeCount_ = 0;
		ActionPattern_ = IDLE;
		SkinMeshPlayerModel->MyChangeAnim(0.15);
		SkinMeshPlayerModel->SetAnimSpeed(0.01f);
	}
	if (pInputKeyboard->GetKeyPress(DIK_A))
	{
		if (rotation_ >= -2.0f)
		{
			rotation_ += D3DXToRadian(-2.5f);
		}
		else if (rotation_ > 2.0f)
		{
			rotation_ -= D3DXToRadian(360.0f);
		}
		else if (rotation_ < -2.0f)
		{
			rotation_ += D3DXToRadian(360.0f);
		}
	}
	if (pInputKeyboard->GetKeyPress(DIK_D))
	{
		if (rotation_ <= 2.0f)
		{
			rotation_ += D3DXToRadian(2.5f);
		}
		else if (rotation_ > 2.0f)
		{
			rotation_ -= D3DXToRadian(360.0f);
		}
		else if (rotation_ < -2.0f)
		{
			rotation_ += D3DXToRadian(360.0f);
		}
	}
	if (Stamina_ <= 345.0f && !pInputKeyboard->GetKeyPress(DIK_K))
	{
		Stamina_ += 0.5f;
		stamina->PolygonWidthSet(Stamina_);
	}
}
void CPlayer::PlayerHelth(int helth)
{
	static int downcount = 0;
	switch (helth)
	{
	case 0:
		helthmanager[0]->helthstate(0);
		break;
	case 1:
		helthmanager[0]->helthstate(1);
		break;
	case 2:
		helthmanager[0]->helthstate(2);
		break;
	case 3:
		helthmanager[0]->helthstate(3);
		break;
	
	case 4:
		helthmanager[0]->helthstate(4);
		helthmanager[1]->helthstate(0);
		break;
	case 5:
		helthmanager[1]->helthstate(1);
		break;
	case 6:
		helthmanager[1]->helthstate(2);
		break;
	
	case 7:
		helthmanager[1]->helthstate(3);
		break;
	case 8:
		helthmanager[1]->helthstate(4);
		helthmanager[2]->helthstate(0);
		break;
	case 9:
		helthmanager[2]->helthstate(1);
		break;
	case 10:
		helthmanager[2]->helthstate(2);
		break;
	case 11:
		helthmanager[2]->helthstate(3);
		break;
	case 12:
		helthmanager[2]->helthstate(4);
		helthmanager[3]->helthstate(0);
		break;
	case 13:
		helthmanager[3]->helthstate(1);
		break;
	case 14:
		helthmanager[3]->helthstate(2);
		break;
	case 15:
		helthmanager[3]->helthstate(3);
		break;
	case 16:
		helthmanager[3]->helthstate(4);
		helthmanager[4]->helthstate(0);
		break;
	case 17:
		helthmanager[4]->helthstate(1);
		break;
	case 18:
		helthmanager[4]->helthstate(2);
		break;
	case 19:
		helthmanager[4]->helthstate(3);
		break;
	case 20:
		helthmanager[5]->helthstate(0);
		helthmanager[4]->helthstate(4);
		break;
	case 21:
		helthmanager[5]->helthstate(1);
		break;
	case 22:
		helthmanager[5]->helthstate(2);
		break;
	case 23:
		helthmanager[5]->helthstate(3);
		break;
	case 24:
		helthmanager[5]->helthstate(4);
		helthmanager[6]->helthstate(0);
		break;
	case 25:
		helthmanager[6]->helthstate(1);
		break;
	
	case 26:
		helthmanager[6]->helthstate(2);
		break;
	case 27:
		helthmanager[6]->helthstate(3);
		break;
	case 28:
		helthmanager[6]->helthstate(4);
		helthmanager[7]->helthstate(0);
		break;
	case 29:
		helthmanager[7]->helthstate(1);
		break;
	case 30:
		helthmanager[7]->helthstate(2);
		break;
	case 31:
		helthmanager[7]->helthstate(3);
		break;
	case 32:
		helthmanager[7]->helthstate(4);
		helthmanager[8]->helthstate(0);
		break;
	case 33:
		helthmanager[8]->helthstate(1);
		break;
	case 34:
		helthmanager[8]->helthstate(2);
		break;
	case 35:
		helthmanager[8]->helthstate(3);
		break;
	case 36:
		helthmanager[8]->helthstate(4);
		helthmanager[9]->helthstate(0);
		break;
	case 37:
		helthmanager[9]->helthstate(1);
		break;
	case 38:
		helthmanager[9]->helthstate(2);
		break;
	case 39:
		helthmanager[9]->helthstate(3);
		break;
	case 40:
		helthmanager[9]->helthstate(4);
		break;
	default:
		break;
	}
}
void CPlayer::PlayerAnimation()
{
	//-----------------------------------------------
	//更新処理
	//-----------------------------------------------
	switch (ActionPattern_)
	{
	case WALK:
		if (nTimeCount_ <= 50)
		{
			nTimeCount_++;
		}
		else
		{
			SkinMeshPlayerModel->MyChangeAnim(0);
			nTimeCount_ = 0;
		}
		break;
	case RUN:
		if (nTimeCount_ <= 30)
		{
			nTimeCount_++;
		}
		else
		{
			SkinMeshPlayerModel->MyChangeAnim(0.008);
			nTimeCount_ = 0;
		}
		break;
	case ATTACK:
		if (nTimeCount_ <= 60)
		{
			nTimeCount_++;
			AttackCount_++;
		}
		else
		{
			//SkinMeshPlayerModel->MyChangeAnim(0.015);
			SkinMeshPlayerModel->MyChangeAnim(0.15);
			SkinMeshPlayerModel->SetAnimSpeed(0.01f);
			nTimeCount_ = 0;
			AttackCount_ = 0;
			ActionPattern_ = IDLE;
		}
		break;
	case SKILL:
		if (nTimeCount_ <= 60)
		{
			nTimeCount_++;
		}
		else
		{
			//SkinMeshPlayerModel->MyChangeAnim(0.03);
			SkinMeshPlayerModel->MyChangeAnim(0.15);
			SkinMeshPlayerModel->SetAnimSpeed(0.01f);
			nTimeCount_ = 0;
			ActionPattern_ = IDLE;
		}
		break;
	case DIFFENCE:
		if (nTimeCount_ <= 30)
		{
			nTimeCount_++;
		}
		else
		{
			SkinMeshPlayerModel->MyChangeAnim(0.15);
			SkinMeshPlayerModel->SetAnimSpeed(0.01f);
			nTimeCount_ = 0;
			ActionPattern_ = IDLE;
		}
		break;
	case DIFFENCIVE:
		if (nTimeCount_ <= 60)
		{
			nTimeCount_++;
		}
		else
		{
			SkinMeshPlayerModel->MyChangeAnim(0.15);
			SkinMeshPlayerModel->SetAnimSpeed(0.01f);
			nTimeCount_ = 0;
			ActionPattern_ = IDLE;
		}
		break;
	case DAMAGE:
		if (nTimeCount_ <= 120)
		{
			nTimeCount_++;
		}
		else
		{
			SkinMeshPlayerModel->MyChangeAnim(0.15);
			SkinMeshPlayerModel->SetAnimSpeed(0.01f);
			nTimeCount_ = 0;
			ActionPattern_ = IDLE;
		}
		break;
	case DOWN:
		if (nTimeCount_ <= 500)
		{
			nTimeCount_++;
		}
		else
		{
			SkinMeshPlayerModel->SetAnimSpeed(0.0f);
			nTimeCount_ = 0;
			IsPlayer_ = true;
		}
		break;
	case IDLE:
		if (nTimeCount_ <= 90)
		{
			nTimeCount_++;
		}
		else
		{
			SkinMeshPlayerModel->MyChangeAnim(0.15);
			nTimeCount_ = 0;
		}
		break;
	default:
		break;
	}
}
D3DXMATRIX CPlayer::PlayerMtxtoCameraMtx(int type)
{
	if (type == 0)
	{
		return this->player_model_->world_[this->player_model_->index_];
	}
	D3DXMATRIX none;
	D3DXMatrixIdentity(&none);
	return none;
}
constexpr int PLAYER_FRONTVECTOR = 1;
constexpr int PLAYER_RIGHTVECTOR = 2;
constexpr int PLAYER_UPVECTOR = 3;
constexpr int PLAYER_POSITION = 4;

D3DXVECTOR3 CPlayer::PlayertoCamera(int type)
{
	if (type == PLAYER_FRONTVECTOR)
	{
		return  this->front_;
	}
	if (type == PLAYER_RIGHTVECTOR)
	{
		return this->right_;
	}
	if (type == PLAYER_UPVECTOR)
	{
		return this->up_;
	}
	if (type == PLAYER_POSITION)
	{
		return this->position_;
	}
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
void CPlayer::Uninit()
{
	nAttackCount_ = 0;
	item_.Uninit();
	
	if (this->player_model_->p_mesh_ != NULL)
	{
		this->player_model_->p_mesh_->Release();
		this->player_model_->p_mesh_ = NULL;
	}
	SkinMeshPlayerModel->Release();
	//delete SkinMeshPlayerModel;
	//delete SkinMeshPlayerModel;
	delete[] this->player_model_->p_mesh_;
	delete[] this->player_model_->p_meshtexture_;
	delete[] this->player_model_->p_meshmaterial_;	
}

void CPlayer::SetMtxView(D3DXMATRIX view)
{
	this->mtx_view_ = view;
}

void CPlayer::SetMtxProj(D3DXMATRIX proj)
{
	this->mtx_proj_ = proj;
}

CPlayer * CPlayer::Create(MATRIX3DMANAGER::Model * model, std::string filename)
{
	CPlayer* CreateModel = new CPlayer(0, model, filename);
	CreateModel->Init();
	return CreateModel;
}

float CPlayer::SetRotation()
{
	return rotation_;
}

bool CPlayer::IsPlayer()
{
	return IsPlayer_;
}

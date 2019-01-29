#include "enemy.h"
#include "camera.h"
#include "CModeGame.h"
#include "gamemanager.h"
#include "input.h"
#include <random>
constexpr int SHADOW_MAP_SIZE = 512;
void CEnemy::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMatrixIdentity(&this->mtx_rotation_);
	D3DXMatrixIdentity(&this->mtx_translation_);
	D3DXMatrixIdentity(&this->mtx_scale_);
	D3DXMatrixIdentity(&this->mtx_world_);
	rotation_ = 0.0f;
	rotflag = false;
	movespeed_ = 0.0f;
	//MovePattern_ = 1;
	nTimeCount = 0;
	EnemySkinMesh_ = new CSkinMesh();
	EnemySkinMesh_->Init(pDevice, "resource/xmodel/enemy.x");
	MeshFiled* filed = CModeGame::GetFiled();
	this->enemy_model_->position_.y = filed->GetHeight(this->enemy_model_->position_);
	this->firstposition_ = this->enemy_model_->position_;
	EnemySkinMesh_->ChangeAnim(20);
	EnemySkinMesh_->SetAnimSpeed(0.00025f);
	ActionPattern_ = 0;
	rotatecount_ = 5.0f;
	invrotatecount_ = -10.0f;
	Helth_ = 5;
}

void CEnemy::Update()
{
	MeshFiled* filed = CModeGame::GetFiled();
	CPlayer* player = CModeGame::GetPlayer();
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();
	float Rotate = 0.5f;
	D3DXMATRIX RotateMtx;
	D3DXVECTOR3 vecTarget(-Rotate, 0.0f, 0.0f);
	D3DXVECTOR3 hitvec;
	SphereInfo enemy_colision;
	float enemy_playerrad = 0.0f;
	SphereInfo sphere_player;
	SphereInfo sphere_enemy;
	static int deathtimer = 0;
	static int attacktimer = 0;
	D3DXVECTOR3 sphere_hit_vec;
	D3DXVECTOR3 enemy_playerlength;
	float movespeed = 0.05f;
	//moveangle = enemy_playerrad * 180.0f / D3DX_PI;
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> rand1000(-1000.0f, 1000.0f);
	//敵とプレイヤー当たり判定用
	enemy_colision.modelpos = enemy_model_->position_;
	enemy_colision.r = 3.0f;
	//敵のプレイヤー探索用
	sphere_player.modelpos = player->GetPosition();
	sphere_player.r = 2.0f;
	sphere_enemy.modelpos = enemy_model_->position_;
	sphere_enemy.r = 20.0f;
	enemycolision_.modelpos = this->enemy_model_->position_;
	enemycolision_.r = 2.0f;

	enemy_playerlength = player->GetPosition() - enemy_model_->position_;
	D3DXVec3Normalize(&enemy_playerlength, &enemy_playerlength);
	D3DXVec3Normalize(&front_, &front_);
	D3DXVec3TransformNormal(&front_, &front_, &mtx_rotation_);
	bool hit = CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, player->Set());
	if (ActionPattern_ <= 0)
	{
		EnemySkinMesh_->SetAnimSpeed(0.00025f);
	}
	//-----------------------------------------------
	//更新処理
	//-----------------------------------------------
	switch (ActionPattern_)
	{
	case MOVE:
		if (nTimeCount <= 60)
		{
			nTimeCount++;
		}
		else
		{
			EnemySkinMesh_->MyChangeAnim(0);
			nTimeCount = 0;
		}
		break;
	case ATTACK:
		if (nTimeCount <= 60)
		{
			nTimeCount++;
			attacktimer++;
		}
		else
		{
			EnemySkinMesh_->MyChangeAnim(4);
			nTimeCount = 0;
			if (attacktimer >= 60)
			{
				player->PlayerDmg(1);
				attacktimer = 0;
				ActionPattern_ = MOVE;
			}
		}
		break;
	case DAMAGE:
		if (nTimeCount <= 30)
		{
			nTimeCount++;
		}
		else
		{
			EnemySkinMesh_->MyChangeAnim(5);
			nTimeCount = 0;
		}
		break;
	case DOWN:
		if (nTimeCount <= 60)
		{
			nTimeCount++;
			deathtimer++;
		}
		else
		{
			EnemySkinMesh_->MyChangeAnim(6);
			nTimeCount = 0;	
			if (deathtimer >= 60)
			{
				player->Playerheal(2);
				D3DXVECTOR3 areaposition;
				do {
					areaposition = { (float)rand1000(mt),0.0f,(float)rand1000(mt) };
					areaposition.y = filed->GetHeight(areaposition);
				} while (areaposition.y > -55.f);
				enemy_model_->position_ = areaposition;
				ActionPattern_ = MOVE;
				Helth_ = 5;
				deathtimer = 0;
			}
		}
		break;
	default:
		break;
	}
	if (Helth_ <= 0)
	{
		ActionPattern_ = DOWN;
	}
	if(Helth_ > 0)
	{

		if (!hit)
		{

		}
		switch (MovePattern_)
		{
		case SIDEMOVE:
			if (!rotflag)
			{
				enemy_model_->position_.x = enemy_model_->position_.x + 0.1f;
				this->enemy_model_->position_.y = filed->GetHeight(this->enemy_model_->position_);
				if (rotation_ >= D3DXToRadian(-90.0f))
				{
					rotation_ = rotation_ + D3DXToRadian(-10.0f);
				}
				if (enemy_model_->position_.x > firstposition_.x + 15.0f)
				{
					rotflag = true;
				}
			}
			if (rotflag)
			{
				enemy_model_->position_.x = enemy_model_->position_.x - 0.1f;
				this->enemy_model_->position_.y = filed->GetHeight(this->enemy_model_->position_);
				if (rotation_ >= D3DXToRadian(-270.0f))
				{
					rotation_ = rotation_ + D3DXToRadian(-10.0f);
				}
				else
				{
					if (enemy_model_->position_.x < firstposition_.x - 15.0f)
					{
						rotation_ = (rotation_ * -1.0f) + D3DXToRadian(-180.0f);
						rotflag = false;
					}
				}
			}
			break;
		case LENGTHMOVE:
			if (!rotflag)
			{
				enemy_model_->position_.z = enemy_model_->position_.z + 0.1f;
				this->enemy_model_->position_.y = filed->GetHeight(this->enemy_model_->position_);
				if (rotation_ >= D3DXToRadian(-180.0f))
				{
					rotation_ = rotation_ + D3DXToRadian(-10.0f);
				}
				if (enemy_model_->position_.z > firstposition_.z + 15.0f)
				{
					rotflag = true;
				}
			}
			if (rotflag)
			{
				enemy_model_->position_.z = enemy_model_->position_.z - 0.1f;
				this->enemy_model_->position_.y = filed->GetHeight(this->enemy_model_->position_);
				if (rotation_ >= D3DXToRadian(-360.0f))
				{
					rotation_ = rotation_ + D3DXToRadian(-10.0f);
				}
				else
				{
					if (enemy_model_->position_.z < firstposition_.z - 15.0f)
					{
						rotation_ = (rotation_ * -1.0f) + D3DXToRadian(-360.0f);
						rotflag = false;
					}
				}
			}
			break;
		case CIRCLEMOVE:
			rotation_ = 1.5f;
			D3DXMatrixRotationY(&RotateMtx, timeGetTime() / 1000.0f);
			D3DXVec3TransformCoord(&vecTarget, &vecTarget, &RotateMtx);
			D3DXVec3Add(&this->enemy_model_->position_, &vecTarget, &this->enemy_model_->position_);
			this->enemy_model_->position_.y = filed->GetHeight(this->enemy_model_->position_);

			rotatecount_ = rotatecount_ + D3DXToRadian(0.75f);
			D3DXMatrixRotationY(&this->mtx_rotation_, rotatecount_);
			if (rotatecount_ >= D3DXToRadian(360.0f))
			{
				rotatecount_ = 0.0f;
			}
			break;
		case INVCIRCLEMOVE:
			rotation_ = 1.0f;
			D3DXMatrixRotationY(&RotateMtx, -(timeGetTime() / 1000.0f));
			D3DXVec3TransformCoord(&vecTarget, &vecTarget, &RotateMtx);
			D3DXVec3Add(&this->enemy_model_->position_, &vecTarget, &this->enemy_model_->position_);
			this->enemy_model_->position_.y = filed->GetHeight(this->enemy_model_->position_);

			invrotatecount_ = invrotatecount_ + D3DXToRadian(-0.75f);
			D3DXMatrixRotationY(&this->mtx_rotation_, invrotatecount_);
			if (invrotatecount_ <= D3DXToRadian(-360.0f))
			{
				invrotatecount_ = 0.0f;
			}
			break;
		case SPHERE:
			if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(nullptr, sphere_player, sphere_enemy))
			{
				if (player->GetPosition().x < sphere_enemy.modelpos.x)
				{
					if (CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
					{
						ActionPattern_ = MOVE;
					}
					if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
					{
						movespeed = 0.0f;
						//どうするかを記入
						this->enemy_model_->position_.x = this->enemy_model_->position_.x + movespeed;
						ActionPattern_ = ATTACK;
					}

					this->enemy_model_->position_.x = this->enemy_model_->position_.x - movespeed;
					
					if (player->GetPosition().z > enemy_model_->position_.z)
					{
						enemy_model_->position_.z = enemy_model_->position_.z + movespeed;
						if (CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
						{
							ActionPattern_ = MOVE;
						}
						if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
						{
							movespeed = 0.0f;
							//どうするかを記入
							enemy_model_->position_.z = enemy_model_->position_.z - movespeed;
							ActionPattern_ = ATTACK;
						}
					}
					if (player->GetPosition().z < enemy_model_->position_.z)
					{
						enemy_model_->position_.z = enemy_model_->position_.z - movespeed;

						if (CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
						{
							ActionPattern_ = MOVE;
						}
						if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
						{
							//どうするかを記入
							enemy_model_->position_.z = enemy_model_->position_.z + movespeed;
							ActionPattern_ = ATTACK;
						}
					}
				}
				if (player->GetPosition().x > sphere_enemy.modelpos.x)
				{
					if (CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
					{
						ActionPattern_ = MOVE;
					}
					if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
					{
						movespeed = 0.0f;
						//どうするかを記入
						this->enemy_model_->position_.x = this->enemy_model_->position_.x - movespeed;
						ActionPattern_ = ATTACK;
					}
					this->enemy_model_->position_.x = this->enemy_model_->position_.x + movespeed;
					if (player->GetPosition().z > enemy_model_->position_.z)
					{
						enemy_model_->position_.z = enemy_model_->position_.z + movespeed;

						if (CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
						{
							ActionPattern_ = MOVE;
						}
						if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
						{
							enemy_model_->position_.z = enemy_model_->position_.z - movespeed;
							//どうするかを記入
							ActionPattern_ = ATTACK;
						}

					}
					if (player->GetPosition().z < enemy_model_->position_.z)
					{
						enemy_model_->position_.z = enemy_model_->position_.z - movespeed;

						if (CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
						{
							ActionPattern_ = MOVE;
						}
						if (!CSphereColision::Collision_detection_of_Sphere_and_Sphere(&hitvec, enemy_colision, sphere_player))
						{
							enemy_model_->position_.z = enemy_model_->position_.z + movespeed;
							//どうするかを記入
							ActionPattern_ = ATTACK;
						}
					}
				}
				rotation_ = atan2f(enemy_playerlength.x, enemy_playerlength.z);
				rotation_ = rotation_ + D3DX_PI;
			}
			
			break;
		default:
			break;
		}
		if (pInputKeyboard->GetKeyTrigger(DIK_P))
		{
			EnemySkinMesh_->SetAnimSpeed(0.0f);
		}
		
		this->enemy_model_->position_.y = filed->GetHeight(this->enemy_model_->position_);
		
		//rotation_ = 0.0f;
		D3DXMatrixScaling(&this->mtx_scale_, this->enemy_model_->scall_.x, this->enemy_model_->scall_.y, this->enemy_model_->scall_.z);
		if (MovePattern_ < CIRCLEMOVE)
		{
			D3DXMatrixRotationY(&this->mtx_rotation_, rotation_ );
		}
		D3DXMatrixRotationY(&this->mtx_rotation_, rotation_);
		D3DXMatrixTranslation(&this->mtx_translation_, this->enemy_model_->position_.x, this->enemy_model_->position_.y, this->enemy_model_->position_.z);

		this->mtx_world_ = this->mtx_scale_ * this->mtx_rotation_ * this->mtx_translation_;
		EnemySkinMesh_->Update(this->mtx_world_);
	}
}

void CEnemy::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//ライトON
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	//αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 64);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	EnemySkinMesh_->Draw(pDevice);
}

void CEnemy::Uninit()
{
	EnemySkinMesh_->Release();
	//delete EnemySkinMesh_;
}
CEnemy *CEnemy::Create(int MovePattern,MATRIX3DMANAGER::Model* model,Area MoveArea, std::string filename)
{
	CEnemy* CreateEnemy = new CEnemy(0, MovePattern, MoveArea, model, (LPSTR)filename.c_str());
	CreateEnemy->Init();
	return CreateEnemy;
}

SphereInfo CEnemy::Set()
{
	return this->enemycolision_;
}

D3DXVECTOR3 CEnemy::SetPosition()
{
	return this->enemy_model_->position_;
}

void CEnemy::EnemyDmg(int dmg)
{
	Helth_ = Helth_ - dmg;
}

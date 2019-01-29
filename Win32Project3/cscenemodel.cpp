#include "cscenemodel.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "CModeGame.h"
D3DXMATRIX CModel::proj_;
D3DXMATRIX CModel::view_;
void CModel::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX(this->filename_.c_str(), D3DXMESH_SYSTEMMEM, pDevice, NULL, &pD3DXMtrlBuffer, NULL, &this->model_->dw_material_, &this->model_->p_mesh_)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", this->filename_.c_str(), MB_OK);
		PostQuitMessage(0);
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	this->model_->p_meshmaterial_ = new D3DMATERIAL9[this->model_->dw_material_];
	this->model_->p_meshtexture_ = new LPDIRECT3DTEXTURE9[this->model_->dw_material_];

	for (DWORD i = 0; i < this->model_->dw_material_; i++)
	{
		this->model_->p_meshmaterial_[i] = d3dxMaterials[i].MatD3D;
		this->model_->p_meshmaterial_[i].Ambient = this->model_->p_meshmaterial_->Diffuse;
		this->model_->p_meshtexture_[i] = NULL;

		if (d3dxMaterials[i].pTextureFilename != NULL &&strlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice, d3dxMaterials[i].pTextureFilename, &this->model_->p_meshtexture_[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				PostQuitMessage(0);
			}
		}
	}

	pD3DXMtrlBuffer->Release();
	
	////シェーダーを読み込む
	if (FAILED(D3DXCreateEffectFromFile(pDevice, "resource/shader/Min.fx", NULL, NULL, 0, NULL, &effect_, NULL)))
	{
		MessageBox(NULL, "シェーダーファイル読み込み失敗", NULL, MB_OK);
		PostQuitMessage(0);
	}

	this->technique_ = this->effect_->GetTechniqueByName("tecMinimum");
	this->technique_ = this->effect_->GetParameterByName(NULL, "mWVP");
	//this->technique_ = this->effect_->GetParameterByName(NULL, "mWLP");
	//this->technique_ = this->effect_->GetParameterByName(NULL, "mWLPB");
	//this->technique_ = this->effect_->GetParameterByName(NULL, "vCol");
	//this->technique_ = this->effect_->GetParameterByName(NULL, "vLightDir");
	//this->shadowmap_ = this->effect_->GetParameterByName(NULL, "ShadowMap");
	this->srcmap_ = this->effect_->GetParameterByName(NULL, "SrcMap");

	constexpr int SHADOW_MAP_SIZE = 512;
	if (FAILED(pDevice->CreateDepthStencilSurface(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, true, &this->pshadowmapz_, NULL)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->pshadowmap_, NULL)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(this->pshadowmap_->GetSurfaceLevel(0, &this->pshadowmapsurf_)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->pedgemap_, NULL)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(this->pedgemap_->GetSurfaceLevel(0, &this->pedgemapsurf_)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->psoftmap_[0], NULL)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(this->psoftmap_[0]->GetSurfaceLevel(0, &this->psoftmapsurf_[0])))
	{
		PostQuitMessage(0);
	}
	if (FAILED(pDevice->CreateTexture(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->psoftmap_[1], NULL)))
	{
		PostQuitMessage(0);
	}
	if (FAILED(this->psoftmap_[1]->GetSurfaceLevel(0, &this->psoftmapsurf_[1])))
	{
		PostQuitMessage(0);
	}

	this->effect_->OnResetDevice();
	modelinfo_.modelpos = model_->position_;
	modelinfo_.r = 1.0f;
	D3DXMatrixIdentity(&model_->world_[model_->index_]);
}

void CModel::Update()
{
	LPDIRECT3DDEVICE9 device = GetDevice();
	CPlayer* playerinfo = CModeGame::GetPlayer();
	D3DXMatrixIdentity(&this->model_->world_[this->model_->index_]);
	D3DXMatrixTranslation(&this->mtxtranslasion, setmtx._41, setmtx._42, setmtx._43);

	if (this->model_->isscall_)
	{
		D3DXMatrixScaling(&this->mtxscaling, this->model_->scall_.x, this->model_->scall_.y, this->model_->scall_.z);
	}

	//D3DXMatrixRotationY(&this->mtxrotation, playerinfo->SetRotation());
	
	this->model_->world_[this->model_->index_] = this->mtxscaling * this->mtxrotation * this->mtxtranslasion;
}

void CModel::Draw()
{
	LPDIRECT3DDEVICE9 device = GetDevice();

	D3DXMATRIX mtxall = this->model_->world_[this->model_->index_] * this->view_ * this->proj_;

	//D3DXMATRIX mtxall = setmtx * this->view_ * this->proj_;
	device->SetTransform(D3DTS_WORLD, &mtxall);
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	device->SetRenderState(D3DRS_ZENABLE, true);
	//*シェーダーの関数名*/
	this->effect_->SetTechnique("tecMinimum");
	//
	//D3DXMATRIX mtxall = 　/*ワールド * ビュー * プロジェクション*/
	//D3DXMATRIX mtxall = this->model_->world_[this->model_->index_] * this->view_ * this->proj_;
	
	this->effect_->SetMatrix("mWVP", (D3DXMATRIX*)&mtxall);
	this->effect_->Begin(NULL, 0);
	this->effect_->BeginPass(0);
	
	for (DWORD i = 0; i < this->model_->dw_material_; i++)
	{
		this->effect_->SetTexture("texDecal",this->model_->p_meshtexture_[i]);
		this->model_->p_mesh_->DrawSubset(i);
	}

	this->effect_->EndPass();
	this->effect_->End();
}

void CModel::Uninit()
{
	if (this->model_->p_mesh_ != NULL)
	{
		this->model_->p_mesh_->Release();
		this->model_->p_mesh_ = NULL;
	}
	
	delete[] this->model_->p_mesh_;
	delete[] this->model_->p_meshtexture_;
	delete[] this->model_->p_meshmaterial_;
}

void CModel::SetMtxView(D3DXMATRIX view)
{
	view_ = view;
}

void CModel::SetMtxProj(D3DXMATRIX proj)
{
	proj_ = proj;
}
void CModel::SetMtxModel(D3DXMATRIX model)
{
	setmtx = model;
}
void CModel::SetMtxRot(D3DXMATRIX Rot)
{
	this->mtxrotation = Rot;
}
SphereInfo CModel::Set()
{
	return modelinfo_;
}
CModel * CModel::Create(MATRIX3DMANAGER::Model * model, std::string filename)
{
	CModel* CreateModel = new CModel( 0, model, filename);
	CreateModel->Init();
	return CreateModel;
}

//D3DXMATRIX CModel::GetXModelMatrix(Model * model, int num)
//{
//	return this->model_->world_[num];
//}

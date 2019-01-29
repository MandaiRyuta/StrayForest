#include "modelInstancing.h"
#include "meshfiled.h"
#include "CModeGame.h"

#include <string>
#include <random>
D3DXMATRIX CModelInstancing::view_;
D3DXMATRIX CModelInstancing::proj_;

void CModelInstancing::Init()
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

		if (d3dxMaterials[i].pTextureFilename != NULL &&lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice, d3dxMaterials[i].pTextureFilename, &this->model_->p_meshtexture_[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", d3dxMaterials[i].pTextureFilename, MB_OK);
				PostQuitMessage(0);
			}
		}
	}

	pD3DXMtrlBuffer->Release();
	pD3DXMtrlBuffer = NULL;
	for (int t = 0; t < MAX_MODELNUM; t++)
	{
		mesh[t] = model_->p_mesh_;
	}

	this->model_->p_mesh_->GetVertexBuffer(&vertexbuf_);
	this->model_->p_mesh_->GetIndexBuffer(&indexbuf_);
	pDevice->CreateVertexBuffer(
		sizeof(MODEL_WORLD1) * MAX_MODELNUM,
		0,
		0,
		D3DPOOL_MANAGED,
		&this->pWorld1Buffer_,
		0
	);

	pDevice->CreateVertexBuffer(
		sizeof(MODEL_WORLD2) * MAX_MODELNUM,
		0,
		0,
		D3DPOOL_MANAGED,
		&this->pWorld2Buffer_,
		0
	);

	pDevice->CreateVertexBuffer(
		sizeof(MODEL_WORLD3) * MAX_MODELNUM,
		0,
		0,
		D3DPOOL_MANAGED,
		&this->pWorld3Buffer_,
		0
	);

	pDevice->CreateVertexBuffer(
		sizeof(MODEL_WORLD4) * MAX_MODELNUM,
		0,
		0,
		D3DPOOL_MANAGED,
		&this->pWorld4Buffer_,
		0
	);

	pWorld1 = new MODEL_WORLD1[MAX_MODELNUM];
	pWorld2 = new MODEL_WORLD2[MAX_MODELNUM];
	pWorld3 = new MODEL_WORLD3[MAX_MODELNUM];
	pWorld4 = new MODEL_WORLD4[MAX_MODELNUM];

	for (DWORD dw = 0; dw < MAX_MODELNUM; dw++)
	{
		D3DXMATRIX mtxWorld;
		D3DXMatrixIdentity(&mtxWorld);
		D3DXMatrixTranslation(&mtxWorld, 0.0f, 5.0f, 10.0f);
		pWorld1[dw].m11 = mtxWorld._11;
		pWorld1[dw].m12 = mtxWorld._12;
		pWorld1[dw].m13 = mtxWorld._13;
		pWorld1[dw].m14 = mtxWorld._14;

		pWorld2[dw].m21 = mtxWorld._21;
		pWorld2[dw].m22 = mtxWorld._22;
		pWorld2[dw].m23 = mtxWorld._23;
		pWorld2[dw].m24 = mtxWorld._24;

		pWorld3[dw].m31 = mtxWorld._31;
		pWorld3[dw].m32 = mtxWorld._32;
		pWorld3[dw].m33 = mtxWorld._33;
		pWorld3[dw].m34 = mtxWorld._34;

		pWorld4[dw].m41 = mtxWorld._41;
		pWorld4[dw].m42 = mtxWorld._42;
		pWorld4[dw].m43 = mtxWorld._43;
		pWorld4[dw].m44 = mtxWorld._44;
	}

	D3DVERTEXELEMENT9 declElems[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	// Local coord
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	// UV
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },	// 法線
																						//以下行列データ
		{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	// ワールド行列1
		{ 2, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },	// ワールド行列2
		{ 3, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },	// ワールド行列3
		{ 4, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },	// ワールド行列4

		D3DDECL_END()
	};

	pDevice->CreateVertexDeclaration(declElems, &pDecl_);

	////シェーダーを読み込む
	if (FAILED(D3DXCreateEffectFromFile(pDevice, "resource/shader/3dinstance.fx", NULL, NULL, 0, NULL, &effect_, NULL)))
	{
		MessageBox(NULL, "シェーダーファイル読み込み失敗", NULL, MB_OK);
		PostQuitMessage(0);
	}

	this->technique_ = this->effect_->GetTechniqueByName("tecMinimum");
	this->technique_ = this->effect_->GetParameterByName(NULL, "mWVP");
	this->technique_ = this->effect_->GetParameterByName(NULL, "mWLP");
	this->technique_ = this->effect_->GetParameterByName(NULL, "mWLPB");
	this->technique_ = this->effect_->GetParameterByName(NULL, "vCol");
	this->technique_ = this->effect_->GetParameterByName(NULL, "vLightDir");
	this->shadowmap_ = this->effect_->GetParameterByName(NULL, "ShadowMap");
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

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> rand500(-1000.0f, 1000.0f);
	MeshFiled* meshfiled;
	meshfiled = CModeGame::GetFiled();
	D3DXMATRIX scale;
	//グレースケール
	for (int i = 0, e = -50; i < 100, e < 50; i++, e++)
	{
		//D3DXVECTOR3 boardpos(float(rand500(mt)), 0.0f, float(rand500(mt)));
		D3DXVECTOR3 boardpos(1000.f, 0.0f, float(e * 20.f));
		float y = meshfiled->GetHeight(boardpos);
		D3DXMatrixIdentity(&this->model_->world_[i]);
		D3DXMatrixTranslation(&this->mtxtranslasion[i], boardpos.x, y, boardpos.z);

		if (this->model_->isscall_)
		{
			D3DXMatrixScaling(&scale, this->model_->scall_.x, this->model_->scall_.y, this->model_->scall_.z);
		}

		D3DXMatrixRotationY(&this->mtxrotation[i], this->model_->rotatiion_ * D3DX_PI / 180.f);

		this->model_->world_[i] = scale * this->mtxrotation[i] * this->mtxtranslasion[i];
	}
	for (int i = 100, n = -50; i < 200, n < 50; i++, n++)
	{
		D3DXVECTOR3 boardpos(-1000.f, 0.0f, float(n * 20.f));
		float y = meshfiled->GetHeight(boardpos);
		D3DXMatrixIdentity(&this->model_->world_[i]);
		D3DXMatrixTranslation(&this->mtxtranslasion[i], boardpos.x, y, boardpos.z);

		if (this->model_->isscall_)
		{
			D3DXMatrixScaling(&scale, this->model_->scall_.x, this->model_->scall_.y, this->model_->scall_.z);
		}

		D3DXMatrixRotationY(&this->mtxrotation[i], this->model_->rotatiion_ * D3DX_PI / 180.f);

		this->model_->world_[i] = scale * this->mtxrotation[i] * this->mtxtranslasion[i];
	}
	for (int i = 200, c = -50; i < 300, c < 50; i++, c++)
	{
		D3DXVECTOR3 boardpos(float(c * 20.f), 0.0f, 1000.f);
		float y = meshfiled->GetHeight(boardpos);
		D3DXMatrixIdentity(&this->model_->world_[i]);
		D3DXMatrixTranslation(&this->mtxtranslasion[i], boardpos.x, y, boardpos.z);

		if (this->model_->isscall_)
		{
			D3DXMatrixScaling(&scale, this->model_->scall_.x, this->model_->scall_.y, this->model_->scall_.z);
		}

		D3DXMatrixRotationY(&this->mtxrotation[i], this->model_->rotatiion_ * D3DX_PI / 180.f);

		this->model_->world_[i] = scale * this->mtxrotation[i] * this->mtxtranslasion[i];
	}
	for (int i = 300, f = -50; i < 400, f < 50; i++, f++)
	{
		D3DXVECTOR3 boardpos(float(f * 20.f), 0.0f, -1000.f);
		float y = meshfiled->GetHeight(boardpos);
		D3DXMatrixIdentity(&this->model_->world_[i]);
		D3DXMatrixTranslation(&this->mtxtranslasion[i], boardpos.x, y, boardpos.z);

		if (this->model_->isscall_)
		{
			D3DXMatrixScaling(&scale, this->model_->scall_.x, this->model_->scall_.y, this->model_->scall_.z);
		}

		D3DXMatrixRotationY(&this->mtxrotation[i], this->model_->rotatiion_ * D3DX_PI / 180.f);

		this->model_->world_[i] = scale * this->mtxrotation[i] * this->mtxtranslasion[i];
	}
	float y;
	D3DXVECTOR3 boardpos;
	for (int i = 400; i < MAX_MODELNUM; i++)
	{
		do {
			boardpos = D3DXVECTOR3(float(rand500(mt)), 0.0f, float(rand500(mt)));
			y = meshfiled->GetHeight(boardpos);
		} while (y > -50.0f);
		D3DXMatrixIdentity(&this->model_->world_[i]);
		D3DXMatrixTranslation(&this->mtxtranslasion[i], boardpos.x, y, boardpos.z);
		instancemodel[i - 400].modelpos = D3DXVECTOR3(boardpos.x, y, boardpos.z);
		instancemodel[i - 400].r = 3.0f;
		if (this->model_->isscall_)
		{
			D3DXMatrixScaling(&scale, this->model_->scall_.x, this->model_->scall_.y, this->model_->scall_.z);
		}

		D3DXMatrixRotationY(&this->mtxrotation[i], this->model_->rotatiion_ * D3DX_PI / 180.f);

		this->model_->world_[i] = scale * this->mtxrotation[i] * this->mtxtranslasion[i];
	}
}

void CModelInstancing::Uninit()
{
	delete pWorld1;
	delete pWorld2;
	delete pWorld3;
	delete pWorld4;
	if (this->effect_ != NULL)
	{
		this->effect_->Release();
		this->effect_ = NULL;
	}
	if (this->pDecl_ != NULL)
	{
		this->pDecl_->Release();
		this->pDecl_ = NULL;
	}
	if (this->pWorld1Buffer_ != NULL)
	{
		pWorld1Buffer_->Release();
		pWorld1Buffer_ = NULL;
	}
	if (this->pWorld2Buffer_ != NULL)
	{
		pWorld2Buffer_->Release();
		pWorld2Buffer_ = NULL;
	}
	if (this->pWorld3Buffer_ != NULL)
	{
		pWorld3Buffer_->Release();
		pWorld3Buffer_ = NULL;
	}
	if (this->pWorld4Buffer_ != NULL)
	{
		pWorld4Buffer_->Release();
		pWorld4Buffer_ = NULL;
	}
	if (this->vertexbuf_ != NULL)
	{
		this->vertexbuf_->Release();
		this->vertexbuf_ = NULL;
	}
	if (this->indexbuf_ != NULL)
	{
		this->indexbuf_->Release();
		this->indexbuf_ = NULL;
	}
	if (this->model_->p_mesh_ != NULL)
	{
		this->model_->p_mesh_->Release();
		this->model_->p_mesh_ = NULL;
	}
	delete[] this->model_->p_mesh_;
	delete[] this->model_->p_meshtexture_;
	delete[] this->model_->p_meshmaterial_;
}

void CModelInstancing::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (UINT)MAX_MODELNUM); //ここだけ板ポリゴンの枚数
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | (UINT)1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | (UINT)1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | (UINT)1);
	pDevice->SetStreamSourceFreq(4, D3DSTREAMSOURCE_INSTANCEDATA | (UINT)1);

	pDevice->SetVertexDeclaration(pDecl_);

	pDevice->SetStreamSource(0, vertexbuf_, 0, model_->p_mesh_->GetNumBytesPerVertex());
	pDevice->SetStreamSource(1, pWorld1Buffer_, 0, sizeof(WORLD1));
	pDevice->SetStreamSource(2, pWorld2Buffer_, 0, sizeof(WORLD2));
	pDevice->SetStreamSource(3, pWorld3Buffer_, 0, sizeof(WORLD3));
	pDevice->SetStreamSource(4, pWorld4Buffer_, 0, sizeof(WORLD4));
	pDevice->SetIndices(indexbuf_);
	pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	//ライトON
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	//αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 64);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//*シェーダーの関数名*/
	this->effect_->SetTechnique("tecMinimum");
	D3DXMATRIX mtxall[MAX_MODELNUM] = {};
	//D3DXMATRIX mtxall = 　/*ワールド * ビュー * プロジェクション*/
	this->effect_->Begin(NULL, 0);
	for (int a = 0; a < MAX_MODELNUM; a++)
	{
		this->effect_->BeginPass(0);
		mtxall[a] = this->model_->world_[a] * this->view_ * this->proj_;
		D3DXVECTOR4 color = { model_->color_.r,model_->color_.g,model_->color_.b,model_->color_.a };
		this->effect_->SetMatrix("mWVP", (D3DXMATRIX*)&mtxall[a]);
		this->effect_->SetVector("mColor", &color);
		for (DWORD i = 0; i < this->model_->dw_material_; i++)
		{
			//pDevice->SetTexture(0, this->model_->p_meshtexture_[i]);
			this->effect_->SetTexture("texDecal", this->model_->p_meshtexture_[i]);
			mesh[a]->DrawSubset(i);
			//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,mesh[a]->GetNumVertices(), 0, mesh[a]->GetNumFaces());
		}
		this->effect_->EndPass();
	}
	this->effect_->End();
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSourceFreq(3, 1);
	pDevice->SetStreamSourceFreq(4, 1);
}

void CModelInstancing::Update()
{
	LPDIRECT3DDEVICE9 device = GetDevice();
}

void CModelInstancing::SetMtxView(D3DXMATRIX view)
{
	view_ = view;
}

void CModelInstancing::SetMtxProj(D3DXMATRIX proj)
{
	proj_ = proj;
}

CModelInstancing * CModelInstancing::Create(MATRIX3DMANAGER::Model * model, std::string filename)
{
	CModelInstancing* CreateModel = new CModelInstancing(0, model, filename);
	CreateModel->Init();
	return CreateModel;
}

SphereInfo CModelInstancing::GetInstancingModelColision(int num)
{
	return instancemodel[num];
}
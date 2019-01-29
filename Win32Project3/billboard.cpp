#include "billboard.h"
#include "gamemanager.h"
#include "camera.h"
#include "CModeGame.h"
#include <random>

void CBillBoard::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->CreateVertexBuffer(
		sizeof(BillbaordSet) * 4,
		0,
		0,
		D3DPOOL_MANAGED,
		&this->vertexbuf_,
		0
	);

	pDevice->CreateVertexBuffer(
		sizeof(WORLD1) * BoardNumMax,
		0,
		0,
		D3DPOOL_MANAGED,
		&this->pWorld1Buffer_,
		0
	);

	pDevice->CreateVertexBuffer(
		sizeof(WORLD2) * BoardNumMax,
		0,
		0,
		D3DPOOL_MANAGED,
		&this->pWorld2Buffer_,
		0
	);

	pDevice->CreateVertexBuffer(
		sizeof(WORLD3) * BoardNumMax,
		0,
		0,
		D3DPOOL_MANAGED,
		&this->pWorld3Buffer_,
		0
	);

	pDevice->CreateVertexBuffer(
		sizeof(WORLD4) * BoardNumMax,
		0,
		0,
		D3DPOOL_MANAGED,
		&this->pWorld4Buffer_,
		0
	);

	pWorld1 = new WORLD1[BoardNumMax];
	pWorld2 = new WORLD2[BoardNumMax];
	pWorld3 = new WORLD3[BoardNumMax];
	pWorld4 = new WORLD4[BoardNumMax];

	for (DWORD dw = 0; dw < BoardNumMax; dw++)
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


	HRESULT hr;
	hr = D3DXCreateTextureFromFile
	(
		pDevice,
		this->filename_.c_str(),
		&this->texture_[0]
	);
	if (FAILED(hr))
	{
		MessageBox(NULL, "テクスチャが読み込めません", "エラー", MB_OK);
		PostQuitMessage(0);
	}

	BillbaordSet* pV;
	this->vertexbuf_->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	BillbaordSet v[4] =
	{
		{ D3DXVECTOR3(2.5f, -2.5f, 0.0f), D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f) },
		{ D3DXVECTOR3(2.5f, 2.5f,  0.0f), D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f) },
		{ D3DXVECTOR3(-2.5f, 2.5f,  0.0f), D3DXVECTOR2(1.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f) },
		{ D3DXVECTOR3(-2.5f, -2.5f, 0.0f), D3DXVECTOR2(1.0f,1.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f) },
	};

	memcpy(&pV[0], &v[0], sizeof(BillbaordSet) * 4);

	this->vertexbuf_->Unlock();
	void *p = 0;

	this->pWorld1Buffer_->Lock(0, 0, &p, 0);
	memcpy(p, pWorld1, sizeof(WORLD1) * BoardNumMax);
	pWorld1Buffer_->Unlock();
	p = 0;

	pWorld2Buffer_->Lock(0, 0, &p, 0);
	memcpy(p, pWorld2, sizeof(WORLD2) * BoardNumMax);
	pWorld2Buffer_->Unlock();
	p = 0;

	pWorld3Buffer_->Lock(0, 0, &p, 0);
	memcpy(p, pWorld3, sizeof(WORLD3) * BoardNumMax);
	pWorld3Buffer_->Unlock();
	p = 0;

	pWorld4Buffer_->Lock(0, 0, &p, 0);
	memcpy(p, pWorld4, sizeof(WORLD4) * BoardNumMax);
	pWorld4Buffer_->Unlock();
	p = 0;

	hr = pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&this->indexbuf_,
		NULL
	);
	if (FAILED(hr))
	{
		MessageBox(NULL, "インデックスバッファが作れませんでした。", "エラー", MB_OK);
		PostQuitMessage(0);
	}

	LPWORD pIndex;
	this->indexbuf_->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	WORD index[] =
	{
		0,3,2,
		2,1,0,
	};

	memcpy(&pIndex[0], &index[0], sizeof(WORD) * 6);
	this->indexbuf_->Unlock();

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

	LPD3DXBUFFER errBuf = 0;

	if (FAILED(hr = D3DXCreateEffectFromFile(pDevice, "resource/shader/instance.fx", NULL, NULL, 0, NULL, &seffect_, &errBuf)))
	{
		MessageBox(NULL, (LPCSTR)errBuf->GetBufferPointer(), "ERROR", MB_OK);
	}
	else
	{
		technique_ = seffect_->GetTechniqueByName("tech");
	}

	if (errBuf)
		errBuf->Release();

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> rand500(-1000.0f, 1000.0f);
	D3DXMatrixIdentity(&this->world_);
	for (int s = 0; s < BoardNumMax; s++)
	{
		D3DXMatrixIdentity(&this->mtx_position_[s]);
		D3DXMatrixIdentity(&this->mtx_rotation_[s]);
		D3DXMatrixIdentity(&this->mtx_scale_[s]);
	}
	MeshFiled* meshfiled;
	meshfiled = CModeGame::GetFiled();

	D3DXVECTOR3 boardpos;
	float y;
	for (int z = 0; z < 30; z++)
	{
		for (int x = 0; x < 30; x++)
		{
			do
			{
				boardpos = D3DXVECTOR3(float(rand500(mt)), 0.0f, float(rand500(mt)));
				y = meshfiled->GetHeight(boardpos);
			} while (y > -50.0f);
			D3DXMatrixTranslation(&this->mtx_position_[z * 30 + x], boardpos.x, y + 1.0f, boardpos.z);
		}
	}
}

void CBillBoard::Uninit()
{
	delete pWorld1;
	delete pWorld2;
	delete pWorld3;
	delete pWorld4;

	if (this->seffect_ != NULL)
	{
		this->seffect_->Release();
		this->seffect_ = NULL;
		delete this->seffect_;
	}
	if (this->pDecl_ != NULL)
	{
		this->pDecl_->Release();
		this->pDecl_ = NULL;
		delete this->pDecl_;
	}
	if (this->pWorld1Buffer_ != NULL)
	{
		pWorld1Buffer_->Release();
		pWorld1Buffer_ = NULL;
		delete this->pWorld1Buffer_;
	}
	if (this->pWorld2Buffer_ != NULL)
	{
		pWorld2Buffer_->Release();
		pWorld2Buffer_ = NULL;
		delete this->pWorld2Buffer_;
	}
	if (this->pWorld3Buffer_ != NULL)
	{
		pWorld3Buffer_->Release();
		pWorld3Buffer_ = NULL;
		delete this->pWorld3Buffer_;
	}
	if (this->pWorld4Buffer_ != NULL)
	{
		pWorld4Buffer_->Release();
		pWorld4Buffer_ = NULL;
		delete this->pWorld4Buffer_;
	}
	if (this->vertexbuf_ != NULL)
	{
		this->vertexbuf_->Release();
		this->vertexbuf_ = NULL;
		delete this->vertexbuf_;
	}
	if (this->indexbuf_ != NULL)
	{
		this->indexbuf_->Release();
		this->indexbuf_ = NULL;
		delete this->indexbuf_;
	}
	for (int i = 0; i < BILLBOARD_NUM_MAX; i++)
	{
		if (this->texture_ != NULL)
		{
			this->texture_[i]->Release();
			this->texture_[i] = NULL;
			delete this->texture_[i];
		}
	}
}

void CBillBoard::Update()
{
}

void CBillBoard::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (UINT)BoardNumMax); //ここだけ板ポリゴンの枚数
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | (UINT)1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | (UINT)1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | (UINT)1);
	pDevice->SetStreamSourceFreq(4, D3DSTREAMSOURCE_INSTANCEDATA | (UINT)1);

	pDevice->SetVertexDeclaration(pDecl_);

	pDevice->SetStreamSource(0, vertexbuf_, 0, sizeof(BillbaordSet));
	pDevice->SetStreamSource(1, pWorld1Buffer_, 0, sizeof(WORLD1));
	pDevice->SetStreamSource(2, pWorld2Buffer_, 0, sizeof(WORLD2));
	pDevice->SetStreamSource(3, pWorld3Buffer_, 0, sizeof(WORLD3));
	pDevice->SetStreamSource(4, pWorld4Buffer_, 0, sizeof(WORLD4));
	pDevice->SetIndices(indexbuf_);
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	seffect_->SetTechnique(technique_);
	for (int i = 0; i < BoardNumMax; i++)
	{
		seffect_->Begin(NULL, 0);
		seffect_->BeginPass(0);
		seffect_->SetMatrix("mPosition", &this->mtx_position_[i]);
		seffect_->SetMatrix("mScale", &this->mtx_scale_[i]);
		//逆行列を渡す。
		seffect_->SetMatrix("mRotation", &getTransposeview_);
		seffect_->SetMatrix("mView", &getview_);
		seffect_->SetMatrix("mProj", &getproj_);
		seffect_->SetTexture("texDecal", texture_[0]);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
		seffect_->EndPass();
		seffect_->End();
		pDevice->SetStreamSourceFreq(0, 1);
		pDevice->SetStreamSourceFreq(1, 1);
		pDevice->SetStreamSourceFreq(2, 1);
		pDevice->SetStreamSourceFreq(3, 1);
		pDevice->SetStreamSourceFreq(4, 1);
	}
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//pDevice->SetRenderState(D3DRS_ZENABLE, false);
	//pDevice->SetRenderState(D3DRS_ZENABLE, false);

}

void CBillBoard::SetMtxView(D3DXMATRIX view)
{
	this->getTransposeview_ = view;
	this->getTransposeview_._41 = 0.0f;
	this->getTransposeview_._42 = 0.0f;
	this->getTransposeview_._43 = 0.0f;
	D3DXMatrixTranspose(&this->getTransposeview_, &this->getTransposeview_);
	this->getview_ = view;
}

void CBillBoard::SetMtxProj(D3DXMATRIX proj)
{
	this->getproj_ = proj;
}

CBillBoard * CBillBoard::Create(std::string filename/*, D3DXVECTOR3 pos, D3DXVECTOR3 scall, float rotation*/)
{
	CBillBoard* billboard = new CBillBoard(1, filename/*, pos, scall, rotation*/);
	billboard->Init();

	return billboard;
}


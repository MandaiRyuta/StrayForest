#include "scene3d.h"

void CScene3D::Init()
{
	LPDIRECT3DDEVICE9 device = GetDevice();

	for (int count = 0; count < TEST_NUM_MAX; count++)
	{
		HRESULT hr;
		hr = D3DXCreateTextureFromFile
		(
			device,
			this->filename_.c_str(),
			&this->texture_[count]
		);

		if (FAILED(hr))
		{
			MessageBox(NULL, "テクスチャが読み込めません", "エラー", MB_OK);
			PostQuitMessage(0);
		}

		hr = device->CreateVertexBuffer(
			sizeof(VERTEX3D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX3D,
			D3DPOOL_MANAGED,
			&this->vertexbuf_,
			NULL
		);
		if (FAILED(hr))
		{
			MessageBox(NULL, "頂点バッファが作れませんでした", "エラー", MB_OK);
			PostQuitMessage(0);
		}

		VERTEX3D* pV;
		this->vertexbuf_->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

		VERTEX3D v[4] =
		{
			{{ 10.0f,0.0f, 0.0f}, {0.0f,1.0f,0.0f}, D3DCOLOR(D3DCOLOR_RGBA(255,255,255,255)), {0.0f,0.0f}},
			{{ 10.0f,0.0f,10.0f}, {0.0f,1.0f,0.0f}, D3DCOLOR(D3DCOLOR_RGBA(255,255,255,255)), {1.0f,0.0f}},
			{{-10.0f,0.0f,10.0f}, {0.0f,1.0f,0.0f}, D3DCOLOR(D3DCOLOR_RGBA(255,255,255,255)), {1.0f,1.0f}},
			{{-10.0f,0.0f, 0.0f}, {0.0f,1.0f,0.0f}, D3DCOLOR(D3DCOLOR_RGBA(255,255,255,255)), {1.0f,0.0f}},
		};

		memcpy(&pV[0], &v[0], sizeof(VERTEX3D) * 4);
		
		this->vertexbuf_->Unlock();

		hr = device->CreateIndexBuffer
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
	}
}

void CScene3D::Uninit()
{
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
	for (int i = 0; i < TEST_NUM_MAX; i++)
	{
		if (this->texture_ != NULL)
		{
			this->texture_[i]->Release();
			this->texture_[i] = NULL;
		}
	}
}

void CScene3D::Update()
{
	D3DXMatrixIdentity(&this->world_);
	D3DXMATRIX position;
	D3DXMatrixScaling(&this->world_, this->scall_.x, this->scall_.y, this->scall_.z);
	D3DXMatrixTranslation(&position, this->position_.x, this->position_.y, this->position_.z);
	D3DXMATRIX rot;

	D3DXMatrixRotationY(&rot, this->rotation_);

	this->world_ = position * rot * this->world_;
}

void CScene3D::Draw()
{	
	LPDIRECT3DDEVICE9 device = GetDevice();

	device->SetStreamSource(0, this->vertexbuf_, 0, sizeof(VERTEX3D));
	device->SetIndices(this->indexbuf_);

	device->SetFVF(FVF_VERTEX3D);
	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetTransform(D3DTS_WORLD, &this->world_);
	device->SetTexture(	0, this->texture_[0]);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	//device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

CScene3D * CScene3D::Create(std::string filename, D3DXVECTOR3 pos, D3DXVECTOR3 scall, float rotation)
{
	CScene3D* scene3D = new CScene3D( 1, filename, pos, scall, rotation);
	scene3D->Init();

	return scene3D;
}

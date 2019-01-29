#include "PlayerItem.h"
#include "player.h"

void Sword::Init(LPDIRECT3DDEVICE9 device)
{
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX(WEPONE.c_str(), D3DXMESH_SYSTEMMEM, device, NULL, &pD3DXMtrlBuffer, NULL, &Weapone_->dw_material_, &Weapone_->p_mesh_)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", WEPONE.c_str(), MB_OK);
		PostQuitMessage(0);
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	Weapone_->p_meshmaterial_ = new D3DMATERIAL9[Weapone_->dw_material_];
	Weapone_->p_meshtexture_ = new LPDIRECT3DTEXTURE9[Weapone_->dw_material_];

	for (DWORD i = 0; i < Weapone_->dw_material_; i++)
	{
		Weapone_->p_meshmaterial_[i] = d3dxMaterials[i].MatD3D;
		Weapone_->p_meshmaterial_[i].Ambient = Weapone_->p_meshmaterial_->Diffuse;
		Weapone_->p_meshtexture_[i] = NULL;

		if (d3dxMaterials[i].pTextureFilename != NULL &&strlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(device, d3dxMaterials[i].pTextureFilename, &Weapone_->p_meshtexture_[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				PostQuitMessage(0);
			}
		}
	}

	pD3DXMtrlBuffer->Release();
	////シェーダーを読み込む
	if (FAILED(D3DXCreateEffectFromFile(device, "resource/shader/modelshader.fx", NULL, NULL, 0, NULL, &effect_, NULL)))
	{
		MessageBox(NULL, "シェーダーファイル読み込み失敗", NULL, MB_OK);
		PostQuitMessage(0);
	}

	D3DXMatrixIdentity(&mtx_world_);
	D3DXMatrixIdentity(&mtx_rotation_);
	D3DXMatrixIdentity(&mtx_scale_);
	D3DXMatrixIdentity(&mtx_translation_);
}

void Sword::Update()
{
}

void Sword::Draw(LPDIRECT3DDEVICE9 device, D3DXMATRIX TargetBone)
{
	D3DXMatrixScaling(&mtx_scale_, Weapone_->scall_.x, Weapone_->scall_.y, Weapone_->scall_.z);
	D3DXMatrixTranslation(&mtx_translation_, Weapone_->position_.x, Weapone_->position_.y, Weapone_->position_.z);
	D3DXMatrixRotationYawPitchRoll(&mtx_rotation_, D3DXToRadian(90.0f), D3DXToRadian(45.0f), D3DXToRadian(-45.0f));
	mtx_world_ = mtx_scale_ * mtx_rotation_ * mtx_translation_ * TargetBone;


	device->SetTransform(D3DTS_WORLD, &mtx_world_);
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	device->SetRenderState(D3DRS_ZENABLE, true);

	effect_->SetTechnique("tecMinimum");
	effect_->SetMatrix("mWVP", (D3DXMATRIX*)&mtx_world_);
	effect_->Begin(NULL, 0);
	for (DWORD i = 0; i < Weapone_->dw_material_; i++)
	{
		device->SetTexture(0,Weapone_->p_meshtexture_[i]);
		Weapone_->p_mesh_->DrawSubset(i);
	}
	effect_->End();
}


void Sword::Uninit()
{
	if (effect_ != NULL)
	{
		effect_->Release();
		effect_ = NULL;
	}

	if (Weapone_->p_mesh_ != NULL)
	{
		Weapone_->p_mesh_->Release();
		Weapone_->p_mesh_ = NULL;
	}
	delete[] Weapone_->p_meshtexture_;
	delete[] Weapone_->p_meshmaterial_;
}

void Shild::Init(LPDIRECT3DDEVICE9 device)
{
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX(ARMOR.c_str(), D3DXMESH_SYSTEMMEM, device, NULL, &pD3DXMtrlBuffer, NULL, &Armor_->dw_material_, &Armor_->p_mesh_)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", WEPONE.c_str(), MB_OK);
		PostQuitMessage(0);
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	Armor_->p_meshmaterial_ = new D3DMATERIAL9[Armor_->dw_material_];
	Armor_->p_meshtexture_ = new LPDIRECT3DTEXTURE9[Armor_->dw_material_];

	for (DWORD i = 0; i < Armor_->dw_material_; i++)
	{
		Armor_->p_meshmaterial_[i] = d3dxMaterials[i].MatD3D;
		Armor_->p_meshmaterial_[i].Ambient = Armor_->p_meshmaterial_->Diffuse;
		Armor_->p_meshtexture_[i] = NULL;

		if (d3dxMaterials[i].pTextureFilename != NULL &&strlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(device, d3dxMaterials[i].pTextureFilename, &Armor_->p_meshtexture_[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				PostQuitMessage(0);
			}
		}
	}

	pD3DXMtrlBuffer->Release();
	////シェーダーを読み込む
	if (FAILED(D3DXCreateEffectFromFile(device, "resource/shader/modelshader.fx", NULL, NULL, 0, NULL, &effect_, NULL)))
	{
		MessageBox(NULL, "シェーダーファイル読み込み失敗", NULL, MB_OK);
		PostQuitMessage(0);
	}

	D3DXMatrixIdentity(&mtx_world_);
	D3DXMatrixIdentity(&mtx_rotation_);
	D3DXMatrixIdentity(&mtx_scale_);
	D3DXMatrixIdentity(&mtx_translation_);
}

void Shild::Update()
{
}

void Shild::Draw(LPDIRECT3DDEVICE9 device, D3DXMATRIX TargetBone)
{
	D3DXMatrixScaling(&mtx_scale_, Armor_->scall_.x, Armor_->scall_.y, Armor_->scall_.z);
	D3DXMatrixTranslation(&mtx_translation_, Armor_->position_.x + 5.0f, Armor_->position_.y - 10.0f, Armor_->position_.z + 15.0f);
	D3DXMatrixRotationYawPitchRoll(&mtx_rotation_, D3DXToRadian(0.0f), D3DXToRadian(0.0f), D3DXToRadian(180.0f));

	mtx_world_ = mtx_scale_ * mtx_rotation_ * mtx_translation_ * TargetBone;


	device->SetTransform(D3DTS_WORLD, &mtx_world_);
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	device->SetRenderState(D3DRS_ZENABLE, true);

	effect_->SetTechnique("tecMinimum");
	effect_->SetMatrix("mWVP", (D3DXMATRIX*)&mtx_world_);
	effect_->Begin(NULL, 0);
	for (DWORD i = 0; i < Armor_->dw_material_; i++)
	{
		device->SetTexture(0, Armor_->p_meshtexture_[i]);
		Armor_->p_mesh_->DrawSubset(i);
	}
	effect_->End();
}

void Shild::Uninit()
{
	if (effect_ != NULL)
	{
		effect_->Release();
		effect_ = NULL;
	}

	if (Armor_->p_mesh_ != NULL)
	{
		Armor_->p_mesh_->Release();
		Armor_->p_mesh_ = NULL;
	}
	delete[] Armor_->p_meshtexture_;
	delete[] Armor_->p_meshmaterial_;
}

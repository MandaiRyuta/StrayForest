#pragma once
#include "main.h"
#include <vector>

const std::string WEPONE = "resource/xmodel/sword.x";
const std::string ARMOR = "resource/xmodel/shiled.x";

class PlayerWeapon
{
public:
	virtual ~PlayerWeapon() {}
	virtual void Init(LPDIRECT3DDEVICE9 device) = 0;
	virtual void Update() = 0;
	virtual void Draw(LPDIRECT3DDEVICE9 device, D3DXMATRIX TargetBone) = 0;
	virtual void Uninit() = 0;
};

class Sword : public PlayerWeapon
{
public:
	Sword()
	{
		Weapone_ = new MATRIX3DMANAGER::Model;
		Weapone_->color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
		Weapone_->index_ = 0;
		Weapone_->position_ = { 0.0f,0.0f,0.0f };
		Weapone_->isscall_ = true;
		Weapone_->scall_ = { 25.f,25.f,25.f };
	}
	void Init(LPDIRECT3DDEVICE9 device) override;
	void Update() override;
	void Draw(LPDIRECT3DDEVICE9 device, D3DXMATRIX TargetBone) override;
	void Uninit() override;
private:
	MATRIX3DMANAGER::Model* Weapone_;
	D3DXMATRIX mtx_world_;
	D3DXMATRIX mtx_view_;
	D3DXMATRIX mtx_proj_;
	D3DXMATRIX mtx_translation_;
	D3DXMATRIX mtx_rotation_;
	D3DXMATRIX mtx_scale_;
	D3DXVECTOR3 position_;
	D3DXVECTOR3 scale_;
	D3DXVECTOR3 rotation_;

	LPDIRECT3DVERTEXBUFFER9 vertexbuffer_;
	LPDIRECT3DINDEXBUFFER9 indexbuffer_;

	LPD3DXEFFECT effect_;
};

class PlayerArmor
{
public:
	virtual ~PlayerArmor() {}
	virtual void Init(LPDIRECT3DDEVICE9 device) = 0;
	virtual void Update() = 0;
	virtual void Draw(LPDIRECT3DDEVICE9 device, D3DXMATRIX TargetBone) = 0;
	virtual void Uninit() = 0;
};
class Shild : public PlayerArmor
{
public:
	Shild()
	{
		Armor_ = new MATRIX3DMANAGER::Model;
		Armor_->color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
		Armor_->index_ = 0;
		Armor_->position_ = { 0.0f,0.0f,0.0f };
		Armor_->isscall_ = true;
		Armor_->scall_ = { 30.f,30.f,30.f };
	}
	void Init(LPDIRECT3DDEVICE9 device) override;
	void Update() override;
	void Draw(LPDIRECT3DDEVICE9 device, D3DXMATRIX TargetBone) override;
	void Uninit() override;
private:
	MATRIX3DMANAGER::Model* Armor_;
	D3DXMATRIX mtx_world_;
	D3DXMATRIX mtx_view_;
	D3DXMATRIX mtx_proj_;
	D3DXMATRIX mtx_translation_;
	D3DXMATRIX mtx_rotation_;
	D3DXMATRIX mtx_scale_;
	D3DXVECTOR3 position_;
	D3DXVECTOR3 scale_;
	D3DXVECTOR3 rotation_;

	LPDIRECT3DVERTEXBUFFER9 vertexbuffer_;
	LPDIRECT3DINDEXBUFFER9 indexbuffer_;

	LPD3DXEFFECT effect_;
};
//•Ší•–h‹ïŠÇ—ƒNƒ‰ƒX
//Initˆ—‚Ì‘O‚Éadd‚Å•Ší‚Æ‚‚ð’Ç‰Á‚µ
class ItemList
{
public:
	void add(PlayerWeapon* weapon, PlayerArmor* armor) {
		weapon_.push_back(weapon);
		armor_.push_back(armor);
	}
	void WeaponClear() {
		for (auto itr = weapon_.begin(); itr != weapon_.end();)
		{
			if (*itr != nullptr)
			{
				itr = weapon_.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
	void ArmorClear() {
		for (auto itr = armor_.begin(); itr != armor_.end();)
		{
			if (*itr != nullptr)
			{
				itr = armor_.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
	void Init(LPDIRECT3DDEVICE9 device) {
		for (auto itr = weapon_.begin(); itr != weapon_.end(); ++itr)
		{
			(*itr)->Init(device);
		}
		for (auto itr = armor_.begin(); itr != armor_.end(); ++itr)
		{
			(*itr)->Init(device);
		}
	}
	void Update() {
		for (auto itr = weapon_.begin(); itr != weapon_.end(); ++itr)
		{
			(*itr)->Update();
		}
		for (auto itr = armor_.begin(); itr != armor_.end(); ++itr)
		{
			(*itr)->Update();
		}
	}
	void WeaponDraw(LPDIRECT3DDEVICE9 device, D3DXMATRIX targetbone) {
		for (auto itr = weapon_.begin(); itr != weapon_.end(); ++itr)
		{
			(*itr)->Draw(device, targetbone);
		}
	}
	void ArmorDraw(LPDIRECT3DDEVICE9 device, D3DXMATRIX targetbone)
	{
		for (auto itr = armor_.begin(); itr != armor_.end(); ++itr)
		{
			(*itr)->Draw(device, targetbone);
		}
	}
	void Uninit() {
		WeaponClear();
		ArmorClear();
	}
private:
	std::vector<PlayerWeapon*> weapon_;
	std::vector<PlayerArmor*> armor_;
};
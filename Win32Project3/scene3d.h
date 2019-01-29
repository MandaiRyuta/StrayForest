#pragma once

#include <string>

#include "main.h"
#include "cscene.h"
#include "light.h"
#include "camera.h"

#define FVF_VERTEX3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

class CScene3D : public CScene
{
private:
	typedef enum
	{
		TEST_TEXTURE,
		TEST_NUM_MAX
	}NUMPOLYGON3D;
private:
	D3DXMATRIX world_;
	D3DXVECTOR3 position_;
	D3DXVECTOR3 scall_;
	float		rotation_;
	LPDIRECT3DVERTEXBUFFER9 vertexbuf_;
	LPDIRECT3DINDEXBUFFER9  indexbuf_;
	LPDIRECT3DTEXTURE9      texture_[TEST_NUM_MAX];
	std::string filename_;
public:
	CScene3D(int Priority ,std::string filename, D3DXVECTOR3 pos, D3DXVECTOR3 scall, float rotation) : filename_(filename),CScene( Priority )
	{
		this->position_ = pos;
		this->scall_ = scall;
		this->rotation_ = rotation;
	}
	~CScene3D(){}
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

public:
	static CScene3D* Create(std::string filename, D3DXVECTOR3 pos, D3DXVECTOR3 scall, float rotation);
};
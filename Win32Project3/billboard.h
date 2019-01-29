#pragma once
#include "main.h"
#include "cscene.h"

constexpr int BoardNumMax = 900;

struct WORLD1
{
	float m11, m12, m13, m14;
};
struct WORLD2
{
	float m21, m22, m23, m24;
};
struct WORLD3
{
	float m31, m32, m33, m34;
};
struct WORLD4
{
	float m41, m42, m43, m44;
};

struct BillbaordSet
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;
	D3DXVECTOR3 normal;
};
class CBillBoard : public CScene
{
private:
	typedef enum
	{
		BILLBOARD_TEXTURE,
		BILLBOARD_NUM_MAX
	}BILLBOARD_POLYGON3D;
private:
	IDirect3DVertexBuffer9* pWorld1Buffer_;
	IDirect3DVertexBuffer9* pWorld2Buffer_;
	IDirect3DVertexBuffer9* pWorld3Buffer_;
	IDirect3DVertexBuffer9* pWorld4Buffer_;
	IDirect3DVertexDeclaration9* pDecl_;
	LPD3DXEFFECT seffect_;
	WORLD1* pWorld1;
	WORLD2* pWorld2;
	WORLD3* pWorld3;
	WORLD4* pWorld4;

	D3DXHANDLE technique_;
	D3DXMATRIX world_;
	D3DXMATRIX getTransposeview_;
	D3DXMATRIX getview_;
	D3DXMATRIX getproj_;
	D3DXMATRIX mtx_position_[BoardNumMax];
	D3DXMATRIX mtx_rotation_[BoardNumMax];
	D3DXMATRIX mtx_scale_[BoardNumMax];
	D3DXVECTOR3 position_;
	D3DXVECTOR3 scall_;
	float		rotation_;
	LPDIRECT3DVERTEXBUFFER9 vertexbuf_;
	LPDIRECT3DINDEXBUFFER9  indexbuf_;
	LPDIRECT3DTEXTURE9      texture_[BILLBOARD_NUM_MAX];
	std::string filename_;
public:
	CBillBoard(int Priority, std::string filename/*, D3DXVECTOR3 pos, D3DXVECTOR3 scale, float rotation*/) : filename_(filename), CScene(Priority)
	{
		//this->position_ = pos;
		//this->scall_ = scall;
		//this->rotation_ = rotation;
		pWorld1 = NULL;
		pWorld2 = NULL;
		pWorld3 = NULL;
		pWorld4 = NULL;
	}
	~CBillBoard() {}
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetMtxView(D3DXMATRIX view) override;
	void SetMtxProj(D3DXMATRIX proj) override;
public:
	static CBillBoard* Create(std::string filename/*, D3DXVECTOR3 pos, D3DXVECTOR3 scale, float rotation*/);
};
#pragma once
#include "cscene.h"
#include "main.h"


constexpr int MAX_MODELNUM = 1000;
constexpr int MODELNUM = 600;
struct MODEL_WORLD1
{
	float m11, m12, m13, m14;
};
struct MODEL_WORLD2
{
	float m21, m22, m23, m24;
};
struct MODEL_WORLD3
{
	float m31, m32, m33, m34;
};
struct MODEL_WORLD4
{
	float m41, m42, m43, m44;
};

struct MODEL_INFOMATION
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 texcoord;
	D3DCOLOR color;
};
class CModelInstancing : public CScene
{
public:
	CModelInstancing(int Priority, MATRIX3DMANAGER::Model * model, std::string filename) : filename_(filename), model_(model),CScene(Priority) {
		pWorld1 = NULL;
		pWorld2 = NULL;
		pWorld3 = NULL;
		pWorld4 = NULL;
	}
	~CModelInstancing() {}
public:
	void Init() override;
	void Uninit() override;
	void Draw() override;
	void Update() override;

	void SetMtxView(D3DXMATRIX view) override;
	void SetMtxProj(D3DXMATRIX proj) override;

	static CModelInstancing* Create(MATRIX3DMANAGER::Model * model,std::string filename);
	SphereInfo GetInstancingModelColision(int num);
private:
	MATRIX3DMANAGER::Model* model_;
	D3DXMATRIX mtxrotation[MAX_MODELNUM] = {};
	D3DXMATRIX mtxtranslasion[MAX_MODELNUM] = {};
	static D3DXMATRIX view_;
	static D3DXMATRIX proj_;
	std::string filename_;

	LPDIRECT3DVERTEXBUFFER9 vertexbuf_;
	LPDIRECT3DINDEXBUFFER9  indexbuf_;

	LPD3DXMESH mesh[MAX_MODELNUM];
	LPD3DXEFFECT effect_;
	D3DXHANDLE technique_;
	D3DXHANDLE	 shadowmap_;
	D3DXHANDLE	 srcmap_;
	D3DXMATRIX world_;
	D3DXMATRIX getTransposeview_;
	D3DXMATRIX getview_;
	D3DXMATRIX getproj_;
	D3DXMATRIX mtx_position_[MAX_MODELNUM];
	D3DXMATRIX mtx_rotation_[MAX_MODELNUM];
	D3DXMATRIX mtx_scale_[MAX_MODELNUM];
	D3DXVECTOR3 position_;
	D3DXVECTOR3 scall_;

	SphereInfo instancemodel[MODELNUM];

	MODEL_WORLD1* pWorld1;
	MODEL_WORLD2* pWorld2;
	MODEL_WORLD3* pWorld3;
	MODEL_WORLD4* pWorld4;
	LPDIRECT3DSURFACE9 pshadowmapz_;
	LPDIRECT3DTEXTURE9 pshadowmap_;
	LPDIRECT3DSURFACE9 pshadowmapsurf_;
	LPDIRECT3DTEXTURE9 pedgemap_;
	LPDIRECT3DSURFACE9 pedgemapsurf_;
	LPDIRECT3DTEXTURE9 psoftmap_[2];
	LPDIRECT3DSURFACE9 psoftmapsurf_[2];

	IDirect3DVertexBuffer9* pWorld1Buffer_;
	IDirect3DVertexBuffer9* pWorld2Buffer_;
	IDirect3DVertexBuffer9* pWorld3Buffer_;
	IDirect3DVertexBuffer9* pWorld4Buffer_;
	IDirect3DVertexDeclaration9* pDecl_;
};
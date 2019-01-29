#pragma once
#include "main.h"
#include "cscene.h"

#define FVF_MESHFILED3D ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 Normal;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}MESHFILED_VERTEX3D;

struct MESHFILED_INFOMATION {
	int filed_x;
	int filed_y;
	float size_x;
	float size_z;
};

class MeshFiled : public CScene {
public:
	MeshFiled(int Priority, std::string filename, MESHFILED_INFOMATION mesh_info) : filename_(filename), mesh_info_(mesh_info), CScene(Priority) {}
	~MeshFiled() {}
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;
	static MeshFiled* Create(MESHFILED_INFOMATION mesh_info, std::string filename);

	float GetHeight(D3DXVECTOR3 position);
	void GethWnd(HWND* hWnd);
	void SetMtxView(D3DXMATRIX view);
	void SetMtxProj(D3DXMATRIX proj);
private:
	HWND m_hWnd;
	std::string filename_;
	LPDIRECT3DVERTEXBUFFER9 meshfiled_vertexbuffer;
	LPDIRECT3DINDEXBUFFER9  meshfiled_indexbuffer;
	LPDIRECT3DTEXTURE9      meshfiled_texture;
	LPDIRECT3DTEXTURE9      hightmap_;
	MESHFILED_INFOMATION mesh_info_;
	LPD3DXEFFECT seffect_;
	D3DXHANDLE technique_;
	D3DMATERIAL9 MeshMaterial_;
	D3DXMATRIX world_;
	D3DXMATRIX view_;
	D3DXMATRIX proj_;
    static MESHFILED_VERTEX3D* mpv;
	int number_of_vertices_;
	int number_of_indices_;
	int number_of_primities_;

	int SetnX;
	int SetnZ;
//private:
	//LPDIRECT3DVERTEXDECLARATION9	m_pDecl;// 頂点宣言

	//// シェーダ
	//LPD3DXEFFECT		    m_pEffect;		// エフェクト
	//D3DXHANDLE				m_hTechnique;	// テクニック
	//D3DXHANDLE				m_hmWVP;		// ローカル-射影変換行列
	//D3DXHANDLE				m_hvLightDir;	// ライトの方向
	//D3DXHANDLE				m_hvColor;		// 頂点色
	//D3DXHANDLE				m_hvEyePos;		// 視点の位置
	//D3DXHANDLE				m_htDecaleTex;	// 模様のテクスチャ
	//D3DXHANDLE				m_htNormalMap;	// 法線マップ
};
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
	//LPDIRECT3DVERTEXDECLARATION9	m_pDecl;// ���_�錾

	//// �V�F�[�_
	//LPD3DXEFFECT		    m_pEffect;		// �G�t�F�N�g
	//D3DXHANDLE				m_hTechnique;	// �e�N�j�b�N
	//D3DXHANDLE				m_hmWVP;		// ���[�J��-�ˉe�ϊ��s��
	//D3DXHANDLE				m_hvLightDir;	// ���C�g�̕���
	//D3DXHANDLE				m_hvColor;		// ���_�F
	//D3DXHANDLE				m_hvEyePos;		// ���_�̈ʒu
	//D3DXHANDLE				m_htDecaleTex;	// �͗l�̃e�N�X�`��
	//D3DXHANDLE				m_htNormalMap;	// �@���}�b�v
};
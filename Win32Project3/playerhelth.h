#pragma once

#include "cscene.h"
#include <string>

class CPlayerHelth : public CScene
{
private:
	bool backgroundset_;
	LPDIRECT3DTEXTURE9 texture_[5];
	LPDIRECT3DVERTEXBUFFER9 vertexbuf_;

	POLYGONSIZE polygonsize_;

	std::string filename_[5];
	int helthstate_;
	int texturewidth_;
	int textureheight_;

	float scalex_, scaley_;
	float polygon_angle_;
	float rotation_offsetx_;
	float rotation_offsety_;
	float scale_offsetx_;
	float scale_offsety_;
public:
	CPlayerHelth(int Priority, POLYGONSIZE polygonsize, int texturewidth, int textureheight, bool backgroundset)
		: polygonsize_(polygonsize), backgroundset_(backgroundset), CScene(Priority)
	{
		this->vertexbuf_ = nullptr;
		for (int i = 0; i < 5; i++)
		{
			this->texture_[i] = nullptr;
		}
		this->polygon_angle_ = 0.0f;
		this->rotation_offsetx_ = 0.0f;
		this->rotation_offsety_ = 0.0f;
		this->scalex_ = 1.0f;
		this->scaley_ = 1.0f;
		this->scale_offsetx_ = 0.0f;
		this->scale_offsety_ = 0.0f;
		this->texturewidth_ = texturewidth;
		this->textureheight_ = textureheight;
	}
	~CPlayerHelth();

private:
	void CreateVertex(D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);
	void CreateVertexAffine(D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetPolygonScale(float scaleX, float scaleY);
	void PolygonWidthSet(float width);
	void helthstate(int i) override;
public:
	static CPlayerHelth* Create(POLYGONSIZE polygonsize, int texturewidth, int textureheight, bool backgroundset);
};
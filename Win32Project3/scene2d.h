#pragma once

#include "main.h"
#include "cscene.h"
#include <string>


class CScene2D : public CScene
{
private:
	bool backgroundset_;
	LPDIRECT3DTEXTURE9 texture_;
	LPDIRECT3DVERTEXBUFFER9 vertexbuf_;

	POLYGONSIZE polygonsize_;

	std::string filename_;

	int texturewidth_;
	int textureheight_;

	float scalex_, scaley_;
	float polygon_angle_;
	float rotation_offsetx_;
	float rotation_offsety_;
	float scale_offsetx_;
	float scale_offsety_;

	bool set_;
public:
	CScene2D(int Priority,bool set, POLYGONSIZE polygonsize,std::string filename, int texturewidth, int textureheight,bool backgroundset)
		: set_(set),polygonsize_(polygonsize), filename_(filename), backgroundset_(backgroundset), CScene(Priority)
	{
		this->vertexbuf_ = nullptr;
		this->texture_ = nullptr;
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
	~CScene2D();

private :
	void CreateVertex(D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);
	void CreateVertexAffine(D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ColorSet(D3DCOLOR setcolor);
	void SetPolygonScale(float scaleX, float scaleY);
	void PolygonDrawIn(bool Set);
	void PolygonWidthSet(float width);
public:
	static CScene2D* Create(bool set,POLYGONSIZE polygonsize, std::string filename, int texturewidth, int textureheight,bool backgroundset);
};
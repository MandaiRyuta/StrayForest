#pragma once
#include "main.h"
#include "cscene.h"

enum TEXTURENUM
{
	NUMBER
};
constexpr int TEXTURE_MAX = 1;
class Counter : public CScene
{
private:
	Counter(int Priority) : CScene(Priority) {}
	~Counter() {}
public:
	void Init() {}
	void Update() {}
	void Draw() {}
	void Uninit() {}

	// nÇ…ÇÕÇOÅ`ÇXÇ‹Ç≈ÇÃêîéö
	// Ç±ÇÃä÷êîÇÕêîéö1ï∂éöÇï\é¶
	void DrawNumber(int n, float x, float y);
	void DrawNumberScore(int n, float x, float y);
	// dig:ÉXÉRÉAÇÃåÖêî
	void DrawTimer(int nTime, int dig, bool bZero, float x, float y, int ver);
	bool InitPolygon(void);
	void UninitPolygon(void);
	void DrawPolygon(TEXTURENUM texNum, D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, bool bAffine);
	void CreateVertexAffine(int texNum, D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);
	void CreateVertex(int texNum, D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);
	void SetPolygonRotation(float cx, float cy, float fangle);
	void SetPolygonScale(float scaleX, float scaleY);
	static Counter* Create(int Priority);
private:
	float g_fPolygonAngle = 0.0f;
	float g_fRotationOffsetX = 0.0f;
	float g_fRotationOffsetY = 0.0f;
	float g_fScaleX = 1.0f;
	float g_fScaleY = 1.0f;
	float g_fScaleOFFsetX = 0.0f;
	float g_fScaleOFFsetY = 0.0f;
	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
	LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
	LPDIRECT3DTEXTURE9  g_pTextures[TEXTURE_MAX];
};
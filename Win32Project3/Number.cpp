#include "Number.h"
#include "scene2d.h"
#include "gamemanager.h"
#include <d3dx9.h>
#include <math.h>

typedef struct
{
	char fileName[256];
	int width;
	int height;
}TEXTURE;
static const TEXTURE g_pTexture[TEXTURE_MAX] = {
	{ "resource/texture/NumberSet.png",566,102 },
};

//********************************************************************************
//
// マクロ定義
//
//********************************************************************************
#define SCORE_INTERVAL	( 50 )
#define NUMBER_WIDTH    ( 75 )

//********************************************************************************
//
// 列挙型宣言
//
//********************************************************************************



//********************************************************************************
//
// 構造体宣言
//
//********************************************************************************



//********************************************************************************
//
// プロトタイプ宣言
//
//********************************************************************************



//********************************************************************************
//
// グローバル変数宣言
//
//********************************************************************************


//================================================================================
//
// 描画関数
//
//================================================================================

void Counter::DrawTimer(int nTime, int dig, bool bZero, float x, float y, int ver)
{
	if (ver == 1)
	{
		// 左詰め対応( こっちのが早い )
		if (dig <= 0)
		{
			dig = 1;

			for (; ; )
			{
				nTime /= 6;

				if (nTime == 0)
				{
					break;
				}

				dig++;
			}

			// 左詰め対応( 見やすさ重視 )
			//char buf[ 256 ];
			//sprintf( &buf[ 0 ], "%d", score );
			//dig = strlen( buf );
		}




		// カンスト判定
		int scoreMax = 6;

		for (int i = 1; i < dig; i++)
		{
			scoreMax *= 6;
		}

		scoreMax--;

		nTime = min(scoreMax, nTime);

		//数値の分解表示
		for (int i = dig - 1; i >= 0; i--)
		{
			int n = nTime % 6;
			nTime /= 6;

			DrawNumber(n, x + (NUMBER_WIDTH - SCORE_INTERVAL) * i, y);

			bool isZero = !bZero && !nTime;
			if (isZero)
			{
				i = 0;
			}
		}
	}
	else if (ver == 2)
	{
		// 左詰め対応( こっちのが早い )
		if (dig <= 0)
		{
			dig = 1;

			for (; ; )
			{
				nTime /= 6;

				if (nTime == 0)
				{
					break;
				}

				dig++;
			}

			// 左詰め対応( 見やすさ重視 )
			//char buf[ 256 ];
			//sprintf( &buf[ 0 ], "%d", score );
			//dig = strlen( buf );
		}




		// カンスト判定
		int scoreMax = 6;

		for (int i = 1; i < dig; i++)
		{
			scoreMax *= 6;
		}

		scoreMax--;

		nTime = min(scoreMax, nTime);

		//数値の分解表示
		for (int i = dig - 1; i >= 0; i--)
		{
			int n = nTime % 6;
			nTime /= 6;

			DrawNumberScore(n, x + (NUMBER_WIDTH - SCORE_INTERVAL) * i, y);

			bool isZero = !bZero && !nTime;
			if (isZero)
			{
				i = 0;
			}
		}
	}
	if (ver == 3)
	{
		// 左詰め対応( こっちのが早い )
		if (dig <= 0)
		{
			dig = 1;

			for (; ; )
			{
				nTime /= 6;

				if (nTime == 0)
				{
					break;
				}

				dig++;
			}

			// 左詰め対応( 見やすさ重視 )
			//char buf[ 256 ];
			//sprintf( &buf[ 0 ], "%d", score );
			//dig = strlen( buf );
		}




		// カンスト判定
		int scoreMax = 6;

		for (int i = 1; i < dig; i++)
		{
			scoreMax *= 6;
		}

		scoreMax--;

		nTime = min(scoreMax, nTime);

		//数値の分解表示
		for (int i = dig - 1; i >= 0; i--)
		{
			int n = nTime % 6;
			nTime /= 6;

			DrawNumberScore(n, x + (NUMBER_WIDTH - SCORE_INTERVAL) * i, y);

			bool isZero = !bZero && !nTime;
			if (isZero)
			{
				i = 0;
			}
		}
	}
}

//================================================================================
//
// 描画関数
//
//================================================================================

void Counter::DrawNumber(int n, float x, float y)
{
	if (n < 0 || n > 9)
	{
		return;
	}

	SetPolygonScale(0.5f, 0.5f);
	DrawPolygon(TEXTURENUM::NUMBER, D3DCOLOR_RGBA(255, 255, 255, 255), x, y, 96.f, 96.f, 56 * n, 0, 57, 102, true);
}

void Counter::DrawNumberScore(int n, float x, float y)
{
	if (n < 0 || n > 9)
	{
		return;
	}

	SetPolygonScale(0.25f, 0.5f);
	DrawPolygon(TEXTURENUM::NUMBER, D3DCOLOR_RGBA(255, 255, 255, 255), x, y, 96, 96, 56 * n, 0, 57, 102, true);
}
bool Counter::InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}
	HRESULT hr;
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		hr = D3DXCreateTextureFromFile(pDevice,
			g_pTexture[i].fileName,//ファイル名
			&g_pTextures[i]);
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファが作れなかった", "ERROR", MB_OK);
	}
	return true;
}
void Counter::UninitPolygon(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_pTextures[i] != NULL)
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}
void Counter::DrawPolygon(TEXTURENUM texNum, D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, bool bAffine)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int tw = g_pTexture[texNum].width;
	int th = g_pTexture[texNum].height;
	float u0 = (float)tcx / tw;
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw;
	float v1 = (float)(tcy + tch) / th;
	///////////////////////////////////////////図１//////////////////////////////////////////////////
	//ポリゴンの描画処理
	VERTEX_2D vtx[] = {
		//									絶対に1
		//{ D3DXVECTOR4(dx - dw / 2 , dy - dh / 2, 1.0f, 1.0f), color,D3DXVECTOR2(u0,v0) },	//16進 0xffffffff
		//{ D3DXVECTOR4(dx + dw / 2 , dy - dh / 2, 1.0f, 1.0f), color,D3DXVECTOR2(u1,v0) },  //RGBA
		//{ D3DXVECTOR4(dx + dw / 2 , dy + dh / 2, 1.0f, 1.0f), color,D3DXVECTOR2(u1,v1) },
		//{ D3DXVECTOR4(dx - dw / 2 , dy + dh / 2, 1.0f, 1.0f), color,D3DXVECTOR2(u0,v1) },
		{ D3DXVECTOR4(dx, dy - dh / 2, 1.0f, 1.0f), color,D3DXVECTOR2(u0,v0) },	//16進 0xffffffff
		{ D3DXVECTOR4(dx + dw  , dy - dh / 2, 1.0f, 1.0f), color,D3DXVECTOR2(u1,v0) },  //RGBA
		{ D3DXVECTOR4(dx + dw  , dy + dh / 2, 1.0f, 1.0f), color,D3DXVECTOR2(u1,v1) },
		{ D3DXVECTOR4(dx, dy + dh / 2, 1.0f, 1.0f), color,D3DXVECTOR2(u0,v1) },
	};
	if (bAffine)
	{
		CreateVertexAffine(texNum, color, dx, dy, dw, dh, tcx, tcy, tcw, tch);
	}
	else
	{
		CreateVertex(texNum, color, dx, dy, dw, dh, tcx, tcy, tcw, tch);
	}

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX_2D));


	//	FVF	(今から使用する頂点情報)
	pDevice->SetFVF(FVF_VERTEX_2D);
	//ライトON
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
	//テクスチャ貼り込む
	pDevice->SetTexture(0, g_pTextures[texNum]);


	//ポリゴンを描いて
	//DrawPrimitiveUP 遅い、簡単
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLEFAN, 0,
		2
	);	//ポリゴンの数

}

void Counter::CreateVertex(int texNum, D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch)
{
	VERTEX_2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
	pV[0].pos = D3DXVECTOR4(dx, dy - dh / 2, 0.5f, 1.0f);
	pV[1].pos = D3DXVECTOR4(dx + dw / 2, dy - dh / 2, 0.5f, 1.0f);
	pV[2].pos = D3DXVECTOR4(dx + dw / 2, dy + dh / 2, 0.5f, 1.0f);
	pV[3].pos = D3DXVECTOR4(dx, dy + dh / 2, 0.5f, 1.0f);

	pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = color;


	int tw = g_pTexture[texNum].width;
	int th = g_pTexture[texNum].height;
	float u0 = (float)tcx / tw;
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw;
	float v1 = (float)(tcy + tch) / th;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u1, v1);
	pV[3].texcoord = D3DXVECTOR2(u0, v1);

	g_pVertexBuffer->Unlock();

}

void Counter::CreateVertexAffine(int texNum, D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch)
{
	VERTEX_2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	//g_fScaleOFFsetX g_fRotationOffsetX

	//////////////////////////拡大////////////////////////////////	
	float x0 = (0.0f - g_fScaleOFFsetX) * g_fScaleX + g_fScaleX;
	float x1 = (dw - g_fScaleOFFsetX) * g_fScaleX + g_fScaleX;
	float y0 = (0.0f - g_fScaleOFFsetY) * g_fScaleY + g_fScaleY;
	float y1 = (dh - g_fScaleOFFsetY) * g_fScaleY + g_fScaleY;

	float fRotationOffsetX = g_fRotationOffsetX*g_fScaleX;
	float fRotationOffsetY = g_fRotationOffsetY*g_fScaleY;

	x0 -= fRotationOffsetX;
	x1 -= fRotationOffsetX;
	y0 -= fRotationOffsetY;
	y1 -= fRotationOffsetY;

	dx += fRotationOffsetX;
	dy += fRotationOffsetX;

	float x_x0 = dx + x0 *cosf(g_fPolygonAngle/* * D3DX_PI / 180*/) - y0 * sinf(g_fPolygonAngle/* * D3DX_PI / 180*/);
	float y_y0 = dy + x0 *sinf(g_fPolygonAngle/* * D3DX_PI / 180*/) + y0 * cosf(g_fPolygonAngle/* * D3DX_PI / 180*/);
	float x_x1 = dx + x1 *cosf(g_fPolygonAngle/* * D3DX_PI / 180*/) - y0 * sinf(g_fPolygonAngle/* * D3DX_PI / 180*/);
	float y_y1 = dy + x1 *sinf(g_fPolygonAngle/* * D3DX_PI / 180*/) + y0 * cosf(g_fPolygonAngle/* * D3DX_PI / 180*/);
	float x_x2 = dx + x1 *cosf(g_fPolygonAngle/* * D3DX_PI / 180*/) - y1 * sinf(g_fPolygonAngle/* * D3DX_PI / 180*/);
	float y_y2 = dy + x1 *sinf(g_fPolygonAngle/* * D3DX_PI / 180*/) + y1 * cosf(g_fPolygonAngle/* * D3DX_PI / 180*/);
	float x_x3 = dx + x0 *cosf(g_fPolygonAngle/* * D3DX_PI / 180*/) - y1 * sinf(g_fPolygonAngle/* * D3DX_PI / 180*/);
	float y_y3 = dy + x0 *sinf(g_fPolygonAngle/* * D3DX_PI / 180*/) + y1 * cosf(g_fPolygonAngle/* * D3DX_PI / 180*/);


	pV[0].pos = D3DXVECTOR4(dx + x_x0, dy + y_y0, 0.5f, 1.0f);//x´=xcos(α)-ysin(α);y´=xsin(α)+ycos(α);
	pV[1].pos = D3DXVECTOR4(dx + x_x1, dy + y_y1, 0.5f, 1.0f);
	pV[2].pos = D3DXVECTOR4(dx + x_x2, dy + y_y2, 0.5f, 1.0f);
	pV[3].pos = D3DXVECTOR4(dx + x_x3, dy + y_y3, 0.5f, 1.0f);

	pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = color;


	int tw = g_pTexture[texNum].width;
	int th = g_pTexture[texNum].height;

	float u0 = (float)tcx / g_pTexture[texNum].width;
	float v0 = (float)tcy / g_pTexture[texNum].height;
	float u1 = (float)(tcx + tcw) / g_pTexture[texNum].width;
	float v1 = (float)(tcy + tch) / g_pTexture[texNum].height;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u1, v1);
	pV[3].texcoord = D3DXVECTOR2(u0, v1);


	g_pVertexBuffer->Unlock();

}
void Counter::SetPolygonRotation(float cx, float cy, float fangle)
{
	g_fPolygonAngle = fangle;
	g_fRotationOffsetX = cx;
	g_fRotationOffsetY = cy;
}

void Counter::SetPolygonScale(float scaleX, float scaleY)
{
	g_fScaleX = scaleX;
	g_fScaleY = scaleY;
}

Counter * Counter::Create(int Priority)
{
	Counter* CreateNumber = new Counter(Priority);
	CreateNumber->InitPolygon();
	return CreateNumber;
}

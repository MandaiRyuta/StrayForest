#include "scene2d.h"

CScene2D::~CScene2D()
{

}

void CScene2D::CreateVertex(D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch)
{
	VERTEX_2D* pV;
	this->vertexbuf_->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
	pV[0].pos = D3DXVECTOR4(dx, dy - dh / 2, 0.5f, 1.0f);
	pV[1].pos = D3DXVECTOR4(dx + dw / 2, dy - dh / 2, 0.5f, 1.0f);
	pV[2].pos = D3DXVECTOR4(dx + dw / 2, dy + dh / 2, 0.5f, 1.0f);
	pV[3].pos = D3DXVECTOR4(dx, dy + dh / 2, 0.5f, 1.0f);

	pV[0].color =
	pV[1].color =
	pV[2].color =
	pV[3].color = color;

	int tw = this->texturewidth_;
	int th = this->textureheight_;
	float u0 = (float)tcx / tw;
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw;
	float v1 = (float)(tcy + tch) / th;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u1, v1);
	pV[3].texcoord = D3DXVECTOR2(u0, v1);
}

void CScene2D::CreateVertexAffine(D3DCOLOR color, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch)
{
	VERTEX_2D* pV;
	this->vertexbuf_->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	//////////////////////////拡大////////////////////////////////
	float x0 = (0.0f - this->scale_offsetx_) * this->scalex_ + +this->scalex_;
	float x1 = (dw - this->scale_offsetx_) * this->scalex_ + +this->scalex_;
	float y0 = (0.0f - this->scale_offsety_) * this->scaley_ + this->scaley_;
	float y1 = (dh - this->scale_offsety_) * this->scaley_ + this->scaley_;

	float fRotationOffsetX = this->rotation_offsetx_ * this->scalex_;
	float fRotationOffsetY = this->rotation_offsety_ * this->scaley_;

	x0 -= fRotationOffsetX;
	x1 -= fRotationOffsetX;
	y0 -= fRotationOffsetY;
	y1 -= fRotationOffsetY;

	dx += fRotationOffsetX;
	dy += fRotationOffsetX;

	float x_x0 = dx + x0 *cosf(this->polygon_angle_/* * D3DX_PI / 180*/) - y0 * sinf(this->polygon_angle_/* * D3DX_PI / 180*/);
	float y_y0 = dy + x0 *sinf(this->polygon_angle_/* * D3DX_PI / 180*/) + y0 * cosf(this->polygon_angle_/* * D3DX_PI / 180*/);
	float x_x1 = dx + x1 *cosf(this->polygon_angle_/* * D3DX_PI / 180*/) - y0 * sinf(this->polygon_angle_/* * D3DX_PI / 180*/);
	float y_y1 = dy + x1 *sinf(this->polygon_angle_/* * D3DX_PI / 180*/) + y0 * cosf(this->polygon_angle_/* * D3DX_PI / 180*/);
	float x_x2 = dx + x1 *cosf(this->polygon_angle_/* * D3DX_PI / 180*/) - y1 * sinf(this->polygon_angle_/* * D3DX_PI / 180*/);
	float y_y2 = dy + x1 *sinf(this->polygon_angle_/* * D3DX_PI / 180*/) + y1 * cosf(this->polygon_angle_/* * D3DX_PI / 180*/);
	float x_x3 = dx + x0 *cosf(this->polygon_angle_/* * D3DX_PI / 180*/) - y1 * sinf(this->polygon_angle_/* * D3DX_PI / 180*/);
	float y_y3 = dy + x0 *sinf(this->polygon_angle_/* * D3DX_PI / 180*/) + y1 * cosf(this->polygon_angle_/* * D3DX_PI / 180*/);


	pV[0].pos = D3DXVECTOR4(dx + x_x0, dy + y_y0, 0.5f, 1.0f);//x´=xcos(α)-ysin(α);y´=xsin(α)+ycos(α);
	pV[1].pos = D3DXVECTOR4(dx + x_x1, dy + y_y1, 0.5f, 1.0f);
	pV[2].pos = D3DXVECTOR4(dx + x_x2, dy + y_y2, 0.5f, 1.0f);
	pV[3].pos = D3DXVECTOR4(dx + x_x3, dy + y_y3, 0.5f, 1.0f);

	pV[0].color =
	pV[1].color =
	pV[2].color =
	pV[3].color = color;

	float u0 = (float)tcx / this->texturewidth_;
	float v0 = (float)tcy / this->textureheight_;
	float u1 = (float)(tcx + tcw) / this->texturewidth_;
	float v1 = (float)(tcy + tch) / this->textureheight_;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u1, v1);
	pV[3].texcoord = D3DXVECTOR2(u0, v1);

	this->vertexbuf_->Unlock();
}

void CScene2D::SetPolygonScale(float scaleX, float scaleY)
{
	this->scalex_ = scaleX;
	this->scaley_ = scaleY;
}

void CScene2D::PolygonDrawIn(bool Set)
{
	this->set_ = Set;
}

void CScene2D::PolygonWidthSet(float width)
{
	if (width >= 0.0f)
	{
		this->polygonsize_.dw_ = width;
	}
}

CScene2D * CScene2D::Create(bool set,POLYGONSIZE polygonsize, std::string filename, int texturewidth, int textureheight,bool backgroundset)
{
	CScene2D* scene2D = new CScene2D( 2, set, polygonsize, filename, texturewidth, textureheight ,backgroundset);
	scene2D->Init();
	return scene2D;
}

void CScene2D::Init()
{	
	LPDIRECT3DDEVICE9 device = GetDevice();
	HRESULT hr;
	//このせいです。
	if (backgroundset_)
	{
		this->polygonsize_ = SetPolygonsize();
	}

	//TextureReading
	//for (int i = 0; i < TEXTURE_MAX; i++)
	//{
		hr = D3DXCreateTextureFromFile(device, this->filename_.c_str() , &this->texture_);
	//}

		hr = device->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &this->vertexbuf_, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファが作れなかった", "ERROR", MB_OK);
	}
}
void CScene2D::Uninit()
{
	if (this->texture_ != nullptr)
	{
		this->texture_->Release();
		this->texture_ = nullptr;
		delete this->texture_;
	}
	if (this->vertexbuf_ != nullptr)
	{
		this->vertexbuf_->Release();
		this->vertexbuf_ = nullptr;
		delete this->vertexbuf_;
	}
}

void CScene2D::Update()
{
}

void CScene2D::Draw()
{
	if (this->set_)
	{
		LPDIRECT3DDEVICE9 device = GetDevice();
		int tw = this->texturewidth_;
		int th = this->textureheight_;
		float u0 = (float)this->polygonsize_.tcx_ / tw;
		float v0 = (float)this->polygonsize_.tcy_ / th;
		float u1 = (float)(this->polygonsize_.tcx_ + this->polygonsize_.tcw_) / tw;
		float v1 = (float)(this->polygonsize_.tcy_ + this->polygonsize_.tch_) / th;
		
		///////////////////////////////////////////図１//////////////////////////////////////////////////
		//ポリゴンの描画処理
		VERTEX_2D vtx[] = {
			{ D3DXVECTOR4(this->polygonsize_.dx_, this->polygonsize_.dy_ - this->polygonsize_.dh_ / 2, 1.0f, 1.0f), this->polygonsize_.color_,D3DXVECTOR2(u0,v0) },	//16進 0xffffffff
			{ D3DXVECTOR4(this->polygonsize_.dx_ + this->polygonsize_.dw_  , this->polygonsize_.dy_ - this->polygonsize_.dh_ / 2, 1.0f, 1.0f), this->polygonsize_.color_,D3DXVECTOR2(u1,v0) },  //RGBA
			{ D3DXVECTOR4(this->polygonsize_.dx_ + this->polygonsize_.dw_  , this->polygonsize_.dy_ + this->polygonsize_.dh_ / 2, 1.0f, 1.0f), this->polygonsize_.color_,D3DXVECTOR2(u1,v1) },
			{ D3DXVECTOR4(this->polygonsize_.dx_, this->polygonsize_.dy_ + this->polygonsize_.dh_ / 2, 1.0f, 1.0f), this->polygonsize_.color_,D3DXVECTOR2(u0,v1) },
		};

		if (this->polygonsize_.affine_)
		{
			CreateVertexAffine(this->polygonsize_.color_, this->polygonsize_.dx_, this->polygonsize_.dy_, this->polygonsize_.dw_, this->polygonsize_.dh_, this->polygonsize_.tcx_, this->polygonsize_.tcy_, this->polygonsize_.tcw_, this->polygonsize_.tch_);
		}
		else
		{
			CreateVertex(this->polygonsize_.color_, this->polygonsize_.dx_, this->polygonsize_.dy_, this->polygonsize_.dw_, this->polygonsize_.dh_, this->polygonsize_.tcx_, this->polygonsize_.tcy_, this->polygonsize_.tcw_, this->polygonsize_.tch_);
		}

		device->SetStreamSource(0, this->vertexbuf_, 0, sizeof(VERTEX_2D));

		//	FVF	(今から使用する頂点情報)
		device->SetFVF(FVF_VERTEX_2D);
		//ライトON
		device->SetRenderState(D3DRS_LIGHTING, false);
		//ノーマライズ設定
		device->SetRenderState(D3DRS_NORMALIZENORMALS, false);
		//テクスチャ貼り込む
		device->SetTexture(0, this->texture_);

		//ポリゴンを描いて
		//DrawPrimitiveUP 遅い、簡単
		device->DrawPrimitive(
			D3DPT_TRIANGLEFAN, 0,
			2
		);	//ポリゴンの数
	}
	else
	{

	}
}

void CScene2D::ColorSet(D3DCOLOR setcolor)
{
	polygonsize_.color_ = setcolor;
}


#pragma once
#include "main.h"

class CCamera
{
private:
	// ビュー変換行列( 4×4のfloat行列 )
	D3DXMATRIX matrix_view;

	D3DXVECTOR3 offset;

	// プロジェクション変換行列( 4×4のfloat行列 )
	D3DXMATRIX matrix_projection;

	//// カメラ座標
	D3DXVECTOR3 eye_;

	//// 視点座標
	D3DXVECTOR3 at_;

	//// カメラの上方向
	D3DXVECTOR3 up_;
	float rotation_;

	static D3DXMATRIX m_GetProj_;
	static D3DXMATRIX m_GetView_;
public:
	CCamera(D3DXVECTOR3 at, D3DXVECTOR3 eye, D3DXVECTOR3 up)
		: at_(at), eye_(eye), up_(up)
	{ 
		offset = D3DXVECTOR3(0.0f, 7.0f, -15.0f);
		rotation_ = 0.0f;
	}
	~CCamera();
public:
	void CameraInit() {}
	void CameraUpdate();
	void CameraDraw() {}
	void CameraUninit() {};
public:
	static D3DXMATRIX SetView();

	static D3DXMATRIX SetProj();
};


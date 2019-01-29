#pragma once
#include "main.h"

class CCamera
{
private:
	// �r���[�ϊ��s��( 4�~4��float�s�� )
	D3DXMATRIX matrix_view;

	D3DXVECTOR3 offset;

	// �v���W�F�N�V�����ϊ��s��( 4�~4��float�s�� )
	D3DXMATRIX matrix_projection;

	//// �J�������W
	D3DXVECTOR3 eye_;

	//// ���_���W
	D3DXVECTOR3 at_;

	//// �J�����̏����
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


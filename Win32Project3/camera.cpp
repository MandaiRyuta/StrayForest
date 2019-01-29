#include "camera.h"
#include "main.h"
#include "input.h"
#include "gamemanager.h"
#include "CModeGame.h"

/*
�J�����ɋ��̔�����������ăv���C���[�����̋�����o�����ɂȂ�����A�J������Ǐ]������悤�ɂ���B
*/
D3DXMATRIX CCamera::m_GetView_;
D3DXMATRIX CCamera::m_GetProj_;

CCamera::~CCamera()
{
}

void CCamera::CameraUpdate()
{
	constexpr float SPEED = 0.1f;
	LPDIRECT3DDEVICE9 device = GetDevice();
	CInputKeyboard* pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	CPlayer* player;
	player = CModeGame::GetPlayer();
	
	D3DXMATRIX CameraRotation;
	D3DXMatrixIdentity(&CameraRotation);
	
	if (pInputKeyboard->GetKeyPress(DIK_L))
	{
		rotation_ = D3DXToRadian(0.78f) * D3DX_PI;
		D3DXMatrixRotationY(&CameraRotation, rotation_);
	}
	if (pInputKeyboard->GetKeyPress(DIK_J))
	{
		rotation_ = D3DXToRadian(-0.78f) * D3DX_PI;
		D3DXMatrixRotationY(&CameraRotation, rotation_);
	}

	D3DXVec3TransformCoord(&offset, &offset, &CameraRotation);
	D3DXVec3TransformCoord(&eye_, &eye_, &CameraRotation);
	D3DXVec3TransformCoord(&at_, &at_, &CameraRotation);
	eye_ = offset + player->PlayertoCamera(4);

	at_ = player->PlayertoCamera(4);

	//�r���[�s��̍쐬
	D3DXMatrixLookAtLH(
		&this->matrix_view,
		&this->eye_,
		&this->at_,
		&this->up_
	);

	//�v���W�F�N�V�����s��̍쐬
	D3DXMatrixPerspectiveFovLH(&this->matrix_projection, D3DX_PI / 3 /*D3DXToRadian(60),*/, (float)windows_rect::SCREEN_WIDTH / windows_rect::SCREEN_HEIGHT, 0.1f, 4000.0f);

	//�e��ލs��̐ݒ�
	device->SetTransform(D3DTS_VIEW, &this->matrix_view);
	device->SetTransform(D3DTS_PROJECTION, &this->matrix_projection);

	this->m_GetView_ = this->matrix_view;
	this->m_GetProj_ = this->matrix_projection;
}

D3DXMATRIX CCamera::SetView()
{
	return m_GetView_;
}

D3DXMATRIX CCamera::SetProj()
{
	return m_GetProj_;
}

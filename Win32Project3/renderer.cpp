#include "renderer.h"

LPDIRECT3D9 CRendere::pD3D_;
LPDIRECT3DDEVICE9 CRendere::device_;
bool CRendere::Init(HWND hWnd,bool bWindow, int nWindowWidth, int nWindowHeight)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D_ == NULL)
	{
		MessageBox(hWnd,"Direct3D�C���^�[�t�F�[�X�̎擾�Ɏ��s���܂����B",NULL,MB_OK);
		return false;
	}

	D3DDISPLAYMODE d3ddm;

	if (FAILED(pD3D_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(hWnd, "�f�B�X�v���C���[�h�̎擾�Ɏ��s���܂����B", NULL, MB_OK);
		return false;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^(�f�o�C�X���쐬���邽�߂̃f�[�^)
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));							//�@���̂��̂�S�Ă�����������(����������擪�A�h���X, ����������T�C�Y)
	d3dpp.BackBufferWidth = nWindowWidth;				//�@�����(�o�b�N�o�b�t�@)�̕�
	d3dpp.BackBufferHeight = nWindowHeight;				//	�����(�o�b�N�o�b�t�@)�̍���
	d3dpp.BackBufferFormat = d3ddm.Format;				//	�F��
	d3dpp.BackBufferCount = 1;							//	�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//	�f���M���̓���(DISCARD�͐����������C�ɂ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						//	TRUE�ŗǂ�
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//	���͂���ŗǂ���̃t�H�[�}�b�g
	d3dpp.Windowed = bWindow;						//	TRUE�ŃE�B���h�E���[�h,FALSE�Ńt���X�N���[�����[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//	�t���X�N���[�����[�h�̎��̃��t���b�V�����[�g��Hz����ύX
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//	���j�^�[��Hz���ɍ��킹�ĕ`�悳���(IMMEDIATE���Ɛ���������҂����ɕ`�悷��)

	if (FAILED(pD3D_->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&device_
	)))
	{
		MessageBox(hWnd, "Direct3DDevice�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	device_->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device_->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device_->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device_->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	device_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device_->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return true;
}
void CRendere::Uninit()
{
	if (pD3D_ != NULL)
	{
		pD3D_->Release();
		pD3D_ = NULL;
	}
	if (device_ != NULL)
	{
		device_->Release();
		device_ = NULL;
	}
}

bool CRendere::DrawBegin()
{
	return SUCCEEDED(device_->BeginScene());
}

void CRendere::DrawEnd()
{
	device_->EndScene();
}

LPDIRECT3DDEVICE9 CRendere::GetDevice()
{
	return device_;
}

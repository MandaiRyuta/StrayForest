#include "cscene.h"

CScene *CScene::m_Scene[3][200] = { NULL };
int CScene::PauseState = -1;
CScene::CScene(int Priority/*,SCENE_TYPE Scene_Type*/)
{
	for (int i = 0; i < 200; i++)
	{
		if (m_Scene[Priority][i] == NULL)
		{
			m_Scene[Priority][i] = this;
			//m_Type = Scene_Type;
			break;
		}
	}
}

//SCENE_TYPE CScene::GetType()
//{
//	return m_Type;
//}

void CScene::Release()
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 200; i++)
		{
			if (m_Scene[t][i] == this)
			{
				m_Scene[t][i]->Uninit();
				delete this;
				break;
			}
		}
	}
}

void CScene::UpdateAll()
{
	if (PauseState == -1)
	{
		for (int t = 0; t < 3; t++)
		{
			for (int i = 0; i < 200; i++)
			{
				if (m_Scene[t][i] != NULL)
				{
					m_Scene[t][i]->Update();
				}
			}
		}
	}
}

void CScene::DrawAll()
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 200; i++)
		{
			if (m_Scene[t][i] != NULL)
			{
				m_Scene[t][i]->Draw();
			}
		}
	}
}

void CScene::ReleaseAll()
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 200; i++)
		{
			if (m_Scene[t][i] != NULL)
			{
				m_Scene[t][i]->Release();
				m_Scene[t][i] = NULL;
			}
		}
	}
}

void CScene::SetMtxViewAll(D3DXMATRIX view)
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 200; i++)
		{
			if (m_Scene[t][i] != NULL)
			{			
				m_Scene[t][i]->SetMtxView(view);
			}
		}
	}
}

void CScene::SetMtxProjAll(D3DXMATRIX proj)
{
	for (int t = 0; t < 3; t++)
	{
		for (int i = 0; i < 200; i++)
		{
			if (m_Scene[t][i] != NULL)
			{
				m_Scene[t][i]->SetMtxProj(proj);
			}
		}
	}
}
void CScene::Pause()
{
	PauseState *= -1;
}
int CScene::PauseStateMode()
{
	return PauseState;
}
/*2D�|���S���T�C�Y�ݒ�*/
POLYGONSIZE CScene::SetPolygonsize()
{
	POLYGONSIZE polysize;

	polysize.dx_ = 0.f;
	polysize.dy_ = 0.f;
	polysize.dw_ = windows_rect::D3D_SCREEN_WIDTH;
	polysize.dh_ = windows_rect::D3D_SCREEN_HEIGHT;
	polysize.tcx_ = 0;
	polysize.tcy_ = 2;
	polysize.tcw_ = 184;
	polysize.tch_ = 500;
	polysize.color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
	polysize.affine_ = false;
	return polysize;
}

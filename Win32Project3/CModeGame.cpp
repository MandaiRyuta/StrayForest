#include "CModeGame.h"
#include "gamemanager.h"
#include "CModeResult.h"
#include "CModeGameOver.h"
#include <random>
constexpr float SkydomeRotationSpeed = 0.0005f;

MATRIX3DMANAGER::Model setmodel[7];
MATRIX3DMANAGER::Model setenemy[80];
MATRIX3DMANAGER::Model playermodel;
MATRIX3DMANAGER::Model skydomemodel;

Fade *CModeGame::SetFade;
CLight   *CModeGame::g_light;
CCamera  *CModeGame::g_camera;
MeshFiled *CModeGame::filed;

CPlayer *CModeGame::player;
//Counter *CModeGame::count;
CEnemy *CModeGame::enemy[80];
CModelInstancing *CModeGame::instancemodel;
//CModel *CModeGame::sword;
CPlayerHelth *CModeGame::player_helth[10];
CScene2D *CModeGame::Stamina[3];
CScene2D *CModeGame::Pause;
CBox *CModeGame::box;
CFirePlace *CModeGame::fireplace_;
void CModeGame::Init()
{
	/*
	m_Filed = CFiled::Create();
	m_Player = CPlayer::Create();
	*/
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> rand1000(-1000.0f, 1000.0f);
	std::uniform_int_distribution<> rand3(0, 3);
	std::uniform_int_distribution<> BoxRnad(0, 2);
	fadeCount = 0;
	MeshFiled* info = CModeGame::GetFiled();
	g_camera = new CCamera({ 0.0f,0.0f,0.0f }, { 0.0f,10.0f,-30.0f }, { 0.0f,1.0f,0.0f });
	g_camera->CameraInit();
	g_light = new CLight();
	g_light->light_Init();
	skydomemodel.position_ = { 0.f,0.f,0.f };
	skydomemodel.isscall_ = true;
	skydomemodel.scall_ = { 7.f,7.f,7.f };
	CSkydome::Create(&skydomemodel, XMODELFILEPASS::SKYDOME);
	MESHFILED_INFOMATION meshinfo;
	meshinfo.filed_x = MeshFiled_X;
	meshinfo.filed_y = MeshFiled_Z;
	meshinfo.size_x = MeshFiled_XSize;
	meshinfo.size_z = MeshFiled_ZSize;

	MeshFiled::Create(meshinfo, TEXTURERS::GROUND);

	POLYGONSIZE polygonsize = {};
	
	for (int i = 0; i < 10; i++)
	{
		polygonsize.dx_ = 15.f * i;
		polygonsize.dy_ = 0.0f;
		polygonsize.dw_ = 25.0f;
		polygonsize.dh_ = 25.0f;
		polygonsize.tcx_ = 0;
		polygonsize.tcy_ = 0;
		polygonsize.tcw_ = 500;
		polygonsize.tch_ = 500;
		polygonsize.color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
		polygonsize.affine_ = true;
		player_helth[i] = CPlayerHelth::Create(polygonsize, 500, 500, false);
	}
	polygonsize.dx_ = 0.0f;
	polygonsize.dy_ = 15.0f;
	polygonsize.dw_ = 350.0f;
	polygonsize.dh_ = 25.0f;
	polygonsize.tcx_ = 0;
	polygonsize.tcy_ = 0;
	polygonsize.tcw_ = 500;
	polygonsize.tch_ = 500;
	polygonsize.color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
	polygonsize.affine_ = true;
	Stamina[0] = CScene2D::Create(true,polygonsize, TEXTURERS::STAMINA_FUTI, 500, 500, false);
	polygonsize.dx_ = 1.0f;
	polygonsize.dy_ = 16.0f;
	polygonsize.dw_ = 345.0f;
	polygonsize.dh_ = 20.0f;
	Stamina[1] = CScene2D::Create(true,polygonsize, TEXTURERS::STAMINA_BAR2, 500, 500, false);
	Stamina[2] = CScene2D::Create(true,polygonsize, TEXTURERS::STAMINA_BAR, 500, 500, false);
	//polygonsize.dx_ = 350.f;
	//polygonsize.dy_ = 280.0f;
	//polygonsize.dw_ = 50.0f;
	//polygonsize.dh_ = 50.0f;
	//CScene2D::Create(true,polygonsize, TEXTURERS::RUPI, 500, 500, false);
	//polygonsize.dx_ = 350.f;
	//polygonsize.dy_ = 16.0f;
	//polygonsize.dw_ = 50.0f;
	//polygonsize.dh_ = 50.0f;
	//CScene2D::Create(true,polygonsize, TEXTURERS::BUTTONY , 500, 500, false);
	//CScene2D::Create(true,polygonsize, TEXTURERS::BOOMERANG, 500, 500, false);
	//polygonsize.dx_ = 350.f;
	//polygonsize.dy_ = 66.0f;
	//CScene2D::Create(true,polygonsize, TEXTURERS::BUTTONA, 500, 500, false);
	//polygonsize.dx_ = 325.f;
	//polygonsize.dy_ = 41.0f;
	//CScene2D::Create(true,polygonsize, TEXTURERS::BUTTONX, 500, 500, false);
	//polygonsize.dx_ = 375.f;
	//polygonsize.dy_ = 41.0f;
	//CScene2D::Create(true,polygonsize, TEXTURERS::BUTTONB, 500, 500, false);
	//CScene2D::Create(true,polygonsize, TEXTURERS::SWORD, 500, 500, false);

	//
	setmodel[0].position_ = D3DXVECTOR3(0.f, 0.5f, 0.f);
	setmodel[0].isscall_ = true;
	setmodel[0].scall_ = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	setmodel[0].color_ = D3DCOLOR(D3DCOLOR_RGBA(110, 52, 42, 255));
	setmodel[1].position_ = D3DXVECTOR3(0.0f, 40.0f, -20.f);
	setmodel[1].scall_ = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
	setmodel[1].isscall_ = true;
	setmodel[1].color_ = D3DCOLOR(D3DCOLOR_RGBA(255, 255, 255, 255));

	switch (BoxRnad(mt))
	{
	case 0:
		setmodel[2].position_ = D3DXVECTOR3(-376.962341f, 0.0f, 416.550659f);
		break;
	case 1:
		setmodel[2].position_ = D3DXVECTOR3(-630.455505f, -62.6358719f, 808.372437f);
		break;
	case 2:
		setmodel[2].position_ = D3DXVECTOR3(-349.596466f, -63.7974625f, -619.849854f);
		break;
	default:
		break;
	}
	//setmodel[2].position_ = D3DXVECTOR3(-890.7813110f, 0.0f, -890.072449f);
	setmodel[2].scall_ = D3DXVECTOR3(4.f, 4.f, 4.f);
	setmodel[2].isscall_ = true;
	setmodel[2].color_ = D3DCOLOR(D3DCOLOR_RGBA(255, 255, 255, 255));
	setmodel[3].position_ = D3DXVECTOR3(-376.962341f, 0.0f, 416.550659f);
	setmodel[3].scall_ = D3DXVECTOR3(2.f, 2.f, 2.f);
	setmodel[3].isscall_ = true;
	setmodel[3].color_ = D3DCOLOR(D3DCOLOR_RGBA(255, 255, 255, 255));
	setmodel[4].position_ = { -354.669983f,0.0f,316.243591f };
	setmodel[4].scall_ = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	setmodel[4].isscall_ = true;
	setmodel[4].color_ = D3DCOLOR(D3DCOLOR_RGBA(255, 255, 255, 255));
	setmodel[5].position_ = { -439.946655f,0.0f,259.815155f };
	setmodel[5].scall_ = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	setmodel[5].isscall_ = true;
	setmodel[5].color_ = D3DCOLOR(D3DCOLOR_RGBA(255, 255, 255, 255));
	//CModelInstancing::Create(&setmodel[0], XMODELFILEPASS::THREE);
	CBillBoard::Create(TEXTURERS::GRASS/*, { 0.0f ,0.f,0.f }, { 1.f,1.f,1.f }, 0.0f*/);
	instancemodel = CModelInstancing::Create(&setmodel[0], XMODELFILEPASS::THREE);
	//sword = CModel::Create(&setmodel[1], "resource/xmodel/sword.x");
	box = CBox::Create(&setmodel[2], "resource/xmodel/chest.x");
	CSign::Create(&setmodel[3], "resource/xmodel/Wooden_sign.x");
	CSign::Create(&setmodel[4], "resource/xmodel/Wooden_sign.x");
	CSign::Create(&setmodel[5], "resource/xmodel/Wooden_sign.x");
	setmodel[6].position_ = { -410.505341f,0.0f,307.974213f };
	setmodel[6].scall_ = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	setmodel[6].isscall_ = true;
	setmodel[6].color_ = D3DCOLOR(D3DCOLOR_RGBA(255, 255, 255, 255));
	fireplace_ = CFirePlace::Create(&setmodel[6], "resource/xmodel/fireplace.x");
	
	for (int i = 0; i < 80; i++)
	{
		//setenemy[i].position_ = { -890.7813110f, 0.0f, -901.072449f + i };
		do {
			setenemy[i].position_ = { (float)rand1000(mt),0.0f,(float)rand1000(mt) };
			setenemy[i].position_.y = info->GetHeight(setenemy[i].position_);
		} while (setenemy[i].position_.y > -55.f);
		setenemy[i].rotatiion_ = 0.2f;
		setenemy[i].isscall_ = true;
		setenemy[i].scall_ = { 2.0f,2.0f,2.0f };
		Area EnemyMoveArea;
		EnemyMoveArea.max = setenemy[i].position_ + D3DXVECTOR3(10.0f, 0.0f, 10.0f);
		EnemyMoveArea.min = setenemy[i].position_ + D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
		enemy[i] = CEnemy::Create(4,&setenemy[i], EnemyMoveArea, XMODELFILEPASS::ENEMY);
	}

	playermodel.position_ = { -890.7813110f,0.0f,-901.072449f };
	playermodel.isscall_ = false;
	playermodel.scall_ = { 3.f, 3.f, 3.f };
	player = CPlayer::Create(&playermodel, XMODELFILEPASS::Player);
	polygonsize.dx_ = 100.0f;
	polygonsize.dy_ = 75.0f;
	polygonsize.dw_ = 400 + 24;
	polygonsize.dh_ = 300 + 54;
	polygonsize.tch_ = 400;
	polygonsize.tcx_ = 0;
	polygonsize.tcy_ = 0;
	polygonsize.tcw_ = 1000;
	Pause = CScene2D::Create(true, polygonsize, TEXTURERS::PAUSE, 1000, 400, false);
	CManager::GetSound()->PlaySound(GAME_BGM);
	//count = Counter::Create(1);
	SetFade = Fade::Create();

	//Fog::Create(10, 20);
}

void CModeGame::Uninit()
{
	/*
	CScene::ReleaseAll();
	*/
	g_camera->CameraUninit();
	delete g_camera;
	
	g_light->light_Uninit();
	delete g_light;


	CScene::ReleaseAll();
}

void CModeGame::Update()
{
	//なし
	/*
	画面遷移用
	*/
	static float rot = 0;
	rot += SkydomeRotationSpeed;
	CSkydome::SkydomeSetRotation(rot);
	g_camera->CameraUpdate();
	CScene::SetMtxProjAll(CCamera::SetProj());
	CScene::SetMtxViewAll(CCamera::SetView());
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();
	if (player->IsPlayer())
	{
		fadeCount++;
		if (fadeCount < 255)
		{
			SetFade->FadeSetNumber(1);
		}
		else
		{
			CManager::SetMode(new CModeGameOver());
		}
	}
	if (player->GameClear())
	{
		fadeCount++;
		if (fadeCount < 255)
		{
			SetFade->FadeSetNumber(1);
		}
		else
		{
			CManager::SetMode(new CModeResult());
		}
	}
	if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))
	{
		CManager::GetSound()->PlaySound(BUTTON);
		CManager::SetMode(new CModeResult());
	}

	if (pInputKeyboard->GetKeyTrigger(DIK_P))
	{
		CScene::Pause();
		Pause->ColorSet(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
	if (CScene::PauseStateMode() == -1)
	{
		Pause->ColorSet(D3DCOLOR_RGBA(255, 255, 255, 0));
		//GameTimer++;
	}
}

void CModeGame::Draw()
{
	//count->DrawTimer(GameTimer, 5, true, 280.f, 0.f, 1);
	//DrawTimer(GameTimer, 5, true, 280.f, 0.f, 1);
	//なし
	/*
	得点表示やデバッグ表示
	*/
}

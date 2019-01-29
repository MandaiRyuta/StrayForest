#pragma once
#include <Windows.h>
#include <time.h>
#include <crtdbg.h>
#include <d3dx9.h>
#include <string>
//必要なライブラリファイルのロード
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#ifndef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#endif
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"XAudio2.lib")

namespace windows_rect
{
	constexpr int D3D_SCREEN_WIDTH = 1800;
	constexpr int D3D_SCREEN_HEIGHT = 1300;
	constexpr int SCREEN_WIDTH = 800;
	constexpr int SCREEN_HEIGHT = 600;
}

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

namespace TEXTURERS
{
	const std::string MISSILEICON = "resource/texture/MissileIcon.png";
	const std::string BACKGROUND = "resource/texture/hiru.png";
	const std::string CHUTORIAL = "resource/texture/chutorial.png";
	const std::string RESULTBACKGROUND = "resource/texture/yoru.png";
	const std::string GROUND = "resource/texture/floor.png";
	const std::string GRASS = "resource/texture/grass.png";
	const std::string TITLELOGO = "resource/texture/logo.png";
	const std::string PRESSSPACE = "resource/texture/space.png";
	const std::string GAMECLEAR = "resource/texture/clear.png";
	const std::string GAMEOVER = "resource/texture/over.png";
	const std::string PAUSE = "resource/texture/pause.png";
	const std::string Fade = "resource/texture/fade.png";
	
	const std::string HEART1 = "resource/texture/heart1.png";
	const std::string HEART2 = "resource/texture/heart2.png";
	const std::string HEART3 = "resource/texture/heart3.png";
	const std::string HEART4 = "resource/texture/heart4.png";
	const std::string HEART5 = "resource/texture/heart5.png";

	const std::string BOOMERANG = "resource/texture/boomerang.png";
	const std::string SWORD = "resource/texture/sword.png";

	const std::string STAMINA_BAR = "resource/texture/bar.png";
	const std::string STAMINA_BAR2 = "resource/texture/bar2.png";
	const std::string STAMINA_FUTI = "resource/texture/futi.png";
}	

namespace XMODELFILEPASS
{
	const std::string MAP = "resource/xmodel/Island.x";
	const std::string SKYDOME = "resource/xmodel/sky_dome_29.x";
	const std::string Player = "";
	const std::string THREE = "resource/xmodel/NewTree.x";
	const std::string LEAF = "resource/xmodel/leaf.x";
	const std::string ENEMY = "resource/xmodel/enemy.x";
}
namespace MATRIX3DMANAGER
{
	constexpr int XMODEL_MAX = 1000;

	typedef enum {
		MODEL_TEST,
		MODEL_MAX,
	}MODEL_LABEL;

	struct Model
	{
		LPD3DXMESH p_mesh_;
		D3DMATERIAL9* p_meshmaterial_;
		LPDIRECT3DTEXTURE9* p_meshtexture_;
		DWORD dw_material_;
		D3DXVECTOR3 position_;
		D3DXVECTOR3 scall_;
		D3DXCOLOR color_;
		float rotatiion_;
		bool isscall_;
		int  index_;
		D3DXMATRIX world_[XMODEL_MAX];
		Model()
		{
		}
		~Model()
		{
		}
	};
}

LPDIRECT3DDEVICE9 GetDevice(void);

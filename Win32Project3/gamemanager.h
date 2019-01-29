#pragma once
#include "main.h"
#include "input.h"
#include "SceneMode.h"
#include "sound.h"
//ëOï˚êÈåæ
//class CCamera;

class CManager {
private:
	static CMode* m_Mode;

	static CInputJoypad* m_pInputJoypad;
	static CInputKeyboard* m_pInputKeyboard;
	static CInputMouse* m_pInputMouse;
	static CSound* sound_;
public:
	CManager(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight);
	~CManager(){}

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
   
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
	static CSound *GetSound(void) { return sound_; }
public:
	static void SetMode(CMode *Mode);
};
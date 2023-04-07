#pragma once

#include "Player.h"
#include "Scene.h"

class CGameFramework
{
public:
	CGameFramework() {}
	~CGameFramework() {}

private:
	HINSTANCE m_hInstance = NULL;
	HWND m_hWnd = NULL;

	RECT m_rcClient;

	HDC m_hDCFrameBuffer = NULL;
	HBITMAP m_hBitmapFrameBuffer = NULL;
	HBITMAP m_hBitmapOldFrameBuffer = NULL;

	CPlayer* m_pPlayer = NULL;
	CScene* m_pScene = NULL;

public:
	void OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void BuildFrameBuffer();
	void ClearFrameBuffer(DWORD dwColor);
	void PresentFrameBuffer();

	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();
};


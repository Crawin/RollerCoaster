#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Player.h"

class CScene
{
public:
	CScene(CPlayer *pPlayer);
	virtual ~CScene();

private:
	int							m_nObjects = 0;
	int							m_nRails = 0;

	CGameObject					**m_ppObjects = NULL;

	CWallsObject*				m_pWallsObject = NULL;

	CPlayer*					m_pPlayer = NULL;

	CRailObject**				m_ppRailObject[4] = { NULL };
#ifdef _WITH_DRAW_AXIS
	CGameObject*				m_pWorldAxis = NULL;
#endif

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	void CheckObjectByCamCollisions();
	void CheckObjectByObjectCollisions();
	void CheckObjectByWallCollisions();
	void CheckPlayerByWallCollision();
	void CheckObjectByBulletCollisions();
	void FollowRail();
	void MakeRail(const int& railset);

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	CGameObject* PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera);
};


#include "stdafx.h"
#include "Scene.h"
#include "GraphicsPipeline.h"
#include <random>

CScene::CScene(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

CScene::~CScene()
{
}

void CScene::BuildObjects()
{
	CExplosiveObject::PrepareExplosion();

	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 200.0f;
	CWallMesh* pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);

	m_pWallsObject = new CWallsObject();
	m_pWallsObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_pWallsObject->SetMesh(pWallCubeMesh);
	m_pWallsObject->SetColor(RGB(0, 0, 0));
	m_pWallsObject->m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, fHalfDepth);
	m_pWallsObject->m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, fHalfDepth);
	m_pWallsObject->m_xmOOBBPlayerMoveCheck = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth * 0.05f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	float fRailWidth = 6.0f, fRailHeight = 1.0f, fRailDepth = 6.0f;
	CRailMesh* pRailMesh = new CRailMesh(fRailWidth, fRailHeight, fRailDepth);
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_int_distribution <> uid(0, 2);
	m_nRails = 3;
	m_ppRailObject = new CRailObject * [m_nRails];
	//for (int i = 0; i < m_nRails; ++i) {
	//	m_ppRailObject[i] = new CRailObject();
	//	m_ppRailObject[i]->SetPosition(0.0f, -1.0f, 6.0f * i);
	//	m_ppRailObject[i]->SetMesh(pRailMesh);
	//	m_ppRailObject[i]->SetColor(RGB(0, 255, 0));
	//	m_ppRailObject[i]->Rotate(XMFLOAT3(-1, 0, 0), 30.0f);
	//}
	m_ppRailObject[0] = new CRailObject();
	m_ppRailObject[0]->setRailType(0);
	m_ppRailObject[0]->SetRotationAxis(XMFLOAT3(-1, 0, 0));
	m_ppRailObject[0]->setRotationAngle(0);
	m_ppRailObject[0]->SetPosition(0.0f, -1.0f, 0.0f);
	m_ppRailObject[0]->SetMesh(pRailMesh);
	m_ppRailObject[0]->SetColor(RGB(0, 255, 0));
	m_ppRailObject[0]->Rotate();
	for (int i = 1; i < m_nRails; ++i) {
		int type = 1;
		m_ppRailObject[i] = new CRailObject();
		m_ppRailObject[i]->setRailType(type);
		m_ppRailObject[i]->SetRotationAxis(XMFLOAT3(-1, 0, 0));
		switch (type) {
		case 0:					// 평지
			m_ppRailObject[i]->setRotationAngle(0);
			switch (m_ppRailObject[i-1]->m_iRailType) {
			case 0:				// 평지 - 평지
				m_ppRailObject[i]->SetPosition(0.0f, m_ppRailObject[i - 1]->GetPosition().y, m_ppRailObject[i - 1]->GetPosition().z + fRailDepth);
				break;
			case 1:				// 오르막 - 평지
			case 2:				// 내리막 - 평지
				m_ppRailObject[i]->SetPosition(0.0f, m_ppRailObject[i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle)), m_ppRailObject[i - 1]->GetPosition().z + fRailDepth * 0.5 + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle)));
				break;
			}
			break;
		case 1:					// 오르막
			m_ppRailObject[i]->setRotationAngle(30);
			switch (m_ppRailObject[i-1]->m_iRailType) {
			case 0:				// 평지 - 오르막
				m_ppRailObject[i]->SetPosition(0.0f, m_ppRailObject[i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle)), m_ppRailObject[i - 1]->GetPosition().z + fRailDepth * 0.5 + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle)));
				break;
			case 1:				// 오르막 - 오르막
				m_ppRailObject[i]->SetPosition(0.0f, m_ppRailObject[i - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle))), m_ppRailObject[i - 1]->GetPosition().z + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[i - 1]->m_dRotationAngle + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle)))));
				break;
			case 2:				// 내리막 - 오르막
				m_ppRailObject[i]->SetPosition(0.0f, m_ppRailObject[i - 1]->GetPosition().y, m_ppRailObject[i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[i - 1]->m_dRotationAngle) + cos(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle)))));
				break;
			}
			break;
		case 2:					// 내리막
			m_ppRailObject[i]->setRotationAngle(-30);
			switch (m_ppRailObject[i-1]->m_iRailType) {
			case 0:				// 평지 - 내리막
				m_ppRailObject[i]->SetPosition(0.0f, m_ppRailObject[i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle)), m_ppRailObject[i - 1]->GetPosition().z + fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle))));
				break;
			case 1:				// 오르막 - 내리막
				m_ppRailObject[i]->SetPosition(0.0f, m_ppRailObject[i - 1]->GetPosition().y, m_ppRailObject[i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[i - 1]->m_dRotationAngle) + cos(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle)))));
				break;
			case 2:				// 내리막 - 내리막
				m_ppRailObject[i]->SetPosition(0.0f, m_ppRailObject[i - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle))), m_ppRailObject[i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle))));
				break;
			}
			break;
		case 3:					// 우회전
			break;
		case 4:					// 좌회전
			break;
		}
		m_ppRailObject[i]->SetMesh(pRailMesh);
		m_ppRailObject[i]->SetColor(RGB(0, 255, 0));
		m_ppRailObject[i]->Rotate();
	}
		//m_ppRailObject[1] = new CRailObject();
		//m_ppRailObject[1]->setRailType(1);
		//m_ppRailObject[1]->SetRotationAxis(XMFLOAT3(-1, 0, 0));
		//m_ppRailObject[1]->setRotationAngle(30);
		//m_ppRailObject[1]->SetPosition(0.0f, m_ppRailObject[1 - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[1]->m_dRotationAngle)), m_ppRailObject[1 - 1]->GetPosition().z + fRailDepth * 0.5 + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[1]->m_dRotationAngle)));
		//m_ppRailObject[1]->SetMesh(pRailMesh);
		//m_ppRailObject[1]->SetColor(RGB(0, 255, 0));
		//m_ppRailObject[1]->Rotate();
	
		//m_ppRailObject[2] = new CRailObject();
		//m_ppRailObject[2]->setRailType(2);
		//m_ppRailObject[2]->SetRotationAxis(XMFLOAT3(-1, 0, 0));
		//m_ppRailObject[2]->setRotationAngle(-30);
		//m_ppRailObject[2]->SetPosition(0.0f, m_ppRailObject[2 - 1]->GetPosition().y, m_ppRailObject[2-1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[2-1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[2]->m_dRotationAngle))));
		//m_ppRailObject[2]->SetMesh(pRailMesh);
		//m_ppRailObject[2]->SetColor(RGB(0, 255, 0));
		//m_ppRailObject[2]->Rotate();

		//m_ppRailObject[3] = new CRailObject();
		//m_ppRailObject[3]->setRailType(0);
		//m_ppRailObject[3]->SetRotationAxis(XMFLOAT3(-1, 0, 0));
		//m_ppRailObject[3]->setRotationAngle(0);
		//m_ppRailObject[3]->SetPosition(0.0f, m_ppRailObject[3 - 1]->GetPosition().y + fRailDepth*0.5*sin(DegreeToRadian(m_ppRailObject[3-1]->m_dRotationAngle)), m_ppRailObject[3 - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3 - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[3]->m_dRotationAngle))));
		//m_ppRailObject[3]->SetMesh(pRailMesh);
		//m_ppRailObject[3]->SetColor(RGB(0, 255, 0));
		//m_ppRailObject[3]->Rotate();
	
	//m_ppRailObject[3] = new CRailObject();
	//m_ppRailObject[3]->SetPosition(0.0f, m_ppRailObject[1]->GetPosition().y + fRailDepth * 0.5f * tan(DegreeToRadian(-30)), 6.0f * 3);
	//m_ppRailObject[3]->SetMesh(pRailMesh);
	//m_ppRailObject[3]->SetColor(RGB(0, 255, 0));
	//m_ppRailObject[3]->Rotate(XMFLOAT3(-1, 0, 0), -30.0f);

	// 왜 있는지 잘 모르겠다
	//m_pRailObject->m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, fRailWidth * 0.5f);
	//m_pRailObject->m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, fRailWidth * 0.5f);
	//m_pRailObject->m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, fRailHeight * 0.5f);
	//m_pRailObject->m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, fRailHeight * 0.5f);
	//m_pRailObject->m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, fRailDepth * 0.5f);
	//m_pRailObject->m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, fRailDepth * 0.5f);


	CCubeMesh* pCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
	
	m_nObjects = 1;
	m_ppObjects = new CGameObject * [m_nObjects];

	m_ppObjects[0] = new CExplosiveObject();
	m_ppObjects[0]->SetMesh(pCubeMesh);
	m_ppObjects[0]->SetColor(RGB(255, 0, 0));
	m_ppObjects[0]->SetPosition(-13.5f, 0.0f, -14.0f);
	m_ppObjects[0]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_ppObjects[0]->SetRotationSpeed(0.0f);
	m_ppObjects[0]->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 0.0f));
	m_ppObjects[0]->SetMovingSpeed(0.0f);

	//m_ppObjects[0] = new CExplosiveObject();
	//m_ppObjects[0]->SetMesh(pCubeMesh);
	//m_ppObjects[0]->SetColor(RGB(255, 0, 0));
	//m_ppObjects[0]->SetPosition(-13.5f, 0.0f, -14.0f);
	//m_ppObjects[0]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	//m_ppObjects[0]->SetRotationSpeed(90.0f);
	//m_ppObjects[0]->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 0.0f));
	//m_ppObjects[0]->SetMovingSpeed(10.5f);

	//m_ppObjects[1] = new CExplosiveObject();
	//m_ppObjects[1]->SetMesh(pCubeMesh);
	//m_ppObjects[1]->SetColor(RGB(0, 0, 255));
	//m_ppObjects[1]->SetPosition(+13.5f, 0.0f, -14.0f);
	//m_ppObjects[1]->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	//m_ppObjects[1]->SetRotationSpeed(180.0f);
	//m_ppObjects[1]->SetMovingDirection(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	//m_ppObjects[1]->SetMovingSpeed(8.8f);

	//m_ppObjects[2] = new CExplosiveObject();
	//m_ppObjects[2]->SetMesh(pCubeMesh);
	//m_ppObjects[2]->SetColor(RGB(0, 255, 0));
	//m_ppObjects[2]->SetPosition(0.0f, +5.0f, 20.0f);
	//m_ppObjects[2]->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	//m_ppObjects[2]->SetRotationSpeed(30.15f);
	//m_ppObjects[2]->SetMovingDirection(XMFLOAT3(1.0f, -1.0f, 0.0f));
	//m_ppObjects[2]->SetMovingSpeed(5.2f);

	//m_ppObjects[3] = new CExplosiveObject();
	//m_ppObjects[3]->SetMesh(pCubeMesh);
	//m_ppObjects[3]->SetColor(RGB(0, 255, 255));
	//m_ppObjects[3]->SetPosition(0.0f, 0.0f, 0.0f);
	//m_ppObjects[3]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	//m_ppObjects[3]->SetRotationSpeed(40.6f);
	//m_ppObjects[3]->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	//m_ppObjects[3]->SetMovingSpeed(20.4f);

	//m_ppObjects[4] = new CExplosiveObject();
	//m_ppObjects[4]->SetMesh(pCubeMesh);
	//m_ppObjects[4]->SetColor(RGB(128, 0, 255));
	//m_ppObjects[4]->SetPosition(10.0f, 0.0f, 0.0f);
	//m_ppObjects[4]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//m_ppObjects[4]->SetRotationSpeed(50.06f);
	//m_ppObjects[4]->SetMovingDirection(XMFLOAT3(0.0f, 1.0f, 1.0f));
	//m_ppObjects[4]->SetMovingSpeed(6.4f);

	//m_ppObjects[5] = new CExplosiveObject();
	//m_ppObjects[5]->SetMesh(pCubeMesh);
	//m_ppObjects[5]->SetColor(RGB(255, 0, 255));
	//m_ppObjects[5]->SetPosition(-10.0f, 0.0f, -10.0f);
	//m_ppObjects[5]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//m_ppObjects[5]->SetRotationSpeed(60.06f);
	//m_ppObjects[5]->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 1.0f));
	//m_ppObjects[5]->SetMovingSpeed(8.9f);

	//m_ppObjects[6] = new CExplosiveObject();
	//m_ppObjects[6]->SetMesh(pCubeMesh);
	//m_ppObjects[6]->SetColor(RGB(255, 0, 255));
	//m_ppObjects[6]->SetPosition(-10.0f, 10.0f, -10.0f);
	//m_ppObjects[6]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//m_ppObjects[6]->SetRotationSpeed(60.06f);
	//m_ppObjects[6]->SetMovingDirection(XMFLOAT3(1.0f, 1.0f, 1.0f));
	//m_ppObjects[6]->SetMovingSpeed(9.7f);

	//m_ppObjects[7] = new CExplosiveObject();
	//m_ppObjects[7]->SetMesh(pCubeMesh);
	//m_ppObjects[7]->SetColor(RGB(255, 0, 128));
	//m_ppObjects[7]->SetPosition(-10.0f, 10.0f, -20.0f);
	//m_ppObjects[7]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//m_ppObjects[7]->SetRotationSpeed(70.06f);
	//m_ppObjects[7]->SetMovingDirection(XMFLOAT3(-1.0f, 1.0f, 1.0f));
	//m_ppObjects[7]->SetMovingSpeed(15.6f);

	//m_ppObjects[8] = new CExplosiveObject();
	//m_ppObjects[8]->SetMesh(pCubeMesh);
	//m_ppObjects[8]->SetColor(RGB(128, 0, 255));
	//m_ppObjects[8]->SetPosition(-15.0f, 10.0f, -30.0f);
	//m_ppObjects[8]->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	//m_ppObjects[8]->SetRotationSpeed(90.06f);
	//m_ppObjects[8]->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, -1.0f));
	//m_ppObjects[8]->SetMovingSpeed(15.0f);

	//m_ppObjects[9] = new CExplosiveObject();
	//m_ppObjects[9]->SetMesh(pCubeMesh);
	//m_ppObjects[9]->SetColor(RGB(255, 64, 64));
	//m_ppObjects[9]->SetPosition(+15.0f, 10.0f, 0.0f);
	//m_ppObjects[9]->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	//m_ppObjects[9]->SetRotationSpeed(90.06f);
	//m_ppObjects[9]->SetMovingDirection(XMFLOAT3(-0.0f, 0.0f, -1.0f));
	//m_ppObjects[9]->SetMovingSpeed(15.0f);

#ifdef _WITH_DRAW_AXIS
	m_pWorldAxis = new CGameObject();
	CAxisMesh* pAxisMesh = new CAxisMesh(0.5f, 0.5f, 0.5f);
	m_pWorldAxis->SetMesh(pAxisMesh);
#endif
}

void CScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();

	for (int i = 0; i < m_nObjects; i++) if (m_ppObjects[i]) delete m_ppObjects[i];
	if (m_ppObjects) delete[] m_ppObjects;

	if (m_pWallsObject) delete m_pWallsObject;

#ifdef _WITH_DRAW_AXIS
	if (m_pWorldAxis) delete m_pWorldAxis;
#endif
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[int(wParam - '1')];
			pExplosiveObject->m_bBlowingUp = true;
			break;
		}
		case 'A':
			for (int i = 0; i < m_nObjects; i++)
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

CGameObject* CScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera)
{
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / (float)pCamera->m_Viewport.m_nWidth) - 1) / pCamera->m_xmf4x4PerspectiveProject._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / (float)pCamera->m_Viewport.m_nHeight) - 1) / pCamera->m_xmf4x4PerspectiveProject._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->m_xmf4x4View);

	int nIntersected = 0;
	float fNearestHitDistance = FLT_MAX;
	CGameObject* pNearestObject = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		float fHitDistance = FLT_MAX;
		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
		{
			fNearestHitDistance = fHitDistance;
			pNearestObject = m_ppObjects[i];
		}
	}
	return(pNearestObject);
}

void CScene::CheckObjectByObjectCollisions()
{
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->m_pObjectCollided = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = (i + 1); j < m_nObjects; j++)
		{
			if (m_ppObjects[i]->m_xmOOBB.Intersects(m_ppObjects[j]->m_xmOOBB))
			{
				m_ppObjects[i]->m_pObjectCollided = m_ppObjects[j];
				m_ppObjects[j]->m_pObjectCollided = m_ppObjects[i];
			}
		}
	}
	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
			float fMovingSpeed = m_ppObjects[i]->m_fMovingSpeed;
			m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppObjects[i]->m_fMovingSpeed = m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppObjects[i]->m_pObjectCollided = NULL;
		}
	}
}

void CScene::CheckObjectByWallCollisions()
{
	for (int i = 0; i < m_nObjects; i++)
	{
		ContainmentType containType = m_pWallsObject->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB);
		switch (containType)
		{
		case DISJOINT:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == BACK)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case INTERSECTS:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == INTERSECTING)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case CONTAINS:
			break;
		}
	}
}

void CScene::CheckPlayerByWallCollision()
{
	BoundingOrientedBox xmOOBBPlayerMoveCheck;
	m_pWallsObject->m_xmOOBBPlayerMoveCheck.Transform(xmOOBBPlayerMoveCheck, XMLoadFloat4x4(&m_pWallsObject->m_xmf4x4World));
	XMStoreFloat4(&xmOOBBPlayerMoveCheck.Orientation, XMQuaternionNormalize(XMLoadFloat4(&xmOOBBPlayerMoveCheck.Orientation)));

	if (!xmOOBBPlayerMoveCheck.Intersects(m_pPlayer->m_xmOOBB))
		m_pWallsObject->SetPosition(m_pPlayer->m_xmf3Position);
}

void CScene::CheckPlayerByRailCollision()
{
	CCamera* pCamera = m_pPlayer->GetCamera();
	
	for (int i = 0; i < m_nRails; ++i) {
		if (pCamera->IsInFrustum(m_ppRailObject[i]->m_xmOOBB) && m_ppRailObject[i]->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB)) {
			m_pPlayer->Move(0, 0.15f * tan(DegreeToRadian(m_ppRailObject[i]->m_dRotationAngle)) - m_pPlayer->m_gravity, 0.15f);
		}
	}
	
}

void CScene::CheckObjectByBulletCollisions()
{
	CBulletObject** ppBullets = ((CAirplanePlayer*)m_pPlayer)->m_ppBullets;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && m_ppObjects[i]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
				ppBullets[j]->Reset();
			}
		}
	}
}

void CScene::Animate(float fElapsedTime)
{
	m_pWallsObject->Animate(fElapsedTime);

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fElapsedTime);

	for (int i = 0; i < m_nRails; i++) m_ppRailObject[i]->Animate(fElapsedTime);

	CheckPlayerByWallCollision();

	CheckObjectByWallCollisions();

	CheckObjectByObjectCollisions();

	CheckObjectByBulletCollisions();

	CheckPlayerByRailCollision();

	Gravity();
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);

	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);
	//m_pWallsObject->Render(hDCFrameBuffer, pCamera);
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
	for (int i = 0; i < m_nRails; i++) m_ppRailObject[i]->Render(hDCFrameBuffer, pCamera);
	if (m_pPlayer) m_pPlayer->Render(hDCFrameBuffer, pCamera);

//UI
#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}

void CScene::Gravity() {
	m_pPlayer->Move(0, m_pPlayer->m_gravity, 0);
}
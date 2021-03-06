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
	
	for (int i = 0; i < 4; ++i) {
		MakeRail(i);
	}

	CCubeMesh* pCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
	
	m_nObjects = 10;
	m_ppObjects = new CGameObject * [m_nObjects];

	m_ppObjects[0] = new CExplosiveObject();
	m_ppObjects[0]->SetMesh(pCubeMesh);
	m_ppObjects[0]->SetColor(RGB(255, 0, 0));
	m_ppObjects[0]->SetPosition(-13.5f, 0.0f, -14.0f);
	m_ppObjects[0]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_ppObjects[0]->SetRotationSpeed(90.0f);
	m_ppObjects[0]->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 0.0f));
	m_ppObjects[0]->SetMovingSpeed(10.5f);

	m_ppObjects[1] = new CExplosiveObject();
	m_ppObjects[1]->SetMesh(pCubeMesh);
	m_ppObjects[1]->SetColor(RGB(0, 0, 255));
	m_ppObjects[1]->SetPosition(+13.5f, 0.0f, -14.0f);
	m_ppObjects[1]->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	m_ppObjects[1]->SetRotationSpeed(180.0f);
	m_ppObjects[1]->SetMovingDirection(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	m_ppObjects[1]->SetMovingSpeed(8.8f);

	m_ppObjects[2] = new CExplosiveObject();
	m_ppObjects[2]->SetMesh(pCubeMesh);
	m_ppObjects[2]->SetColor(RGB(0, 255, 0));
	m_ppObjects[2]->SetPosition(0.0f, +5.0f, 20.0f);
	m_ppObjects[2]->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	m_ppObjects[2]->SetRotationSpeed(30.15f);
	m_ppObjects[2]->SetMovingDirection(XMFLOAT3(1.0f, -1.0f, 0.0f));
	m_ppObjects[2]->SetMovingSpeed(5.2f);

	m_ppObjects[3] = new CExplosiveObject();
	m_ppObjects[3]->SetMesh(pCubeMesh);
	m_ppObjects[3]->SetColor(RGB(0, 255, 255));
	m_ppObjects[3]->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[3]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_ppObjects[3]->SetRotationSpeed(40.6f);
	m_ppObjects[3]->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_ppObjects[3]->SetMovingSpeed(20.4f);

	m_ppObjects[4] = new CExplosiveObject();
	m_ppObjects[4]->SetMesh(pCubeMesh);
	m_ppObjects[4]->SetColor(RGB(128, 0, 255));
	m_ppObjects[4]->SetPosition(10.0f, 0.0f, 0.0f);
	m_ppObjects[4]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_ppObjects[4]->SetRotationSpeed(50.06f);
	m_ppObjects[4]->SetMovingDirection(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_ppObjects[4]->SetMovingSpeed(6.4f);

	m_ppObjects[5] = new CExplosiveObject();
	m_ppObjects[5]->SetMesh(pCubeMesh);
	m_ppObjects[5]->SetColor(RGB(255, 0, 255));
	m_ppObjects[5]->SetPosition(-10.0f, 0.0f, -10.0f);
	m_ppObjects[5]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_ppObjects[5]->SetRotationSpeed(60.06f);
	m_ppObjects[5]->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 1.0f));
	m_ppObjects[5]->SetMovingSpeed(8.9f);

	m_ppObjects[6] = new CExplosiveObject();
	m_ppObjects[6]->SetMesh(pCubeMesh);
	m_ppObjects[6]->SetColor(RGB(255, 0, 255));
	m_ppObjects[6]->SetPosition(-10.0f, 10.0f, -10.0f);
	m_ppObjects[6]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_ppObjects[6]->SetRotationSpeed(60.06f);
	m_ppObjects[6]->SetMovingDirection(XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_ppObjects[6]->SetMovingSpeed(9.7f);

	m_ppObjects[7] = new CExplosiveObject();
	m_ppObjects[7]->SetMesh(pCubeMesh);
	m_ppObjects[7]->SetColor(RGB(255, 0, 128));
	m_ppObjects[7]->SetPosition(-10.0f, 10.0f, -20.0f);
	m_ppObjects[7]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_ppObjects[7]->SetRotationSpeed(70.06f);
	m_ppObjects[7]->SetMovingDirection(XMFLOAT3(-1.0f, 1.0f, 1.0f));
	m_ppObjects[7]->SetMovingSpeed(15.6f);

	m_ppObjects[8] = new CExplosiveObject();
	m_ppObjects[8]->SetMesh(pCubeMesh);
	m_ppObjects[8]->SetColor(RGB(128, 0, 255));
	m_ppObjects[8]->SetPosition(-15.0f, 10.0f, -30.0f);
	m_ppObjects[8]->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	m_ppObjects[8]->SetRotationSpeed(90.06f);
	m_ppObjects[8]->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, -1.0f));
	m_ppObjects[8]->SetMovingSpeed(15.0f);

	m_ppObjects[9] = new CExplosiveObject();
	m_ppObjects[9]->SetMesh(pCubeMesh);
	m_ppObjects[9]->SetColor(RGB(255, 64, 64));
	m_ppObjects[9]->SetPosition(+15.0f, 10.0f, 0.0f);
	m_ppObjects[9]->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	m_ppObjects[9]->SetRotationSpeed(90.06f);
	m_ppObjects[9]->SetMovingDirection(XMFLOAT3(-0.0f, 0.0f, -1.0f));
	m_ppObjects[9]->SetMovingSpeed(15.0f);

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

void CScene::MakeRail(const int& railset) {
	float fRailWidth = 6.0f, fRailHeight = 1.0f, fRailDepth = 6.0f;
	CRailStraightMesh* pRailStraightMesh = new CRailStraightMesh(fRailWidth, fRailHeight, fRailDepth);
	CRailCornerMesh* pRailCornerMesh = new CRailCornerMesh(fRailWidth * 2, fRailHeight, fRailDepth * 2);
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_int_distribution <> uid(0, 3);
	m_nRails = 10;
	m_ppRailObject[railset] = new CRailObject * [m_nRails];
	if (m_pPlayer->m_bPlayerStart) {
		m_pPlayer->m_bPlayerStart = FALSE;
		m_ppRailObject[railset][0] = new CRailObject();
		m_ppRailObject[railset][0]->setRailSize(XMFLOAT3(fRailWidth, fRailHeight, fRailDepth));
		m_ppRailObject[railset][0]->setRailType(0);
		m_ppRailObject[railset][0]->SetRotationAxis(XMFLOAT3(-1, 0, 0));
		m_ppRailObject[railset][0]->setRotationAngle(0);
		m_ppRailObject[railset][0]->SetPosition(0.0f, -1.0f, 0.0f);
		m_ppRailObject[railset][0]->setGoalPosition(0, +0.5, fRailDepth * 0.5);
		m_ppRailObject[railset][0]->SetMesh(pRailStraightMesh);
		m_ppRailObject[railset][0]->SetColor(RGB(0, 255, 0));
		m_ppRailObject[railset][0]->Rotate();
	}
	else {
		int type = uid(dre);
		m_ppRailObject[railset][0] = new CRailObject();
		m_ppRailObject[railset][0]->setRailSize(XMFLOAT3(fRailWidth, fRailHeight, fRailDepth));
		m_ppRailObject[railset][0]->setRailType(type);
		m_ppRailObject[railset][0]->SetRotationAxis(XMFLOAT3(-1, 0, 0));
		if (railset == 0) {
			switch (type) {
			case 0:					// ????
				m_ppRailObject[railset][0]->setRotationAngle(0);
				m_ppRailObject[railset][0]->SetMesh(pRailStraightMesh);
				m_ppRailObject[railset][0]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
				switch (m_ppRailObject[3][m_nRails - 1]->m_iRailType) {
				case 0:				// ???? - ????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 1:				// ?????? - ????
				case 2:				// ?????? - ????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + 1)); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + 1), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + 1)); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + 1), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 3:				// ?????? - ????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 1.5); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 1.5); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					}
					break;
				case 4:				// ?????? - ????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 1.5); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 1.5); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					}
					break;
				}
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][0]->changeGoalPosition(0, 0.5, fRailDepth * 0.5); break;
				case 3:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth * 0.5, 0.5, 0); break;
				case 6:m_ppRailObject[railset][0]->changeGoalPosition(0, 0.5, -fRailDepth * 0.5); break;
				case 9:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth * 0.5, 0.5, 0); break;
				}
				break;
			case 1:					// ??????
				m_ppRailObject[railset][0]->setRotationAngle(30);
				m_ppRailObject[railset][0]->SetMesh(pRailStraightMesh);
				m_ppRailObject[railset][0]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
				switch (m_ppRailObject[3][m_nRails - 1]->m_iRailType) {
				case 0:				// ???? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 1:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 2:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 3:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					}
					break;
				case 4:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					}
					break;
				}
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][0]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
				case 3:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, 0); break;
				case 6:m_ppRailObject[railset][0]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, -fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
				case 9:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, 0); break;
				}
				break;
			case 2:					// ??????
				m_ppRailObject[railset][0]->setRotationAngle(-30);
				m_ppRailObject[railset][0]->SetMesh(pRailStraightMesh);
				m_ppRailObject[railset][0]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
				switch (m_ppRailObject[3][m_nRails - 1]->m_iRailType) {
				case 0:				// ???? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 1:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 2:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[3][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 3:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * +cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailDepth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					}
					break;
				case 4:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					}
					break;
				}
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][0]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
				case 3:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, 0); break;
				case 6:m_ppRailObject[railset][0]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, -fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
				case 9:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, 0); break;
				}
				break;
			case 3:					// ??????
				m_ppRailObject[railset][0]->setRotationAngle(0);
				m_ppRailObject[railset][0]->SetMesh(pRailCornerMesh);
				m_ppRailObject[railset][0]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
				switch (m_ppRailObject[3][m_nRails - 1]->m_iRailType) {
				case 0:				// ???? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 6;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 1.5);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 12;
						break;
					}
					break;
				case 1:				// ?????? - ??????
				case 2:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + 2));
						m_pPlayer->m_iRailHead = 3;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + fRailWidth, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 6;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) - fRailDepth);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) - fRailWidth, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 12;
						break;
					}
					break;
				case 3:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 2);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z);
						m_pPlayer->m_iRailHead = 6;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 2);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z);
						m_pPlayer->m_iRailHead = 12;
						break;
					}
					break;
				case 4:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 2);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth);
						m_pPlayer->m_iRailHead = 6;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 2);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth);
						m_pPlayer->m_iRailHead = 12;
						break;
					}
					break;
				}
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth * 0.5, 0.5, fRailDepth); break;
				case 3:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth, 0.5, fRailDepth * 0.5); break;
				case 6:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth * 0.5, 0.5, -fRailDepth); break;
				case 9:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth, 0.5, -fRailDepth * 0.5); break;
				}
				break;
			case 4:					// ??????
				m_ppRailObject[railset][0]->setRotationAngle(0);
				m_ppRailObject[railset][0]->RotateHead(m_pPlayer->m_iRailHead, FALSE);
				m_ppRailObject[railset][0]->SetMesh(pRailCornerMesh);
				switch (m_ppRailObject[3][m_nRails - 1]->m_iRailType) {
				case 0:				// ???? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:// ????
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 1.5);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 12;
						break;
					case 6: // ????
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 1.5);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 6;
						break;
					}
					break;
				case 1:				// ?????? - ??????
				case 2:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + fRailDepth);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) + fRailWidth, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 12;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) - fRailDepth);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)) - fRailWidth, m_ppRailObject[3][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[3][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 6;
						break;
					}
					break;
				case 3:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 2);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth);
						m_pPlayer->m_iRailHead = 12;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 2);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth);
						m_pPlayer->m_iRailHead = 6;
						break;
					}
					break;
				case 4:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z + fRailDepth * 2);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z);
						m_pPlayer->m_iRailHead = 12;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z - fRailDepth * 2);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[3][m_nRails - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[3][m_nRails - 1]->GetPosition().y, m_ppRailObject[3][m_nRails - 1]->GetPosition().z);
						m_pPlayer->m_iRailHead = 6;
						break;
					}
					break;
				}
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth * 0.5, 0.5, fRailDepth); break;
				case 3:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth, 0.5, -fRailDepth * 0.5); break;
				case 6:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth * 0.5, 0.5, -fRailDepth); break;
				case 9:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth, 0.5, fRailDepth * 0.5); break;
				}
				break;
			}
		}
		else {
			switch (type) {
			case 0:					// ????
				m_ppRailObject[railset][0]->setRotationAngle(0);
				m_ppRailObject[railset][0]->SetMesh(pRailStraightMesh);
				m_ppRailObject[railset][0]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
				switch (m_ppRailObject[railset - 1][m_nRails - 1]->m_iRailType) {
				case 0:				// ???? - ????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 1:				// ?????? - ????
				case 2:				// ?????? - ????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + 1)); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + 1), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + 1)); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + 1), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 3:				// ?????? - ????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 1.5); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 1.5); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					}
					break;
				case 4:				// ?????? - ????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 1.5); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 1.5); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					}
					break;
				}
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][0]->changeGoalPosition(0, +0.5, fRailDepth * 0.5); break;
				case 3:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth * 0.5, +0.5, 0); break;
				case 6:m_ppRailObject[railset][0]->changeGoalPosition(0, +0.5, -fRailDepth * 0.5); break;
				case 9:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth * 0.5, +0.5, 0); break;
				}
				break;
			case 1:					// ??????
				m_ppRailObject[railset][0]->setRotationAngle(30);
				m_ppRailObject[railset][0]->SetMesh(pRailStraightMesh);
				m_ppRailObject[railset][0]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
				switch (m_ppRailObject[railset - 1][m_nRails - 1]->m_iRailType) {
				case 0:				// ???? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 1:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 2:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 3:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					}
					break;
				case 4:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					}
					break;
				}
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][0]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
				case 3:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, 0); break;
				case 6:m_ppRailObject[railset][0]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, -fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
				case 9:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, 0); break;
				}
				break;
			case 2:					// ??????
				m_ppRailObject[railset][0]->setRotationAngle(-30);
				m_ppRailObject[railset][0]->SetMesh(pRailStraightMesh);
				m_ppRailObject[railset][0]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
				switch (m_ppRailObject[railset - 1][m_nRails - 1]->m_iRailType) {
				case 0:				// ???? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 1:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 2:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z); break;
					}
					break;
				case 3:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * +cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailDepth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					}
					break;
				case 4:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 3:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5); break;
					case 6:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
					case 9:m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5); break;
					}
					break;
				}
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][0]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
				case 3:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, 0); break;
				case 6:m_ppRailObject[railset][0]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, -fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle))); break;
				case 9:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][0]->m_dRotationAngle)) + 0.5, 0); break;
				}
				break;
			case 3:					// ??????
				m_ppRailObject[railset][0]->setRotationAngle(0);
				m_ppRailObject[railset][0]->SetMesh(pRailCornerMesh);
				m_ppRailObject[railset][0]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
				switch (m_ppRailObject[railset - 1][m_nRails - 1]->m_iRailType) {
				case 0:				// ???? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 6;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 1.5);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 12;
						break;
					}
					break;
				case 1:				// ?????? - ??????
				case 2:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + 2));
						m_pPlayer->m_iRailHead = 3;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + fRailWidth, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 6;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) - fRailDepth);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) - fRailWidth, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 12;
						break;
					}
					break;
				case 3:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 2);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z);
						m_pPlayer->m_iRailHead = 6;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 2);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z);
						m_pPlayer->m_iRailHead = 12;
						break;
					}
					break;
				case 4:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 2);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth);
						m_pPlayer->m_iRailHead = 6;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 2);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth);
						m_pPlayer->m_iRailHead = 12;
						break;
					}
					break;
				}
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth * 0.5, +0.5, fRailDepth); break;
				case 3:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth, +0.5, fRailDepth * 0.5); break;
				case 6:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth * 0.5, +0.5, -fRailDepth); break;
				case 9:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth, +0.5, -fRailDepth * 0.5); break;
				}
				break;
			case 4:					// ??????
				m_ppRailObject[railset][0]->setRotationAngle(0);
				m_ppRailObject[railset][0]->RotateHead(m_pPlayer->m_iRailHead, FALSE);
				m_ppRailObject[railset][0]->SetMesh(pRailCornerMesh);
				switch (m_ppRailObject[railset - 1][m_nRails - 1]->m_iRailType) {
				case 0:				// ???? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:// ????
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 1.5);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 12;
						break;
					case 6: // ????
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 1.5);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 6;
						break;
					}
					break;
				case 1:				// ?????? - ??????
				case 2:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + fRailDepth);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) + fRailWidth, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 12;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) - fRailDepth);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)) - fRailWidth, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset - 1][m_nRails - 1]->m_dRotationAngle)), m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 0.5);
						m_pPlayer->m_iRailHead = 6;
						break;
					}
					break;
				case 3:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 2);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth);
						m_pPlayer->m_iRailHead = 12;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 2);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth);
						m_pPlayer->m_iRailHead = 6;
						break;
					}
					break;
				case 4:				// ?????? - ??????
					switch (m_pPlayer->m_iRailHead) {
					case 12:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z + fRailDepth * 2);
						m_pPlayer->m_iRailHead = 9;
						break;
					case 3:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z);
						m_pPlayer->m_iRailHead = 12;
						break;
					case 6:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z - fRailDepth * 2);
						m_pPlayer->m_iRailHead = 3;
						break;
					case 9:
						m_ppRailObject[railset][0]->SetPosition(m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().y, m_ppRailObject[railset - 1][m_nRails - 1]->GetPosition().z);
						m_pPlayer->m_iRailHead = 6;
						break;
					}
					break;
				}
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth * 0.5, +0.5, fRailDepth); break;
				case 3:m_ppRailObject[railset][0]->changeGoalPosition(fRailDepth, +0.5, -fRailDepth * 0.5); break;
				case 6:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth * 0.5, +0.5, -fRailDepth); break;
				case 9:m_ppRailObject[railset][0]->changeGoalPosition(-fRailDepth, +0.5, fRailDepth * 0.5); break;
				}
				break;
			}
		}
		m_ppRailObject[railset][0]->SetColor(RGB(0, 255, 0));
		m_ppRailObject[railset][0]->Rotate();
	}
	for (int i = 1; i < m_nRails; ++i) {
		int type = uid(dre);
		m_ppRailObject[railset][i] = new CRailObject();
		m_ppRailObject[railset][i]->setRailSize(XMFLOAT3(fRailWidth, fRailHeight, fRailDepth));
		m_ppRailObject[railset][i]->setRailType(type);
		m_ppRailObject[railset][i]->SetRotationAxis(XMFLOAT3(-1, 0, 0));
		switch (type) {
		case 0:					// ????
			m_ppRailObject[railset][i]->setRotationAngle(0);
			m_ppRailObject[railset][i]->SetMesh(pRailStraightMesh);
			m_ppRailObject[railset][i]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
			switch (m_ppRailObject[railset][i - 1]->m_iRailType) {
			case 0:				// ???? - ????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				}
				break;
			case 1:				// ?????? - ????
			case 2:				// ?????? - ????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + 1)); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + 1), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + 1)); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + 1), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				}
				break;
			case 3:				// ?????? - ????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 1.5); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 1.5); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
				}
				break;
			case 4:				// ?????? - ????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 1.5); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 1.5); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
				}
				break;
			}
			switch (m_pPlayer->m_iRailHead) {
			case 12:m_ppRailObject[railset][i]->changeGoalPosition(0, +0.5, fRailDepth * 0.5); break;
			case 3:m_ppRailObject[railset][i]->changeGoalPosition(fRailDepth * 0.5, +0.5, 0); break;
			case 6:m_ppRailObject[railset][i]->changeGoalPosition(0, +0.5, -fRailDepth * 0.5); break;
			case 9:m_ppRailObject[railset][i]->changeGoalPosition(-fRailDepth * 0.5, +0.5, 0); break;
			}
			break;
		case 1:					// ??????
			m_ppRailObject[railset][i]->setRotationAngle(30);
			m_ppRailObject[railset][i]->SetMesh(pRailStraightMesh);
			m_ppRailObject[railset][i]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
			switch (m_ppRailObject[railset][i - 1]->m_iRailType) {
			case 0:				// ???? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				}
				break;
			case 1:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				}
				break;
			case 2:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				}
				break;
			case 3:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
				}
				break;
			case 4:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
				}
				break;
			}
			switch (m_pPlayer->m_iRailHead) {
			case 12:m_ppRailObject[railset][i]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)) + 0.5, fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
			case 3:m_ppRailObject[railset][i]->changeGoalPosition(fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)) + 0.5, 0); break;
			case 6:m_ppRailObject[railset][i]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)) + 0.5, -fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
			case 9:m_ppRailObject[railset][i]->changeGoalPosition(-fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)) + 0.5, 0); break;
			}
			break;
		case 2:					// ??????
			m_ppRailObject[railset][i]->setRotationAngle(-30);
			m_ppRailObject[railset][i]->SetMesh(pRailStraightMesh);
			m_ppRailObject[railset][i]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
			switch (m_ppRailObject[railset][i - 1]->m_iRailType) {
			case 0:				// ???? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				}
				break;
			case 1:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				}
				break;
			case 2:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)))); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))), m_ppRailObject[railset][i - 1]->GetPosition().z); break;
				}
				break;
			case 3:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * +cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailDepth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
				}
				break;
			case 4:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
				case 3:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
				case 6:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
				case 9:m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
				}
				break;
			}
			switch (m_pPlayer->m_iRailHead) {
			case 12:m_ppRailObject[railset][i]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)) + 0.5, fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
			case 3:m_ppRailObject[railset][i]->changeGoalPosition(fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)) + 0.5, 0); break;
			case 6:m_ppRailObject[railset][i]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)) + 0.5, -fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle))); break;
			case 9:m_ppRailObject[railset][i]->changeGoalPosition(-fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i]->m_dRotationAngle)) + 0.5, 0); break;
			}
			break;
		case 3:					// ??????
			m_ppRailObject[railset][i]->setRotationAngle(0);
			m_ppRailObject[railset][i]->SetMesh(pRailCornerMesh);
			m_ppRailObject[railset][i]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
			switch (m_ppRailObject[railset][i - 1]->m_iRailType) {
			case 0:				// ???? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5);
					m_pPlayer->m_iRailHead = 3;
					break;
				case 3:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5);
					m_pPlayer->m_iRailHead = 6;
					break;
				case 6:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 1.5);
					m_pPlayer->m_iRailHead = 9;
					break;
				case 9:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5);
					m_pPlayer->m_iRailHead = 12;
					break;
				}
				break;
			case 1:				// ?????? - ??????
			case 2:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + 2));
					m_pPlayer->m_iRailHead = 3;
					break;
				case 3:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + fRailWidth, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5);
					m_pPlayer->m_iRailHead = 6;
					break;
				case 6:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) - fRailDepth);
					m_pPlayer->m_iRailHead = 9;
					break;
				case 9:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) - fRailWidth, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5);
					m_pPlayer->m_iRailHead = 12;
					break;
				}
				break;
			case 3:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 2);
					m_pPlayer->m_iRailHead = 3;
					break;
				case 3:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z);
					m_pPlayer->m_iRailHead = 6;
					break;
				case 6:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 2);
					m_pPlayer->m_iRailHead = 9;
					break;
				case 9:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z);
					m_pPlayer->m_iRailHead = 12;
					break;
				}
				break;
			case 4:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 2);
					m_pPlayer->m_iRailHead = 3;
					break;
				case 3:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth);
					m_pPlayer->m_iRailHead = 6;
					break;
				case 6:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 2);
					m_pPlayer->m_iRailHead = 9;
					break;
				case 9:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth);
					m_pPlayer->m_iRailHead = 12;
					break;
				}
				break;
			}
			switch (m_pPlayer->m_iRailHead) {
			case 12:m_ppRailObject[railset][i]->changeGoalPosition(-fRailDepth * 0.5, +0.5, fRailDepth); break;
			case 3:m_ppRailObject[railset][i]->changeGoalPosition(fRailDepth, +0.5, fRailDepth * 0.5); break;
			case 6:m_ppRailObject[railset][i]->changeGoalPosition(fRailDepth * 0.5, +0.5, -fRailDepth); break;
			case 9:m_ppRailObject[railset][i]->changeGoalPosition(-fRailDepth, +0.5, -fRailDepth * 0.5); break;
			}
			break;
		case 4:					// ??????
			m_ppRailObject[railset][i]->setRotationAngle(0);
			m_ppRailObject[railset][i]->RotateHead(m_pPlayer->m_iRailHead, FALSE);
			m_ppRailObject[railset][i]->SetMesh(pRailCornerMesh);
			switch (m_ppRailObject[railset][i - 1]->m_iRailType) {
			case 0:				// ???? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:// ????
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 1.5);
					m_pPlayer->m_iRailHead = 9;
					break;
				case 3:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5);
					m_pPlayer->m_iRailHead = 12;
					break;
				case 6: // ????
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 1.5);
					m_pPlayer->m_iRailHead = 3;
					break;
				case 9:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5);
					m_pPlayer->m_iRailHead = 6;
					break;
				}
				break;
			case 1:				// ?????? - ??????
			case 2:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + fRailDepth);
					m_pPlayer->m_iRailHead = 9;
					break;
				case 3:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) + fRailWidth, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 0.5);
					m_pPlayer->m_iRailHead = 12;
					break;
				case 6:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) - fRailDepth);
					m_pPlayer->m_iRailHead = 3;
					break;
				case 9:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)) - fRailWidth, m_ppRailObject[railset][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[railset][i - 1]->m_dRotationAngle)), m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 0.5);
					m_pPlayer->m_iRailHead = 6;
					break;
				}
				break;
			case 3:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 2);
					m_pPlayer->m_iRailHead = 9;
					break;
				case 3:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth);
					m_pPlayer->m_iRailHead = 12;
					break;
				case 6:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 2);
					m_pPlayer->m_iRailHead = 3;
					break;
				case 9:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth);
					m_pPlayer->m_iRailHead = 6;
					break;
				}
				break;
			case 4:				// ?????? - ??????
				switch (m_pPlayer->m_iRailHead) {
				case 12:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z + fRailDepth * 2);
					m_pPlayer->m_iRailHead = 9;
					break;
				case 3:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z);
					m_pPlayer->m_iRailHead = 12;
					break;
				case 6:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z - fRailDepth * 2);
					m_pPlayer->m_iRailHead = 3;
					break;
				case 9:
					m_ppRailObject[railset][i]->SetPosition(m_ppRailObject[railset][i - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[railset][i - 1]->GetPosition().y, m_ppRailObject[railset][i - 1]->GetPosition().z);
					m_pPlayer->m_iRailHead = 6;
					break;
				}
				break;
			}
			switch (m_pPlayer->m_iRailHead) {
			case 12:m_ppRailObject[railset][i]->changeGoalPosition(fRailDepth * 0.5, +0.5, fRailDepth); break;
			case 3:m_ppRailObject[railset][i]->changeGoalPosition(fRailDepth, +0.5, -fRailDepth * 0.5); break;
			case 6:m_ppRailObject[railset][i]->changeGoalPosition(-fRailDepth * 0.5, +0.5, -fRailDepth); break;
			case 9:m_ppRailObject[railset][i]->changeGoalPosition(-fRailDepth, +0.5, fRailDepth * 0.5); break;
			}
			break;
		}
		m_ppRailObject[railset][i]->SetColor(RGB(0, 255, 0));
		m_ppRailObject[railset][i]->Rotate();
	}
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

void CScene::CheckObjectByCamCollisions() {
	CCamera* pCamera = m_pPlayer->GetCamera();
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution <> urd(-100, 100);
	std::uniform_real_distribution <> rspeed(0, 30);
	for (int i = 0; i < m_nObjects; ++i) {
		if (!pCamera->IsInFrustum(m_ppObjects[i]->m_xmOOBB)) {
			m_ppObjects[i]->SetPosition(m_pPlayer->GetPosition().x + urd(dre), m_pPlayer->GetPosition().y + urd(dre), m_pPlayer->GetPosition().z + urd(dre));
			m_ppObjects[i]->SetMovingDirection(XMFLOAT3(m_pPlayer->GetPosition().x - m_ppObjects[i]->GetPosition().x, m_pPlayer->GetPosition().y - m_ppObjects[i]->GetPosition().y, m_pPlayer->GetPosition().z - m_ppObjects[i]->GetPosition().z));				// ???????? ???????? ????????
			m_ppObjects[i]->SetMovingSpeed(rspeed(dre));
		}
	}
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
	//m_pWallsObject->Animate(fElapsedTime);

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fElapsedTime);

	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < m_nRails; i++) m_ppRailObject[j][i]->Animate(fElapsedTime);

	}
	//CheckPlayerByWallCollision();

	//CheckObjectByWallCollisions();
	CheckObjectByCamCollisions();

	CheckObjectByObjectCollisions();

	CheckObjectByBulletCollisions();

	FollowRail();
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);

	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);
	//m_pWallsObject->Render(hDCFrameBuffer, pCamera);
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < m_nRails; i++) m_ppRailObject[j][i]->Render(hDCFrameBuffer, pCamera);
	}
	if (m_pPlayer) m_pPlayer->Render(hDCFrameBuffer, pCamera);

//UI
#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}

void CScene::FollowRail() {
	m_pPlayer->MoveTo(m_ppRailObject[m_pPlayer->m_iPlayerRailSet][m_pPlayer->m_iPlayerRailNum]->getGoalPosition(), 0.15f * tan(DegreeToRadian(m_ppRailObject[m_pPlayer->m_iPlayerRailSet][m_pPlayer->m_iPlayerRailNum]->m_dRotationAngle)));
	if (m_pPlayer->GetPosition().x >= m_ppRailObject[m_pPlayer->m_iPlayerRailSet][m_pPlayer->m_iPlayerRailNum]->getGoalPosition().x - 1 &&
		m_pPlayer->GetPosition().x <= m_ppRailObject[m_pPlayer->m_iPlayerRailSet][m_pPlayer->m_iPlayerRailNum]->getGoalPosition().x + 1 &&
		m_pPlayer->GetPosition().y >= m_ppRailObject[m_pPlayer->m_iPlayerRailSet][m_pPlayer->m_iPlayerRailNum]->getGoalPosition().y - 1 &&
		m_pPlayer->GetPosition().y <= m_ppRailObject[m_pPlayer->m_iPlayerRailSet][m_pPlayer->m_iPlayerRailNum]->getGoalPosition().y + 1 &&
		m_pPlayer->GetPosition().z >= m_ppRailObject[m_pPlayer->m_iPlayerRailSet][m_pPlayer->m_iPlayerRailNum]->getGoalPosition().z - 1 &&
		m_pPlayer->GetPosition().z <= m_ppRailObject[m_pPlayer->m_iPlayerRailSet][m_pPlayer->m_iPlayerRailNum]->getGoalPosition().z + 1) {
		m_pPlayer->m_iPlayerRailNum++;
		if (m_pPlayer->m_iPlayerRailNum > 9) {

			if (m_pPlayer->m_bDeleteStart) {
				delete[] m_ppRailObject[(m_pPlayer->m_iPlayerRailSet + 2) % 4];
				MakeRail((m_pPlayer->m_iPlayerRailSet + 2) % 4);
			}
			m_pPlayer->m_iPlayerRailNum = 0;
			m_pPlayer->m_iPlayerRailSet++;
			if (!(m_pPlayer->m_bDeleteStart) && m_pPlayer->m_iPlayerRailSet == 2) {
				m_pPlayer->m_bDeleteStart = TRUE;
			}
			if (m_pPlayer->m_iPlayerRailSet > 3) {
				m_pPlayer->m_iPlayerRailSet = 0;
			}
		}
	}
}
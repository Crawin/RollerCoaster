#include "stdafx.h"
#include "GameObject.h"
#include "GraphicsPipeline.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
inline float RandF(float fMin, float fMax)
{
	return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
}

XMVECTOR RandomUnitVectorOnSphere()
{
	XMVECTOR xmvOne = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR xmvZero = XMVectorZero();

	while (true)
	{
		XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);
		if (!XMVector3Greater(XMVector3LengthSq(v), xmvOne)) return(XMVector3Normalize(v));
	}
}


CGameObject::~CGameObject(void)
{
	if (m_pMesh) m_pMesh->Release();
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}

void CGameObject::SetPosition(XMFLOAT3& xmf3Position)
{
	m_xmf4x4World._41 = xmf3Position.x;
	m_xmf4x4World._42 = xmf3Position.y;
	m_xmf4x4World._43 = xmf3Position.z;
}

XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

XMFLOAT3 CGameObject::GetLook()
{
	XMFLOAT3 xmf3LookAt(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33);
	xmf3LookAt = Vector3::Normalize(xmf3LookAt);
	return(xmf3LookAt);
}

XMFLOAT3 CGameObject::GetUp()
{
	XMFLOAT3 xmf3Up(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23);
	xmf3Up = Vector3::Normalize(xmf3Up);
	return(xmf3Up);
}

XMFLOAT3 CGameObject::GetRight()
{
	XMFLOAT3 xmf3Right(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13);
	xmf3Right = Vector3::Normalize(xmf3Right);
	return(xmf3Right);
}

void CGameObject::SetRotationTransform(XMFLOAT4X4* pmxf4x4Transform)
{
	m_xmf4x4World._11 = pmxf4x4Transform->_11; m_xmf4x4World._12 = pmxf4x4Transform->_12; m_xmf4x4World._13 = pmxf4x4Transform->_13;
	m_xmf4x4World._21 = pmxf4x4Transform->_21; m_xmf4x4World._22 = pmxf4x4Transform->_22; m_xmf4x4World._23 = pmxf4x4Transform->_23;
	m_xmf4x4World._31 = pmxf4x4Transform->_31; m_xmf4x4World._32 = pmxf4x4Transform->_32; m_xmf4x4World._33 = pmxf4x4Transform->_33;
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Right, fDistance));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Up, fDistance));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3LookAt = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3LookAt, fDistance));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(fPitch, fYaw, fRoll);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, fAngle);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Move(XMFLOAT3& vDirection, float fSpeed)
{
	SetPosition(m_xmf4x4World._41 + vDirection.x * fSpeed, m_xmf4x4World._42 + vDirection.y * fSpeed, m_xmf4x4World._43 + vDirection.z * fSpeed);
}

void CGameObject::LookTo(XMFLOAT3& xmf3LookTo, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookToLH(GetPosition(), xmf3LookTo, xmf3Up);
	m_xmf4x4World._11 = xmf4x4View._11; m_xmf4x4World._12 = xmf4x4View._21; m_xmf4x4World._13 = xmf4x4View._31;
	m_xmf4x4World._21 = xmf4x4View._12; m_xmf4x4World._22 = xmf4x4View._22; m_xmf4x4World._23 = xmf4x4View._32;
	m_xmf4x4World._31 = xmf4x4View._13; m_xmf4x4World._32 = xmf4x4View._23; m_xmf4x4World._33 = xmf4x4View._33;
}

void CGameObject::LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(GetPosition(), xmf3LookAt, xmf3Up);
	m_xmf4x4World._11 = xmf4x4View._11; m_xmf4x4World._12 = xmf4x4View._21; m_xmf4x4World._13 = xmf4x4View._31;
	m_xmf4x4World._21 = xmf4x4View._12; m_xmf4x4World._22 = xmf4x4View._22; m_xmf4x4World._23 = xmf4x4View._32;
	m_xmf4x4World._31 = xmf4x4View._13; m_xmf4x4World._32 = xmf4x4View._23; m_xmf4x4World._33 = xmf4x4View._33;
}

void CGameObject::UpdateBoundingBox()
{
	if (m_pMesh)
	{
		m_pMesh->m_xmOOBB.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
		XMStoreFloat4(&m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBB.Orientation)));
	}
}

void CGameObject::Animate(float fElapsedTime)
{
	if (m_fRotationSpeed != 0.0f) Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
	if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);

	UpdateBoundingBox();
}

void CGameObject::Render(HDC hDCFrameBuffer, XMFLOAT4X4* pxmf4x4World, CMesh* pMesh)
{
	if (pMesh)
	{
		CGraphicsPipeline::SetWorldTransform(pxmf4x4World);

		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		pMesh->Render(hDCFrameBuffer);
		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

void CGameObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	if (pCamera->IsInFrustum(m_xmOOBB)) CGameObject::Render(hDCFrameBuffer, &m_xmf4x4World, m_pMesh);
}

void CGameObject::GenerateRayForPicking(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection)
{
	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_xmf4x4World) * xmmtxView);

	XMFLOAT3 xmf3CameraOrigin(0.0f, 0.0f, 0.0f);
	xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
	xmvPickRayDirection = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
	xmvPickRayDirection = XMVector3Normalize(xmvPickRayDirection - xmvPickRayOrigin);
}

int CGameObject::PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance)
{
	int nIntersected = 0;
	if (m_pMesh)
	{
		XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
		GenerateRayForPicking(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
		nIntersected = m_pMesh->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);
	}
	return(nIntersected);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CWallsObject::CWallsObject()
{
}

CWallsObject::~CWallsObject()
{
}

void CWallsObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGameObject::Render(hDCFrameBuffer, &m_xmf4x4World, m_pMesh);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
XMFLOAT3 CExplosiveObject::m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
CMesh* CExplosiveObject::m_pExplosionMesh = NULL;

CExplosiveObject::CExplosiveObject()
{
}

CExplosiveObject::~CExplosiveObject()
{
}

void CExplosiveObject::PrepareExplosion()
{
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) XMStoreFloat3(&m_pxmf3SphereVectors[i], ::RandomUnitVectorOnSphere());

	m_pExplosionMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);
}

void CExplosiveObject::Animate(float fElapsedTime)
{
	if (m_bBlowingUp)
	{
		m_fElapsedTimes += fElapsedTime;
		if (m_fElapsedTimes <= m_fDuration)
		{
			XMFLOAT3 xmf3Position = GetPosition();
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				m_pxmf4x4Transforms[i] = Matrix4x4::Identity();
				m_pxmf4x4Transforms[i]._41 = xmf3Position.x + m_pxmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i]._42 = xmf3Position.y + m_pxmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i]._43 = xmf3Position.z + m_pxmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_pxmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_pxmf4x4Transforms[i]);
			}
		}
		else
		{
			m_bBlowingUp = false;
			m_fElapsedTimes = 0.0f;
		}
	}
	else
	{
		CGameObject::Animate(fElapsedTime);
	}
}

void CExplosiveObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	if (m_bBlowingUp)
	{
		for (int i = 0; i < EXPLOSION_DEBRISES; i++)
		{
			CGameObject::Render(hDCFrameBuffer , &m_pxmf4x4Transforms[i], m_pExplosionMesh);
		}
	}
	else
	{
		CGameObject::Render(hDCFrameBuffer, pCamera);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CBulletObject::CBulletObject(float fEffectiveRange)
{
	m_fBulletEffectiveRange = fEffectiveRange;
}

CBulletObject::~CBulletObject()
{
}

void CBulletObject::SetFirePosition(XMFLOAT3 xmf3FirePosition)
{
	m_xmf3FirePosition = xmf3FirePosition;
	SetPosition(xmf3FirePosition);
}

void CBulletObject::Reset()
{
	m_pLockedObject = NULL;
	m_fElapsedTimeAfterFire = 0;
	m_fMovingDistance = 0;
	m_fRotationAngle = 0.0f;

	m_bActive = false;
}

void CBulletObject::Animate(float fElapsedTime)
{
	m_fElapsedTimeAfterFire += fElapsedTime;

	float fDistance = m_fMovingSpeed * fElapsedTime;

	if ((m_fElapsedTimeAfterFire > m_fLockingDelayTime) && m_pLockedObject)
	{
		XMFLOAT3 xmf3Position = GetPosition();
		XMVECTOR xmvPosition = XMLoadFloat3(&xmf3Position);

		XMFLOAT3 xmf3LockedObjectPosition = m_pLockedObject->GetPosition();
		XMVECTOR xmvLockedObjectPosition = XMLoadFloat3(&xmf3LockedObjectPosition);
		XMVECTOR xmvToLockedObject = xmvLockedObjectPosition - xmvPosition;
		xmvToLockedObject = XMVector3Normalize(xmvToLockedObject);

		XMVECTOR xmvMovingDirection = XMLoadFloat3(&m_xmf3MovingDirection);
		xmvMovingDirection = XMVector3Normalize(XMVectorLerp(xmvMovingDirection, xmvToLockedObject, 0.25f));
		XMStoreFloat3(&m_xmf3MovingDirection, xmvMovingDirection);
	}
#ifdef _WITH_VECTOR_OPERATION
	XMFLOAT3 xmf3Position = GetPosition();

	m_fRotationAngle += m_fRotationSpeed * fElapsedTime;
	if (m_fRotationAngle > 360.0f) m_fRotationAngle = m_fRotationAngle - 360.0f;

	XMFLOAT4X4 mtxRotate1 = Matrix4x4::RotationYawPitchRoll(0.0f, m_fRotationAngle, 0.0f);

	XMFLOAT3 xmf3RotationAxis = Vector3::CrossProduct(m_xmf3RotationAxis, m_xmf3MovingDirection, true);
	float fDotProduct = Vector3::DotProduct(m_xmf3RotationAxis, m_xmf3MovingDirection);
	float fRotationAngle = ::IsEqual(fDotProduct, 1.0f) ? 0.0f : (float)XMConvertToDegrees(acos(fDotProduct));
	XMFLOAT4X4 mtxRotate2 = Matrix4x4::RotationAxis(xmf3RotationAxis, fRotationAngle);

	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate1, mtxRotate2);

	XMFLOAT3 xmf3Movement = Vector3::ScalarProduct(m_xmf3MovingDirection, fDistance, false);
	xmf3Position = Vector3::Add(xmf3Position, xmf3Movement);
	SetPosition(xmf3Position);
#else
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(0.0f, m_fRotationSpeed * fElapsedTime, 0.0f);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
	XMFLOAT3 xmf3Movement = Vector3::ScalarProduct(m_xmf3MovingDirection, fDistance, false);
	XMFLOAT3 xmf3Position = GetPosition();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Movement);
	SetPosition(xmf3Position);
	m_fMovingDistance += fDistance;
#endif

	UpdateBoundingBox();

	if ((m_fMovingDistance > m_fBulletEffectiveRange) || (m_fElapsedTimeAfterFire > m_fLockingTime)) Reset();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
void CAxisObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGraphicsPipeline::SetWorldTransform(&m_xmf4x4World);

	m_pMesh->Render(hDCFrameBuffer);
}

CRailObject::CRailObject() {
}

CRailObject::~CRailObject() {
}

void CRailObject::setRotationAngle(float degree) {
	m_dRotationAngle = degree;
}

void CRailObject::setRailType(int Type) {
	m_iRailType = Type;
}

void CRailObject::setRailSize(XMFLOAT3 Size) {
	m_xmfRailSize = Size;
}

//void CRailObject::MakeRail(CRailStraightMesh* pRailStraightMesh, const float& fRailWidth, const float& fRailHeight, const float& fRailDepth) {
//	switch (type) {
//	case 0:					// 평지
//		setRotationAngle(0);
//		SetMesh(pRailStraightMesh);
//		m_ppRailObject[j][i]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
//		switch (m_ppRailObject[j][i - 1]->m_iRailType) {
//		case 0:				// 평지 - 평지
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			}
//			//m_ppRailObject[j][i]->SetPosition(0, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth);
//			break;
//		case 1:				// 오르막 - 평지
//		case 2:				// 내리막 - 평지
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + 1)); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + 1), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + 1)); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + 1), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			}
//			break;
//		case 3:				// 우회전 - 평지
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 1.5); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 1.5); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
//			}
//			break;
//		case 4:				// 좌회전 - 평지
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 1.5); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 1.5); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
//			}
//			break;
//		}
//		switch (m_pPlayer->m_iRailHead) {
//		case 12:m_ppRailObject[j][i]->changeGoalPosition(0, 0, fRailDepth * 0.5); break;
//		case 3:m_ppRailObject[j][i]->changeGoalPosition(fRailDepth * 0.5, 0, 0); break;
//		case 6:m_ppRailObject[j][i]->changeGoalPosition(0, 0, -fRailDepth * 0.5); break;
//		case 9:m_ppRailObject[j][i]->changeGoalPosition(-fRailDepth * 0.5, 0, 0); break;
//		}
//		break;
//	case 1:					// 오르막
//		m_ppRailObject[j][i]->setRotationAngle(30);
//		m_ppRailObject[j][i]->SetMesh(pRailStraightMesh);
//		m_ppRailObject[j][i]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
//		switch (m_ppRailObject[j][i - 1]->m_iRailType) {
//		case 0:				// 평지 - 오르막
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			}
//			break;
//		case 1:				// 오르막 - 오르막
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			}
//			break;
//		case 2:				// 내리막 - 오르막
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			}
//			break;
//		case 3:				// 우회전 - 오르막
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
//			}
//			break;
//		case 4:				// 좌회전 - 오르막
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
//			}
//			break;
//		}
//		switch (m_pPlayer->m_iRailHead) {
//		case 12:m_ppRailObject[j][i]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//		case 3:m_ppRailObject[j][i]->changeGoalPosition(fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), 0); break;
//		case 6:m_ppRailObject[j][i]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), -fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//		case 9:m_ppRailObject[j][i]->changeGoalPosition(-fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), 0); break;
//		}
//		break;
//	case 2:					// 내리막
//		m_ppRailObject[j][i]->setRotationAngle(-30);
//		m_ppRailObject[j][i]->SetMesh(pRailStraightMesh);
//		m_ppRailObject[j][i]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
//		switch (m_ppRailObject[j][i - 1]->m_iRailType) {
//		case 0:				// 평지 - 내리막
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			}
//			///m_ppRailObject[j][i]->SetPosition(0.0f, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (1 + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))));
//			break;
//		case 1:				// 오르막 - 내리막
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			}
//			//m_ppRailObject[j][i]->SetPosition(0.0f, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))));
//			break;
//		case 2:				// 내리막 - 내리막
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)))); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().z); break;
//			}
//			//m_ppRailObject[j][i]->SetPosition(0.0f, m_ppRailObject[j][i - 1]->GetPosition().y + fRailWidth * 0.5 * (sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))));
//			break;
//		case 3:				// 우회전 - 내리막
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * +cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailDepth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
//			}
//			break;
//		case 4:				// 좌회전 - 내리막
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//			case 3:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5); break;
//			case 6:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//			case 9:m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5); break;
//			}
//			break;
//		}
//		switch (m_pPlayer->m_iRailHead) {
//		case 12:m_ppRailObject[j][i]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//		case 3:m_ppRailObject[j][i]->changeGoalPosition(fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), 0); break;
//		case 6:m_ppRailObject[j][i]->changeGoalPosition(0, fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), -fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle))); break;
//		case 9:m_ppRailObject[j][i]->changeGoalPosition(-fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i]->m_dRotationAngle)), 0); break;
//		}
//		break;
//	case 3:					// 우회전
//		m_ppRailObject[j][i]->setRotationAngle(0);
//		m_ppRailObject[j][i]->SetMesh(pRailCornerMesh);
//		m_ppRailObject[j][i]->RotateHead(m_pPlayer->m_iRailHead, TRUE);
//		switch (m_ppRailObject[j][i - 1]->m_iRailType) {
//		case 0:				// 평지 - 우회전
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth + fRailDepth * 0.5);
//				m_pPlayer->m_iRailHead = 3;
//				break;
//			case 3:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5);
//				m_pPlayer->m_iRailHead = 6;
//				break;
//			case 6:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 1.5);
//				m_pPlayer->m_iRailHead = 9;
//				break;
//			case 9:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5);
//				m_pPlayer->m_iRailHead = 12;
//				break;
//			}
//			break;
//		case 1:				// 오르막 - 우회전
//		case 2:				// 내리막 - 우회전
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + 2));
//				m_pPlayer->m_iRailHead = 3;
//				break;
//			case 3:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + fRailWidth, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5);
//				m_pPlayer->m_iRailHead = 6;
//				break;
//			case 6:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) - fRailDepth);
//				m_pPlayer->m_iRailHead = 9;
//				break;
//			case 9:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) - fRailWidth, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5);
//				m_pPlayer->m_iRailHead = 12;
//				break;
//			}
//			//m_ppRailObject[j][i]->SetPosition(0.0f, m_ppRailObject[j][i - 1]->GetPosition().y + fRailWidth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + 1));
//			break;
//		case 3:				// 우회전 - 우회전
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 2);
//				m_pPlayer->m_iRailHead = 3;
//				break;
//			case 3:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z);
//				m_pPlayer->m_iRailHead = 6;
//				break;
//			case 6:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 2);
//				m_pPlayer->m_iRailHead = 9;
//				break;
//			case 9:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z);
//				m_pPlayer->m_iRailHead = 12;
//				break;
//			}
//			break;
//		case 4:				// 좌회전 - 우회전
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 2);
//				m_pPlayer->m_iRailHead = 3;
//				break;
//			case 3:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth);
//				m_pPlayer->m_iRailHead = 6;
//				break;
//			case 6:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 2);
//				m_pPlayer->m_iRailHead = 9;
//				break;
//			case 9:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth);
//				m_pPlayer->m_iRailHead = 12;
//				break;
//			}
//			break;
//		}
//		switch (m_pPlayer->m_iRailHead) {
//		case 12:m_ppRailObject[j][i]->changeGoalPosition(-fRailDepth * 0.5, 0, fRailDepth); break;
//		case 3:m_ppRailObject[j][i]->changeGoalPosition(fRailDepth, 0, fRailDepth * 0.5); break;
//		case 6:m_ppRailObject[j][i]->changeGoalPosition(fRailDepth * 0.5, 0, -fRailDepth); break;
//		case 9:m_ppRailObject[j][i]->changeGoalPosition(-fRailDepth, 0, -fRailDepth * 0.5); break;
//		}
//		break;
//	case 4:					// 좌회전
//		m_ppRailObject[j][i]->setRotationAngle(0);
//		m_ppRailObject[j][i]->RotateHead(m_pPlayer->m_iRailHead, FALSE);
//		m_ppRailObject[j][i]->SetMesh(pRailCornerMesh);
//		switch (m_ppRailObject[j][i - 1]->m_iRailType) {
//		case 0:				// 평지 - 좌회전
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:// 문제
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 1.5);
//				m_pPlayer->m_iRailHead = 9;
//				break;
//			case 3:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 1.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5);
//				m_pPlayer->m_iRailHead = 12;
//				break;
//			case 6: // 문제
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 1.5);
//				m_pPlayer->m_iRailHead = 3;
//				break;
//			case 9:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 1.5, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5);
//				m_pPlayer->m_iRailHead = 6;
//				break;
//			}
//			break;
//		case 1:				// 오르막 - 좌회전
//		case 2:				// 내리막 - 좌회전
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + fRailDepth);
//				m_pPlayer->m_iRailHead = 9;
//				break;
//			case 3:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + fRailWidth, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5);
//				m_pPlayer->m_iRailHead = 12;
//				break;
//			case 6:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 0.5, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) - fRailDepth);
//				m_pPlayer->m_iRailHead = 3;
//				break;
//			case 9:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 0.5 * cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) - fRailWidth, m_ppRailObject[j][i - 1]->GetPosition().y + fRailDepth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 0.5);
//				m_pPlayer->m_iRailHead = 6;
//				break;
//			}
//			//m_ppRailObject[j][i]->SetPosition(0.0f, m_ppRailObject[j][i - 1]->GetPosition().y + fRailWidth * 0.5 * sin(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)), m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 0.5 * (cos(DegreeToRadian(m_ppRailObject[j][i - 1]->m_dRotationAngle)) + 1));
//			break;
//		case 3:				// 우회전 - 좌회전
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 2);
//				m_pPlayer->m_iRailHead = 9;
//				break;
//			case 3:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth);
//				m_pPlayer->m_iRailHead = 12;
//				break;
//			case 6:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 2);
//				m_pPlayer->m_iRailHead = 3;
//				break;
//			case 9:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth);
//				m_pPlayer->m_iRailHead = 6;
//				break;
//			}
//			break;
//		case 4:				// 좌회전 - 좌회전
//			switch (m_pPlayer->m_iRailHead) {
//			case 12:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z + fRailDepth * 2);
//				m_pPlayer->m_iRailHead = 9;
//				break;
//			case 3:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x + fRailWidth * 2, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z);
//				m_pPlayer->m_iRailHead = 12;
//				break;
//			case 6:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z - fRailDepth * 2);
//				m_pPlayer->m_iRailHead = 3;
//				break;
//			case 9:
//				m_ppRailObject[j][i]->SetPosition(m_ppRailObject[j][i - 1]->GetPosition().x - fRailWidth * 2, m_ppRailObject[j][i - 1]->GetPosition().y, m_ppRailObject[j][i - 1]->GetPosition().z);
//				m_pPlayer->m_iRailHead = 6;
//				break;
//			}
//			break;
//		}
//		switch (m_pPlayer->m_iRailHead) {
//		case 12:m_ppRailObject[j][i]->changeGoalPosition(fRailDepth * 0.5, 0, fRailDepth); break;
//		case 3:m_ppRailObject[j][i]->changeGoalPosition(fRailDepth, 0, -fRailDepth * 0.5); break;
//		case 6:m_ppRailObject[j][i]->changeGoalPosition(-fRailDepth * 0.5, 0, -fRailDepth); break;
//		case 9:m_ppRailObject[j][i]->changeGoalPosition(-fRailDepth, 0, fRailDepth * 0.5); break;
//		}
//		break;
//	}
//
//}

BOOL CRailObject::intersect(const XMFLOAT3& Position) {
	XMFLOAT3 RailCenter = this->GetPosition();
	if (Position.x >= RailCenter.x - m_xmfRailSize.x * 0.4 && Position.x <= RailCenter.x + m_xmfRailSize.x * 0.4
		&& Position.y >= RailCenter.y - m_xmfRailSize.y * 0.4 && Position.y <= RailCenter.y + m_xmfRailSize.y * 0.4
		&& Position.z >= RailCenter.z - m_xmfRailSize.z * 0.4 && Position.z <= RailCenter.z + m_xmfRailSize.z * 0.4) {
		return TRUE;
	}
	return FALSE;
}

XMFLOAT3 CRailObject::getGoalPosition() {
	return m_xmfGoalPosition;
}

void CRailObject::setGoalPosition(const float& x, const float& y, const float& z) {
	m_xmfGoalPosition.x = x;
	m_xmfGoalPosition.y = y;
	m_xmfGoalPosition.z = z;
}

void CRailObject::changeGoalPosition(const float& x, const float& y, const float& z) {
	m_xmfGoalPosition.x += x;
	m_xmfGoalPosition.y += y;
	m_xmfGoalPosition.z += z;
}

void CRailObject::SetPosition(float x, float y, float z) {
	CGameObject::SetPosition(x, y, z);
	setGoalPosition(x, y, z);
}
void CRailObject::Rotate() {
	CGameObject::Rotate(m_xmf3RotationAxis, m_dRotationAngle);
}

void CRailObject::RotateHead(int head, BOOL Right) {
	if (Right) {
		switch (head) {
		case 12:
			CGameObject::Rotate(XMFLOAT3(0, 1, 0), 0);
			break;
		case 3:
			CGameObject::Rotate(XMFLOAT3(0, 1, 0), 90);
			break;
		case 6:
			CGameObject::Rotate(XMFLOAT3(0, 1, 0), 180);
			break;
		case 9:
			CGameObject::Rotate(XMFLOAT3(0, 1, 0), 270);
			break;
		}
	}
	else {
		switch (head) {
		case 12:
			CGameObject::Rotate(XMFLOAT3(0, 1, 0), 90);
			break;
		case 3:
			CGameObject::Rotate(XMFLOAT3(0, 1, 0), 180);
			break;
		case 6:
			CGameObject::Rotate(XMFLOAT3(0, 1, 0), 270);
			break;
		case 9:
			CGameObject::Rotate(XMFLOAT3(0, 1, 0), 0);
			break;
		}
	}

}

void CRailObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGameObject::Render(hDCFrameBuffer, pCamera);
}
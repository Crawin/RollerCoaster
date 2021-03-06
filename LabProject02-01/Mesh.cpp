#include "stdafx.h"
#include "Mesh.h"
#include "GraphicsPipeline.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPolygon::CPolygon(int nVertices)
{
	m_nVertices = nVertices;
	m_pVertices = new CVertex[nVertices];
}

CPolygon::~CPolygon()
{
	if (m_pVertices) delete[] m_pVertices;
}

void CPolygon::SetVertex(int nIndex, CVertex& vertex)
{
	if ((0 <= nIndex) && (nIndex < m_nVertices) && m_pVertices)
	{
		m_pVertices[nIndex] = vertex;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMesh::CMesh(int nPolygons)
{
	m_nPolygons = nPolygons;
	m_ppPolygons = new CPolygon*[nPolygons];
}

CMesh::~CMesh()
{
	if (m_ppPolygons)
	{
		for (int i = 0; i < m_nPolygons; i++) if (m_ppPolygons[i]) delete m_ppPolygons[i];
		delete[] m_ppPolygons;
	}
}

void CMesh::SetPolygon(int nIndex, CPolygon *pPolygon)
{
	if ((0 <= nIndex) && (nIndex < m_nPolygons)) m_ppPolygons[nIndex] = pPolygon;
}

void Draw2DLine(HDC hDCFrameBuffer, XMFLOAT3& f3PreviousProject, XMFLOAT3& f3CurrentProject)
{
	XMFLOAT3 f3Previous = CGraphicsPipeline::ScreenTransform(f3PreviousProject);
	XMFLOAT3 f3Current = CGraphicsPipeline::ScreenTransform(f3CurrentProject);
	::MoveToEx(hDCFrameBuffer, (long)f3Previous.x, (long)f3Previous.y, NULL);
	::LineTo(hDCFrameBuffer, (long)f3Current.x, (long)f3Current.y);
}

void CMesh::Render(HDC hDCFrameBuffer)
{
	XMFLOAT3 f3InitialProject, f3PreviousProject;
	bool bPreviousInside = false, bInitialInside = false, bCurrentInside = false, bIntersectInside = false;

	for (int j = 0; j < m_nPolygons; j++)
	{
		int nVertices = m_ppPolygons[j]->m_nVertices;
		CVertex* pVertices = m_ppPolygons[j]->m_pVertices;

		f3PreviousProject = f3InitialProject = CGraphicsPipeline::Project(pVertices[0].m_xmf3Position);
		bPreviousInside = bInitialInside = (-1.0f <= f3InitialProject.x) && (f3InitialProject.x <= 1.0f) && (-1.0f <= f3InitialProject.y) && (f3InitialProject.y <= 1.0f);
		for (int i = 1; i < nVertices; i++)
		{
			XMFLOAT3 f3CurrentProject = CGraphicsPipeline::Project(pVertices[i].m_xmf3Position);
			bCurrentInside = (-1.0f <= f3CurrentProject.x) && (f3CurrentProject.x <= 1.0f) && (-1.0f <= f3CurrentProject.y) && (f3CurrentProject.y <= 1.0f);
			if (((0.0f <= f3CurrentProject.z) && (f3CurrentProject.z <= 1.0f)) && ((bCurrentInside || bPreviousInside))) ::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3CurrentProject);
			f3PreviousProject = f3CurrentProject;
			bPreviousInside = bCurrentInside;
		}
		if (((0.0f <= f3InitialProject.z) && (f3InitialProject.z <= 1.0f)) && ((bInitialInside || bPreviousInside))) ::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3InitialProject);
	}
}

BOOL CMesh::RayIntersectionByTriangle(XMVECTOR& xmRayOrigin, XMVECTOR& xmRayDirection, XMVECTOR v0, XMVECTOR v1, XMVECTOR v2, float* pfNearHitDistance)
{
	float fHitDistance;
	BOOL bIntersected = TriangleTests::Intersects(xmRayOrigin, xmRayDirection, v0, v1, v2, fHitDistance);
	if (bIntersected && (fHitDistance < *pfNearHitDistance)) *pfNearHitDistance = fHitDistance;

	return(bIntersected);
}

int CMesh::CheckRayIntersection(XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection, float* pfNearHitDistance)
{
	int nIntersections = 0;
	bool bIntersected = m_xmOOBB.Intersects(xmvPickRayOrigin, xmvPickRayDirection, *pfNearHitDistance);
	if (bIntersected)
	{
		for (int i = 0; i < m_nPolygons; i++)
		{
			switch (m_ppPolygons[i]->m_nVertices)
			{
			case 3:
			{
				XMVECTOR v0 = XMLoadFloat3(&(m_ppPolygons[i]->m_pVertices[0].m_xmf3Position));
				XMVECTOR v1 = XMLoadFloat3(&(m_ppPolygons[i]->m_pVertices[1].m_xmf3Position));
				XMVECTOR v2 = XMLoadFloat3(&(m_ppPolygons[i]->m_pVertices[2].m_xmf3Position));
				BOOL bIntersected = RayIntersectionByTriangle(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, pfNearHitDistance);
				if (bIntersected) nIntersections++;
				break;
			}
			case 4:
			{
				XMVECTOR v0 = XMLoadFloat3(&(m_ppPolygons[i]->m_pVertices[0].m_xmf3Position));
				XMVECTOR v1 = XMLoadFloat3(&(m_ppPolygons[i]->m_pVertices[1].m_xmf3Position));
				XMVECTOR v2 = XMLoadFloat3(&(m_ppPolygons[i]->m_pVertices[2].m_xmf3Position));
				BOOL bIntersected = RayIntersectionByTriangle(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, pfNearHitDistance);
				if (bIntersected) nIntersections++;
				v0 = XMLoadFloat3(&(m_ppPolygons[i]->m_pVertices[0].m_xmf3Position));
				v1 = XMLoadFloat3(&(m_ppPolygons[i]->m_pVertices[2].m_xmf3Position));
				v2 = XMLoadFloat3(&(m_ppPolygons[i]->m_pVertices[3].m_xmf3Position));
				bIntersected = RayIntersectionByTriangle(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, pfNearHitDistance);
				if (bIntersected) nIntersections++;
				break;
			}
			}
		}
	}
	return(nIntersections);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CCubeMesh::CCubeMesh(float fWidth, float fHeight, float fDepth) : CMesh(6)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	CPolygon *pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(0, pFrontFace);

	CPolygon *pTopFace = new CPolygon(4);
	pTopFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pTopFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pTopFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pTopFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	SetPolygon(1, pTopFace);

	CPolygon *pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	SetPolygon(2, pBackFace);

	CPolygon *pBottomFace = new CPolygon(4);
	pBottomFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pBottomFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pBottomFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBottomFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	SetPolygon(3, pBottomFace);

	CPolygon *pLeftFace = new CPolygon(4);
	pLeftFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pLeftFace->SetVertex(1, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pLeftFace->SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pLeftFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	SetPolygon(4, pLeftFace);

	CPolygon *pRightFace = new CPolygon(4);
	pRightFace->SetVertex(0, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pRightFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pRightFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pRightFace->SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(5, pRightFace);

	m_xmOOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CWallMesh::CWallMesh(float fWidth, float fHeight, float fDepth, int nSubRects) : CMesh((4 * nSubRects * nSubRects) + 2)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;
	float fCellWidth = fWidth * (1.0f / nSubRects);
	float fCellHeight = fHeight * (1.0f / nSubRects);
	float fCellDepth = fDepth * (1.0f / nSubRects);

	int k = 0;
	CPolygon* pLeftFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pLeftFace = new CPolygon(4);
			pLeftFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight + (i * fCellHeight), -fHalfDepth + (j * fCellDepth)));
			pLeftFace->SetVertex(1, CVertex(-fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight), -fHalfDepth + (j * fCellDepth)));
			pLeftFace->SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight), -fHalfDepth + ((j + 1) * fCellDepth)));
			pLeftFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight + (i * fCellHeight), -fHalfDepth + ((j + 1) * fCellDepth)));
			SetPolygon(k++, pLeftFace);
		}
	}

	CPolygon* pRightFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pRightFace = new CPolygon(4);
			pRightFace->SetVertex(0, CVertex(+fHalfWidth, -fHalfHeight + (i * fCellHeight), -fHalfDepth + (j * fCellDepth)));
			pRightFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight), -fHalfDepth + (j * fCellDepth)));
			pRightFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight), -fHalfDepth + ((j + 1) * fCellDepth)));
			pRightFace->SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight + (i * fCellHeight), -fHalfDepth + ((j + 1) * fCellDepth)));
			SetPolygon(k++, pRightFace);
		}
	}

	CPolygon* pTopFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pTopFace = new CPolygon(4);
			pTopFace->SetVertex(0, CVertex(-fHalfWidth + (i * fCellWidth), +fHalfHeight, -fHalfDepth + (j * fCellDepth)));
			pTopFace->SetVertex(1, CVertex(-fHalfWidth + ((i + 1) * fCellWidth), +fHalfHeight, -fHalfDepth + (j * fCellDepth)));
			pTopFace->SetVertex(2, CVertex(-fHalfWidth + ((i + 1) * fCellWidth), +fHalfHeight, -fHalfDepth + ((j + 1) * fCellDepth)));
			pTopFace->SetVertex(3, CVertex(-fHalfWidth + (i * fCellWidth), +fHalfHeight, -fHalfDepth + ((j + 1) * fCellDepth)));
			SetPolygon(k++, pTopFace);
		}
	}

	CPolygon* pBottomFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pBottomFace = new CPolygon(4);
			pBottomFace->SetVertex(0, CVertex(-fHalfWidth + (i * fCellWidth), -fHalfHeight, -fHalfDepth + (j * fCellDepth)));
			pBottomFace->SetVertex(1, CVertex(-fHalfWidth + ((i + 1) * fCellWidth), -fHalfHeight, -fHalfDepth + (j * fCellDepth)));
			pBottomFace->SetVertex(2, CVertex(-fHalfWidth + ((i + 1) * fCellWidth), -fHalfHeight, -fHalfDepth + ((j + 1) * fCellDepth)));
			pBottomFace->SetVertex(3, CVertex(-fHalfWidth + (i * fCellWidth), -fHalfHeight, -fHalfDepth + ((j + 1) * fCellDepth)));
			SetPolygon(k++, pBottomFace);
		}
	}

	CPolygon* pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(k++, pFrontFace);

	CPolygon* pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	SetPolygon(k++, pBackFace);

	m_xmOOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CRailStraightMesh::CRailStraightMesh(float fWidth, float fHeight, float fDepth) : CMesh(6*5) {
	float fHalfWidth = fWidth * 0.5f, fHalfHeight = fHeight * 0.5f, fHalfDepth = fDepth * 0.5f;
	int k = 0;
	CVertex vertex_data[] = {
		CVertex(-fHalfWidth, fHalfHeight, fHalfDepth),			// 0
		CVertex(-fHalfWidth * 0.5, fHalfHeight, fHalfDepth),				// 1
		CVertex(-fHalfWidth * 0.5, fHalfHeight, -fHalfDepth),				// 2
		CVertex(-fHalfWidth, fHalfHeight, -fHalfDepth),			// 3
		CVertex(-fHalfWidth, -fHalfHeight, fHalfDepth),			// 4
		CVertex(-fHalfWidth * 0.5, -fHalfHeight, fHalfDepth),				// 5
		CVertex(-fHalfWidth * 0.5, -fHalfHeight, -fHalfDepth),				// 6
		CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth),		// 7

		CVertex(fHalfWidth * 0.5, fHalfHeight, fHalfDepth),		// 8
		CVertex(fHalfWidth, fHalfHeight, fHalfDepth),						// 9
		CVertex(fHalfWidth, fHalfHeight, -fHalfDepth),						// 10
		CVertex(fHalfWidth * 0.5, fHalfHeight, -fHalfDepth),	// 11
		CVertex(fHalfWidth * 0.5, -fHalfHeight, fHalfDepth),	// 12
		CVertex(fHalfWidth, -fHalfHeight, fHalfDepth),						// 13
		CVertex(fHalfWidth, -fHalfHeight, -fHalfDepth),						// 14
		CVertex(fHalfWidth * 0.5, -fHalfHeight, -fHalfDepth),	// 15

		CVertex(-fHalfWidth * 0.5,fHalfHeight,fHalfDepth / 6.0f * 5.0f),	// 16
		CVertex(fHalfWidth * 0.5,fHalfHeight,fHalfDepth / 6.0f * 5.0f),				// 17
		CVertex(fHalfWidth * 0.5,fHalfHeight,fHalfDepth * 0.5f),					// 18
		CVertex(-fHalfWidth * 0.5,fHalfHeight,fHalfDepth * 0.5f),			// 19
		CVertex(-fHalfWidth * 0.5,-fHalfHeight,fHalfDepth / 6.0f * 5.0f),	// 20
		CVertex(fHalfWidth * 0.5,-fHalfHeight,fHalfDepth / 6.0f * 5.0f),				// 21
		CVertex(fHalfWidth * 0.5,-fHalfHeight,fHalfDepth * 0.5f),					// 22
		CVertex(-fHalfWidth * 0.5,-fHalfHeight,fHalfDepth * 0.5f),			// 23

		CVertex(-fHalfWidth * 0.5,fHalfHeight,fHalfDepth / 6.0f),			// 24
		CVertex(fHalfWidth * 0.5,fHalfHeight,fHalfDepth / 6.0f),					// 25
		CVertex(fHalfWidth * 0.5,fHalfHeight,-fHalfDepth / 6.0f),					// 26
		CVertex(-fHalfWidth * 0.5,fHalfHeight,-fHalfDepth / 6.0f),			// 27
		CVertex(-fHalfWidth * 0.5,-fHalfHeight,fHalfDepth / 6.0f),			// 28
		CVertex(fHalfWidth * 0.5,-fHalfHeight,fHalfDepth / 6.0f),					// 29
		CVertex(fHalfWidth * 0.5,-fHalfHeight,-fHalfDepth / 6.0f),					// 30
		CVertex(-fHalfWidth * 0.5,-fHalfHeight,-fHalfDepth / 6.0f),			// 31

		CVertex(-fHalfWidth * 0.5,fHalfHeight,-fHalfDepth / 6.0f * 5.0f),	// 16
		CVertex(fHalfWidth * 0.5,fHalfHeight,-fHalfDepth / 6.0f * 5.0f),				// 17
		CVertex(fHalfWidth * 0.5,fHalfHeight,-fHalfDepth * 0.5f),					// 18
		CVertex(-fHalfWidth * 0.5,fHalfHeight,-fHalfDepth * 0.5f),			// 19
		CVertex(-fHalfWidth * 0.5,-fHalfHeight,-fHalfDepth / 6.0f * 5.0f),	// 20
		CVertex(fHalfWidth * 0.5,-fHalfHeight,-fHalfDepth / 6.0f * 5.0f),				// 21
		CVertex(fHalfWidth * 0.5,-fHalfHeight,-fHalfDepth * 0.5f),					// 22
		CVertex(-fHalfWidth * 0.5,-fHalfHeight,-fHalfDepth * 0.5f),			// 23
	};
	CPolygon* pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[0]);
	pLeftRailFace->SetVertex(1, vertex_data[1]);
	pLeftRailFace->SetVertex(2, vertex_data[2]);
	pLeftRailFace->SetVertex(3, vertex_data[3]);
	SetPolygon(k++, pLeftRailFace);

	pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[5]);
	pLeftRailFace->SetVertex(1, vertex_data[4]);
	pLeftRailFace->SetVertex(2, vertex_data[7]);
	pLeftRailFace->SetVertex(3, vertex_data[6]);
	SetPolygon(k++, pLeftRailFace);

	pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[3]);
	pLeftRailFace->SetVertex(1, vertex_data[2]);
	pLeftRailFace->SetVertex(2, vertex_data[6]);
	pLeftRailFace->SetVertex(3, vertex_data[7]);
	SetPolygon(k++, pLeftRailFace);

	pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[1]);
	pLeftRailFace->SetVertex(1, vertex_data[0]);
	pLeftRailFace->SetVertex(2, vertex_data[4]);
	pLeftRailFace->SetVertex(3, vertex_data[5]);
	SetPolygon(k++, pLeftRailFace);

	pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[0]);
	pLeftRailFace->SetVertex(1, vertex_data[3]);
	pLeftRailFace->SetVertex(2, vertex_data[7]);
	pLeftRailFace->SetVertex(3, vertex_data[4]);
	SetPolygon(k++, pLeftRailFace);

	pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[2]);
	pLeftRailFace->SetVertex(1, vertex_data[1]);
	pLeftRailFace->SetVertex(2, vertex_data[5]);
	pLeftRailFace->SetVertex(3, vertex_data[6]);
	SetPolygon(k++, pLeftRailFace);
	//------------------------------------------------
	CPolygon* pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[8]);
	pRightRailFace->SetVertex(1, vertex_data[9]);
	pRightRailFace->SetVertex(2, vertex_data[10]);
	pRightRailFace->SetVertex(3, vertex_data[11]);
	SetPolygon(k++, pRightRailFace);

	pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[13]);
	pRightRailFace->SetVertex(1, vertex_data[12]);
	pRightRailFace->SetVertex(2, vertex_data[15]);
	pRightRailFace->SetVertex(3, vertex_data[14]);
	SetPolygon(k++, pRightRailFace);

	pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[11]);
	pRightRailFace->SetVertex(1, vertex_data[10]);
	pRightRailFace->SetVertex(2, vertex_data[14]);
	pRightRailFace->SetVertex(3, vertex_data[15]);
	SetPolygon(k++, pRightRailFace);

	pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[9]);
	pRightRailFace->SetVertex(1, vertex_data[8]);
	pRightRailFace->SetVertex(2, vertex_data[12]);
	pRightRailFace->SetVertex(3, vertex_data[13]);
	SetPolygon(k++, pRightRailFace);

	pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[8]);
	pRightRailFace->SetVertex(1, vertex_data[11]);
	pRightRailFace->SetVertex(2, vertex_data[15]);
	pRightRailFace->SetVertex(3, vertex_data[12]);
	SetPolygon(k++, pRightRailFace);

	pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[10]);
	pRightRailFace->SetVertex(1, vertex_data[9]);
	pRightRailFace->SetVertex(2, vertex_data[13]);
	pRightRailFace->SetVertex(3, vertex_data[14]);
	SetPolygon(k++, pRightRailFace);
	//------------------------------------------------
	CPolygon* pCenterTopRailFace = new CPolygon(4);
	pCenterTopRailFace->SetVertex(0, vertex_data[16]);
	pCenterTopRailFace->SetVertex(1, vertex_data[17]);
	pCenterTopRailFace->SetVertex(2, vertex_data[18]);
	pCenterTopRailFace->SetVertex(3, vertex_data[19]);
	SetPolygon(k++, pCenterTopRailFace);

	pCenterTopRailFace = new CPolygon(4);
	pCenterTopRailFace->SetVertex(0, vertex_data[21]);
	pCenterTopRailFace->SetVertex(1, vertex_data[20]);
	pCenterTopRailFace->SetVertex(2, vertex_data[23]);
	pCenterTopRailFace->SetVertex(3, vertex_data[22]);
	SetPolygon(k++, pCenterTopRailFace);

	pCenterTopRailFace = new CPolygon(4);
	pCenterTopRailFace->SetVertex(0, vertex_data[19]);
	pCenterTopRailFace->SetVertex(1, vertex_data[18]);
	pCenterTopRailFace->SetVertex(2, vertex_data[22]);
	pCenterTopRailFace->SetVertex(3, vertex_data[23]);
	SetPolygon(k++, pCenterTopRailFace);

	pCenterTopRailFace = new CPolygon(4);
	pCenterTopRailFace->SetVertex(0, vertex_data[17]);
	pCenterTopRailFace->SetVertex(1, vertex_data[16]);
	pCenterTopRailFace->SetVertex(2, vertex_data[20]);
	pCenterTopRailFace->SetVertex(3, vertex_data[21]);
	SetPolygon(k++, pCenterTopRailFace);

	pCenterTopRailFace = new CPolygon(4);
	pCenterTopRailFace->SetVertex(0, vertex_data[16]);
	pCenterTopRailFace->SetVertex(1, vertex_data[19]);
	pCenterTopRailFace->SetVertex(2, vertex_data[23]);
	pCenterTopRailFace->SetVertex(3, vertex_data[20]);
	SetPolygon(k++, pCenterTopRailFace);

	pCenterTopRailFace = new CPolygon(4);
	pCenterTopRailFace->SetVertex(0, vertex_data[18]);
	pCenterTopRailFace->SetVertex(1, vertex_data[17]);
	pCenterTopRailFace->SetVertex(2, vertex_data[21]);
	pCenterTopRailFace->SetVertex(3, vertex_data[22]);
	SetPolygon(k++, pCenterTopRailFace);
	//------------------------------------------------
	CPolygon* pCenterCenRailFace = new CPolygon(4);
	pCenterCenRailFace->SetVertex(0, vertex_data[24]);
	pCenterCenRailFace->SetVertex(1, vertex_data[25]);
	pCenterCenRailFace->SetVertex(2, vertex_data[26]);
	pCenterCenRailFace->SetVertex(3, vertex_data[27]);
	SetPolygon(k++, pCenterCenRailFace);

	pCenterCenRailFace = new CPolygon(4);
	pCenterCenRailFace->SetVertex(0, vertex_data[29]);
	pCenterCenRailFace->SetVertex(1, vertex_data[28]);
	pCenterCenRailFace->SetVertex(2, vertex_data[31]);
	pCenterCenRailFace->SetVertex(3, vertex_data[30]);
	SetPolygon(k++, pCenterCenRailFace);

	pCenterCenRailFace = new CPolygon(4);
	pCenterCenRailFace->SetVertex(0, vertex_data[27]);
	pCenterCenRailFace->SetVertex(1, vertex_data[26]);
	pCenterCenRailFace->SetVertex(2, vertex_data[30]);
	pCenterCenRailFace->SetVertex(3, vertex_data[31]);
	SetPolygon(k++, pCenterCenRailFace);

	pCenterCenRailFace = new CPolygon(4);
	pCenterCenRailFace->SetVertex(0, vertex_data[25]);
	pCenterCenRailFace->SetVertex(1, vertex_data[24]);
	pCenterCenRailFace->SetVertex(2, vertex_data[28]);
	pCenterCenRailFace->SetVertex(3, vertex_data[29]);
	SetPolygon(k++, pCenterCenRailFace);

	pCenterCenRailFace = new CPolygon(4);
	pCenterCenRailFace->SetVertex(0, vertex_data[24]);
	pCenterCenRailFace->SetVertex(1, vertex_data[27]);
	pCenterCenRailFace->SetVertex(2, vertex_data[31]);
	pCenterCenRailFace->SetVertex(3, vertex_data[28]);
	SetPolygon(k++, pCenterCenRailFace);

	pCenterCenRailFace = new CPolygon(4);
	pCenterCenRailFace->SetVertex(0, vertex_data[26]);
	pCenterCenRailFace->SetVertex(1, vertex_data[25]);
	pCenterCenRailFace->SetVertex(2, vertex_data[29]);
	pCenterCenRailFace->SetVertex(3, vertex_data[30]);
	SetPolygon(k++, pCenterCenRailFace);
	//------------------------------------------------
	CPolygon* pCenterBotRailFace = new CPolygon(4);
	pCenterBotRailFace->SetVertex(0, vertex_data[32]);
	pCenterBotRailFace->SetVertex(1, vertex_data[33]);
	pCenterBotRailFace->SetVertex(2, vertex_data[34]);
	pCenterBotRailFace->SetVertex(3, vertex_data[35]);
	SetPolygon(k++, pCenterBotRailFace);

	pCenterBotRailFace = new CPolygon(4);
	pCenterBotRailFace->SetVertex(0, vertex_data[37]);
	pCenterBotRailFace->SetVertex(1, vertex_data[36]);
	pCenterBotRailFace->SetVertex(2, vertex_data[39]);
	pCenterBotRailFace->SetVertex(3, vertex_data[38]);
	SetPolygon(k++, pCenterBotRailFace);

	pCenterBotRailFace = new CPolygon(4);
	pCenterBotRailFace->SetVertex(0, vertex_data[35]);
	pCenterBotRailFace->SetVertex(1, vertex_data[34]);
	pCenterBotRailFace->SetVertex(2, vertex_data[38]);
	pCenterBotRailFace->SetVertex(3, vertex_data[39]);
	SetPolygon(k++, pCenterBotRailFace);

	pCenterBotRailFace = new CPolygon(4);
	pCenterBotRailFace->SetVertex(0, vertex_data[33]);
	pCenterBotRailFace->SetVertex(1, vertex_data[32]);
	pCenterBotRailFace->SetVertex(2, vertex_data[36]);
	pCenterBotRailFace->SetVertex(3, vertex_data[37]);
	SetPolygon(k++, pCenterBotRailFace);

	pCenterBotRailFace = new CPolygon(4);
	pCenterBotRailFace->SetVertex(0, vertex_data[32]);
	pCenterBotRailFace->SetVertex(1, vertex_data[35]);
	pCenterBotRailFace->SetVertex(2, vertex_data[39]);
	pCenterBotRailFace->SetVertex(3, vertex_data[36]);
	SetPolygon(k++, pCenterBotRailFace);

	pCenterBotRailFace = new CPolygon(4);
	pCenterBotRailFace->SetVertex(0, vertex_data[34]);
	pCenterBotRailFace->SetVertex(1, vertex_data[33]);
	pCenterBotRailFace->SetVertex(2, vertex_data[37]);
	pCenterBotRailFace->SetVertex(3, vertex_data[38]);
	SetPolygon(k++, pCenterBotRailFace);

	m_xmOOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CRailCornerMesh::CRailCornerMesh(float fWidth, float fHeight, float fDepth) : CMesh(6 * 2) {
	float fHalfWidth = fWidth * 0.5f, fHalfHeight = fHeight * 0.5f, fHalfDepth = fDepth * 0.5f;
	int k = 0;
	CVertex vertex_data[] = {
		CVertex(-fHalfWidth, fHalfHeight, -fHalfDepth),	// 0
		CVertex(fHalfWidth, fHalfHeight, fHalfDepth),	// 1
		CVertex(fHalfWidth, fHalfHeight, fHalfDepth * 3 / 4),	// 2
		CVertex(-fHalfWidth * 3 / 4, fHalfHeight, -fHalfDepth),	// 3

		CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth),	// 4
		CVertex(fHalfWidth, -fHalfHeight, fHalfDepth),	// 5
		CVertex(fHalfWidth, -fHalfHeight, fHalfDepth * 3 / 4),	// 6
		CVertex(-fHalfWidth * 3 / 4, -fHalfHeight, -fHalfDepth),	// 7

		CVertex(-fHalfWidth / 4, fHalfHeight, -fHalfDepth),	// 8
		CVertex(fHalfWidth, fHalfHeight, fHalfDepth / 4),	// 9
		CVertex(fHalfWidth, fHalfHeight, 0),	// 10
		CVertex(0, fHalfHeight, -fHalfDepth),	// 11

		CVertex(-fHalfWidth / 4, -fHalfHeight, -fHalfDepth),	// 12
		CVertex(fHalfWidth, -fHalfHeight, fHalfDepth / 4),	// 13
		CVertex(fHalfWidth, -fHalfHeight, 0),	// 14
		CVertex(0, -fHalfHeight, -fHalfDepth),	// 15

		CVertex(fWidth*13/96, fHalfHeight, fDepth /96),	// 16
		CVertex(fWidth*25/96, fHalfHeight, -fDepth/8),	// 17
		CVertex(fWidth*23/192, fHalfHeight, -fDepth*51/192),	// 18
		CVertex(-fWidth/96, fHalfHeight, -fDepth * 13 / 96),	// 19

		CVertex(fWidth * 13 / 96, -fHalfHeight, fDepth / 96),	// 20
		CVertex(fWidth * 25 / 96, -fHalfHeight, -fDepth / 8),	// 21
		CVertex(fWidth * 23 / 192, -fHalfHeight, -fDepth * 51 / 192),	// 22
		CVertex(-fWidth / 96, -fHalfHeight, -fDepth * 13 / 96),	// 23
	};
	CPolygon* pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[0]);
	pLeftRailFace->SetVertex(1, vertex_data[1]);
	pLeftRailFace->SetVertex(2, vertex_data[2]);
	pLeftRailFace->SetVertex(3, vertex_data[3]);
	SetPolygon(k++, pLeftRailFace);

	pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[3]);
	pLeftRailFace->SetVertex(1, vertex_data[2]);
	pLeftRailFace->SetVertex(2, vertex_data[6]);
	pLeftRailFace->SetVertex(3, vertex_data[7]);
	SetPolygon(k++, pLeftRailFace);

	pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[0]);
	pLeftRailFace->SetVertex(1, vertex_data[3]);
	pLeftRailFace->SetVertex(2, vertex_data[7]);
	pLeftRailFace->SetVertex(3, vertex_data[4]);
	SetPolygon(k++, pLeftRailFace);

	pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[2]);
	pLeftRailFace->SetVertex(1, vertex_data[1]);
	pLeftRailFace->SetVertex(2, vertex_data[5]);
	pLeftRailFace->SetVertex(3, vertex_data[6]);
	SetPolygon(k++, pLeftRailFace);

	pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[0]);
	pLeftRailFace->SetVertex(1, vertex_data[1]);
	pLeftRailFace->SetVertex(2, vertex_data[5]);
	pLeftRailFace->SetVertex(3, vertex_data[4]);
	SetPolygon(k++, pLeftRailFace);

	pLeftRailFace = new CPolygon(4);
	pLeftRailFace->SetVertex(0, vertex_data[6]);
	pLeftRailFace->SetVertex(1, vertex_data[5]);
	pLeftRailFace->SetVertex(2, vertex_data[4]);
	pLeftRailFace->SetVertex(3, vertex_data[7]);
	SetPolygon(k++, pLeftRailFace);
	//------------------------------------------------
	CPolygon* pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[8]);
	pRightRailFace->SetVertex(1, vertex_data[9]);
	pRightRailFace->SetVertex(2, vertex_data[10]);
	pRightRailFace->SetVertex(3, vertex_data[11]);
	SetPolygon(k++, pRightRailFace);

	pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[11]);
	pRightRailFace->SetVertex(1, vertex_data[10]);
	pRightRailFace->SetVertex(2, vertex_data[14]);
	pRightRailFace->SetVertex(3, vertex_data[15]);
	SetPolygon(k++, pRightRailFace);

	pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[8]);
	pRightRailFace->SetVertex(1, vertex_data[11]);
	pRightRailFace->SetVertex(2, vertex_data[15]);
	pRightRailFace->SetVertex(3, vertex_data[12]);
	SetPolygon(k++, pRightRailFace);

	pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[10]);
	pRightRailFace->SetVertex(1, vertex_data[9]);
	pRightRailFace->SetVertex(2, vertex_data[13]);
	pRightRailFace->SetVertex(3, vertex_data[14]);
	SetPolygon(k++, pRightRailFace);

	pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[8]);
	pRightRailFace->SetVertex(1, vertex_data[9]);
	pRightRailFace->SetVertex(2, vertex_data[13]);
	pRightRailFace->SetVertex(3, vertex_data[12]);
	SetPolygon(k++, pRightRailFace);

	pRightRailFace = new CPolygon(4);
	pRightRailFace->SetVertex(0, vertex_data[14]);
	pRightRailFace->SetVertex(1, vertex_data[13]);
	pRightRailFace->SetVertex(2, vertex_data[12]);
	pRightRailFace->SetVertex(3, vertex_data[15]);
	SetPolygon(k++, pRightRailFace);

	m_xmOOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAirplaneMesh::CAirplaneMesh(float fWidth, float fHeight, float fDepth) : CMesh(24) // ???? ???? ?????? 30 ???????? 24
{
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1)*x2 + (fy - y3);
	int i = 0;

	//Upper Plane
	CPolygon *pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Lower Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	//Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	//Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	// ------- boundingOreintedBox ??????
	/*pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-fx, +fy, -fz));
	pFace->SetVertex(1, CVertex(+fx, +fy, -fz));
	pFace->SetVertex(2, CVertex(+fx, -fy, -fz));
	pFace->SetVertex(3, CVertex(-fx, -fy, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fy, +fz));
	pFace->SetVertex(1, CVertex(+fx, +fy, +fz));
	pFace->SetVertex(2, CVertex(+fx, +fy, -fz));
	pFace->SetVertex(3, CVertex(-fx, +fy, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, -fy, +fz));
	pFace->SetVertex(1, CVertex(+fx, -fy, +fz));
	pFace->SetVertex(2, CVertex(+fx, +fy, +fz));
	pFace->SetVertex(3, CVertex(-fx, +fy, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, -fy, -fz));
	pFace->SetVertex(1, CVertex(+fx, -fy, -fz));
	pFace->SetVertex(2, CVertex(+fx, -fy, +fz));
	pFace->SetVertex(3, CVertex(-fx, -fy, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fy, +fz));
	pFace->SetVertex(1, CVertex(-fx, +fy, -fz));
	pFace->SetVertex(2, CVertex(-fx, -fy, -fz));
	pFace->SetVertex(3, CVertex(-fx, -fy, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(+fx, +fy, -fz));
	pFace->SetVertex(1, CVertex(+fx, +fy, +fz));
	pFace->SetVertex(2, CVertex(+fx, -fy, +fz));
	pFace->SetVertex(3, CVertex(+fx, -fy, -fz));
	SetPolygon(i++, pFace);*/

	m_xmOOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fx, fy, fz), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CAxisMesh::CAxisMesh(float fWidth, float fHeight, float fDepth) : CMesh(3)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	CPolygon* pxAxis = new CPolygon(2);
	pxAxis->SetVertex(0, CVertex(-fHalfWidth, 0.0f, 0.0f));
	pxAxis->SetVertex(1, CVertex(+fHalfWidth, 0.0f, 0.0f));
	SetPolygon(0, pxAxis);

	CPolygon* pyAxis = new CPolygon(2);
	pyAxis->SetVertex(0, CVertex(0.0f, -fHalfWidth, 0.0f));
	pyAxis->SetVertex(1, CVertex(0.0f, +fHalfWidth, 0.0f));
	SetPolygon(1, pyAxis);

	CPolygon* pzAxis = new CPolygon(2);
	pzAxis->SetVertex(0, CVertex(0.0f, 0.0f, -fHalfWidth));
	pzAxis->SetVertex(1, CVertex(0.0f, 0.0f, +fHalfWidth));
	SetPolygon(2, pzAxis);
}

void CAxisMesh::Render(HDC hDCFrameBuffer)
{
	XMFLOAT3 f3PreviousProject = CGraphicsPipeline::Project(m_ppPolygons[0]->m_pVertices[0].m_xmf3Position);
	XMFLOAT3 f3CurrentProject = CGraphicsPipeline::Project(m_ppPolygons[0]->m_pVertices[1].m_xmf3Position);
	HPEN hPen = ::CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
	::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3CurrentProject);
	::SelectObject(hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);

	f3PreviousProject = CGraphicsPipeline::Project(m_ppPolygons[1]->m_pVertices[0].m_xmf3Position);
	f3CurrentProject = CGraphicsPipeline::Project(m_ppPolygons[1]->m_pVertices[1].m_xmf3Position);
	hPen = ::CreatePen(PS_SOLID, 0, RGB(0, 0, 255));
	hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
	::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3CurrentProject);
	::SelectObject(hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);

	f3PreviousProject = CGraphicsPipeline::Project(m_ppPolygons[2]->m_pVertices[0].m_xmf3Position);
	f3CurrentProject = CGraphicsPipeline::Project(m_ppPolygons[2]->m_pVertices[1].m_xmf3Position);
	hPen = ::CreatePen(PS_SOLID, 0, RGB(0, 255, 0));
	hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
	::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3CurrentProject);
	::SelectObject(hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
}

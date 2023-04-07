#include "stdafx.h"
#include "Mesh.h"
#include "GraphicsPipeline.h"

CPolygon::CPolygon(int nVertices)
{
	m_nVertices = nVertices;
	m_vVertices.resize(nVertices);
}

CPolygon::~CPolygon()
{
	m_vVertices.clear();
}

void CPolygon::SetVertex(int nIndex, CVertex vertex)
{
	if ((0 <= nIndex) && (nIndex < m_nVertices) && !m_vVertices.empty()) {
		m_vVertices[nIndex] = vertex;
	}
}

CMesh::CMesh(int nPolygons)
{
	m_nPolygons = nPolygons;
	m_vvPolygons.resize(nPolygons);
}

CMesh::~CMesh()
{
	m_vvPolygons.clear();
}

void CMesh::SetPolygon(int nIndex, CPolygon pPolygon)
{
	if ((0 <= nIndex) && (nIndex < m_nPolygons)) {
		m_vvPolygons[nIndex] = pPolygon;
	}
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
	XMFLOAT3 f3InitailProject, f3PreviousProject;
	bool bPreviousInside = false, bInitailInside = false,
		bCurrentInside = false, bIntersectInside = false;

	for (int j = 0; j < m_nPolygons; j++) {
		int nVertices = m_vvPolygons[j].m_nVertices;
		std::vector<CVertex> vVertices = m_vvPolygons[j].m_vVertices;

		f3PreviousProject = f3InitailProject =
			CGraphicsPipeline::Project(vVertices[0].m_xmf3Position);
		bPreviousInside = bInitailInside =
			(-1.0f <= f3InitailProject.x) && (f3InitailProject.x <= 1.0f) &&
			(-1.0f <= f3InitailProject.y) && (f3InitailProject.y <= 1.0f);

		for (int i = 1; i < nVertices; i++) {
			XMFLOAT3 f3CurrentProject =
				CGraphicsPipeline::Project(vVertices[i].m_xmf3Position);
			bCurrentInside = (-1.0f <= f3CurrentProject.x) && (f3CurrentProject.x <= 1.0f) &&
				(-1.0f <= f3CurrentProject.y) && (f3CurrentProject.y <= 1.0f);
			if (((0.0f <= f3CurrentProject.z) && (f3CurrentProject.z <= 1.0f)) &&
				((bCurrentInside || bPreviousInside)))
				::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3CurrentProject);
			f3PreviousProject = f3CurrentProject;
			bPreviousInside = bCurrentInside;
		}
		if (((0.0f <= f3InitailProject.z) && (f3InitailProject.z <= 1.0f)) &&
			((bInitailInside || bPreviousInside)))
			::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3InitailProject);
	}

}

CCubeMesh::CCubeMesh(float fwidth, float fHeight, float fDepth) : CMesh(6)
{
	float fHalfWidth = fwidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	CPolygon pFrontFace{ 4 };
	pFrontFace.SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	pFrontFace.SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	pFrontFace.SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	pFrontFace.SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	SetPolygon(0, pFrontFace);

	CPolygon pTopFace{ 4 };
	pTopFace.SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	pTopFace.SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	pTopFace.SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	pTopFace.SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	SetPolygon(1, pTopFace);

	CPolygon pBackFace{ 4 };
	pBackFace.SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	pBackFace.SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	pBackFace.SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	pBackFace.SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	SetPolygon(2, pBackFace);

	CPolygon pBottomFace{ 4 };
	pBottomFace.SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	pBottomFace.SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	pBottomFace.SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	pBottomFace.SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	SetPolygon(3, pBottomFace);

	CPolygon pLeftFace{ 4 };
	pLeftFace.SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	pLeftFace.SetVertex(1, CVertex(-fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	pLeftFace.SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	pLeftFace.SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	SetPolygon(4, pLeftFace);

	CPolygon pRightFace{ 4 };
	pRightFace.SetVertex(0, CVertex(+fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	pRightFace.SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	pRightFace.SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	pRightFace.SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	SetPolygon(5, pRightFace);
}

CCubeMesh::~CCubeMesh() {}

CAirplaneMesh::CAirplaneMesh(float fWidth, float fHeight, float fDepth)
{
	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;
	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f,
		y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);
	int i = 0;

	CPolygon pFace{ 3 };
	pFace.SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace.SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace.SetVertex(2, CVertex(0.0f, 0.0f, -fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace.SetVertex(1, CVertex(0.0f, 0.0f, -fz));
	pFace.SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace.SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace.SetVertex(2, CVertex(+x1, -y1, -fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(-x2, +y2, -fz));
	pFace.SetVertex(1, CVertex(-x1, -y1, -fz));
	pFace.SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace.SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace.SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace.SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace.SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace.SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(+x2, +y2, +fz));
	pFace.SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace.SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace.SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace.SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace.SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace.SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace.SetVertex(2, CVertex(+x2, +y2, -fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace.SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace.SetVertex(2, CVertex(+x2, +y2, +fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace.SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace.SetVertex(2, CVertex(+fx, -y3, -fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(+fx, -y3, -fz));
	pFace.SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace.SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace.SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace.SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace.SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace.SetVertex(1, CVertex(+fx, -y3, +fz));
	pFace.SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace.SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace.SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace.SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace.SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	pFace.SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace.SetVertex(1, CVertex(0.0f, +(fy + y3), -fz));
	pFace.SetVertex(2, CVertex(-x2, +y2, -fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace.SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace.SetVertex(2, CVertex(-x2, +y2, +fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace.SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace.SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace.SetVertex(1, CVertex(-fx, -y3, -fz));
	pFace.SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace.SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace.SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace.SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace.SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace.SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace.SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace.SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);
	pFace.SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace.SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace.SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);
}
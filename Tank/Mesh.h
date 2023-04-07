#pragma once

//class CPoint3D {
//public:
//	CPoint3D() {}
//	CPoint3D(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
//	virtual ~CPoint3D() {}
//
//	float x = 0.0f;
//	float y = 0.0f;
//	float z = 0.0f;
//};

class CVertex {
public:
	CVertex() {}
	CVertex(float x, float y, float z) { m_xmf3Position = XMFLOAT3(x, y, z); }
	virtual ~CVertex() {}

	XMFLOAT3 m_xmf3Position;
};

class CPolygon {
public:
	CPolygon() {}
	CPolygon(int nVertices);
	virtual ~CPolygon();

	int m_nVertices = 0;
	std::vector<CVertex> m_vVertices{};

	void SetVertex(int nIndex, CVertex vertex);
};

class CMesh {
public:
	CMesh() {};
	CMesh(int nPolygons);
	virtual ~CMesh();

private:
	int m_nReferences = 1;

public:
	void AddRef() { m_nReferences++; }
	void Release() { m_nReferences--; if (m_nReferences <= 0) delete this; }

private:
	int m_nPolygons = 0;
	std::vector<CPolygon> m_vvPolygons{}; // 이걸 하는 의미가 있나? 그냥 std::vector<CPolygon> 써도 되는거 아냐?

public:
	void SetPolygon(int nIndex, CPolygon pPolygon);

	virtual void Render(HDC hDCFrameBuffer);
};

class CCubeMesh : public CMesh {
public:
	CCubeMesh(float fwidth = 4.0f, float fheight = 4.0f, float fdepth = 4.0f);
	virtual ~CCubeMesh();
};

class CAirplaneMesh : public CMesh {
public:
	CAirplaneMesh(float fWidth, float fHeight, float fDepth);
	virtual ~CAirplaneMesh() {}
};
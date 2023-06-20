#pragma once

class CGameObject {
public:
	CGameObject() {}
	~CGameObject();

public:
	bool m_bActive = true;

	// 다중 메쉬를 가질 수 있도록 변경 해야됨
	// std::shared_ptr<CMesh[]> ? 
	std::shared_ptr<CMesh> m_pMesh = nullptr;
	XMFLOAT4X4 m_xmf4x4World = Matrix4x4::Identity();

	DWORD m_dwColor = RGB(255, 0, 0);

	XMFLOAT3 m_xmf3MovingDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	float m_fMovingSpeed = 0.0f;
	float m_fMovingRange = 0.0f;

	XMFLOAT3 m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	float m_fRotationSpeed = 0.0f;

	// 형제나 자식 객체를 가질 수 있도록 포인터 추가 해야됨
	// 스마트 포인터로 표현하려면 어떻게 해야되나...
	CGameObject* m_pChild = nullptr;
	CGameObject* m_pSibling = nullptr;

public:
	void SetMesh(CMesh* pMesh) {
		m_pMesh = std::make_shared<CMesh>(pMesh);
		if (pMesh) pMesh->AddRef();
	}

	void SetAtive(bool bActive) { m_bActive = bActive; }
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3& xmf3Position);
	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection);
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	void SetRotationAxis(XMFLOAT3& xmf3MovingDirection);
	void SetRotaionSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }

	void Move(XMFLOAT3& vDirection, float fSpeed);

	void Rotate(float fPitch, float fYaw, float fRoll);
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);

	virtual void OnUpdateTransform() {}
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};


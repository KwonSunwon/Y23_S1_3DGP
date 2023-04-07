#include "stdafx.h"
#include "Camera.h"
#include "Player.h"

void CViewport::SetViewport(int nLeft, int nTop, int nWidth, int nHeight)
{
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

void CCamera::GenerateViewMatrix()
{
	XMVECTOR xmvLook = XMVector3Normalize(XMLoadFloat3(&m_xmf3Look));
	XMVECTOR xmvUp = XMVector3Normalize(XMLoadFloat3(&m_xmf3Up));
	XMVECTOR xmvRight = XMVector3Normalize(XMVector3Cross(xmvUp, xmvLook));
	xmvUp = XMVector3Normalize(XMVector3Cross(xmvLook, xmvRight));

	XMStoreFloat3(&m_xmf3Look, xmvLook);
	XMStoreFloat3(&m_xmf3Right, xmvRight);
	XMStoreFloat3(&m_xmf3Up, xmvUp);

	m_xmf4x4View._11 = m_xmf3Right.x;
	m_xmf4x4View._12 = m_xmf3Up.x;
	m_xmf4x4View._13 = m_xmf3Look.x;
	m_xmf4x4View._21 = m_xmf3Right.y;
	m_xmf4x4View._22 = m_xmf3Up.y;
	m_xmf4x4View._23 = m_xmf3Look.y;
	m_xmf4x4View._31 = m_xmf3Right.z;
	m_xmf4x4View._32 = m_xmf3Up.z;
	m_xmf4x4View._33 = m_xmf3Look.z;

	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	m_xmf4x4View._41 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvRight));
	m_xmf4x4View._42 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvUp));
	m_xmf4x4View._43 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvLook));

	XMStoreFloat4x4(&m_xmf4x4ViewProject,
		XMMatrixMultiply(XMLoadFloat4x4(&m_xmf4x4View), XMLoadFloat4x4(&m_xmf4x4Project)));
}

void CCamera::SetLookAt(XMFLOAT3& xmf3Position, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	m_xmf3Position = xmf3Position;
	XMStoreFloat4x4(&m_xmf4x4View,
		XMMatrixLookAtLH(XMLoadFloat3(&m_xmf3Position),
			XMLoadFloat3(&xmf3LookAt), XMLoadFloat3(&xmf3Up)));

	XMVECTORF32 xmf32vRight = {m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31, 0.0f};
	XMVECTORF32 xmf32vUp = {m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32, 0.0f};
	XMVECTORF32 xmf32vLook = {m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33, 0.0f};

	XMStoreFloat3(&m_xmf3Right, XMVector3Normalize(xmf32vRight));
	XMStoreFloat3(&m_xmf3Up, XMVector3Normalize(xmf32vUp));
	XMStoreFloat3(&m_xmf3Look, XMVector3Normalize(xmf32vLook));
}

void CCamera::SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	SetLookAt(m_xmf3Position, xmf3LookAt, xmf3Up);
}

void CCamera::SetViewport(int nLeft, int nTop, int nWidth, int nHeight)
{
	m_Viewport.SetViewport(nLeft, nTop, nWidth, nHeight);
	m_fAspectRatio = float(m_Viewport.m_nWidth) / float(m_Viewport.m_nHeight);
}

void CCamera::SetFOVAngle(float fFOVAngle)
{
	m_fFOVAngle = fFOVAngle;
	m_fProjectRectDistance = float(1.0f / tan(XMConvertToRadians(fFOVAngle * 0.5f)));
}

// CCamera::GeneratePerspectiveProjectionMatrix() 부터 만들면 됨
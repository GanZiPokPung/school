#include "StdAfx.h"
#include "GameObject.h"

CVector3 CVector3::Normalize(CVector3& vector)
{
	double fMagnitude = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	return(CVector3(float(vector.x / fMagnitude), float(vector.y / fMagnitude), float(vector.z / fMagnitude)));
}

float CVector3::DotProduct(CVector3& vector1, CVector3& vector2)
{
	return(vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z);
}

CVector3 CVector3::CrossProduct(CVector3& vector1, CVector3& vector2)
{
	return(CVector3(vector1.y * vector2.z - vector1.z * vector2.y, vector1.z * vector2.x - vector1.x * vector2.z, vector1.x * vector2.y - vector1.y * vector2.x));
}

CMatrix CMatrix::Multiply(CMatrix& mtxMatrix1, CMatrix& mtxMatrix2)
{
	CMatrix mtxResult;
	mtxResult._11 = mtxMatrix1._11 * mtxMatrix2._11 + mtxMatrix1._12 * mtxMatrix2._21 + mtxMatrix1._13 * mtxMatrix2._31 + mtxMatrix1._14 * mtxMatrix2._41;
	mtxResult._12 = mtxMatrix1._11 * mtxMatrix2._12 + mtxMatrix1._12 * mtxMatrix2._22 + mtxMatrix1._13 * mtxMatrix2._32 + mtxMatrix1._14 * mtxMatrix2._42;
	mtxResult._13 = mtxMatrix1._11 * mtxMatrix2._13 + mtxMatrix1._12 * mtxMatrix2._23 + mtxMatrix1._13 * mtxMatrix2._33 + mtxMatrix1._14 * mtxMatrix2._43;
	mtxResult._14 = mtxMatrix1._11 * mtxMatrix2._14 + mtxMatrix1._12 * mtxMatrix2._24 + mtxMatrix1._13 * mtxMatrix2._34 + mtxMatrix1._14 * mtxMatrix2._44;
	mtxResult._21 = mtxMatrix1._21 * mtxMatrix2._11 + mtxMatrix1._22 * mtxMatrix2._21 + mtxMatrix1._23 * mtxMatrix2._31 + mtxMatrix1._24 * mtxMatrix2._41;
	mtxResult._22 = mtxMatrix1._21 * mtxMatrix2._12 + mtxMatrix1._22 * mtxMatrix2._22 + mtxMatrix1._23 * mtxMatrix2._32 + mtxMatrix1._24 * mtxMatrix2._42;
	mtxResult._23 = mtxMatrix1._21 * mtxMatrix2._13 + mtxMatrix1._22 * mtxMatrix2._23 + mtxMatrix1._23 * mtxMatrix2._33 + mtxMatrix1._24 * mtxMatrix2._43;
	mtxResult._24 = mtxMatrix1._21 * mtxMatrix2._14 + mtxMatrix1._22 * mtxMatrix2._24 + mtxMatrix1._23 * mtxMatrix2._34 + mtxMatrix1._24 * mtxMatrix2._44;
	mtxResult._31 = mtxMatrix1._31 * mtxMatrix2._11 + mtxMatrix1._32 * mtxMatrix2._21 + mtxMatrix1._33 * mtxMatrix2._31 + mtxMatrix1._34 * mtxMatrix2._41;
	mtxResult._32 = mtxMatrix1._31 * mtxMatrix2._12 + mtxMatrix1._32 * mtxMatrix2._22 + mtxMatrix1._33 * mtxMatrix2._32 + mtxMatrix1._34 * mtxMatrix2._42;
	mtxResult._33 = mtxMatrix1._31 * mtxMatrix2._13 + mtxMatrix1._32 * mtxMatrix2._23 + mtxMatrix1._33 * mtxMatrix2._33 + mtxMatrix1._34 * mtxMatrix2._43;
	mtxResult._34 = mtxMatrix1._31 * mtxMatrix2._14 + mtxMatrix1._32 * mtxMatrix2._24 + mtxMatrix1._33 * mtxMatrix2._34 + mtxMatrix1._34 * mtxMatrix2._44;
	mtxResult._41 = mtxMatrix1._41 * mtxMatrix2._11 + mtxMatrix1._42 * mtxMatrix2._21 + mtxMatrix1._43 * mtxMatrix2._31 + mtxMatrix1._44 * mtxMatrix2._41;
	mtxResult._42 = mtxMatrix1._41 * mtxMatrix2._12 + mtxMatrix1._42 * mtxMatrix2._22 + mtxMatrix1._43 * mtxMatrix2._32 + mtxMatrix1._44 * mtxMatrix2._42;
	mtxResult._43 = mtxMatrix1._41 * mtxMatrix2._13 + mtxMatrix1._42 * mtxMatrix2._23 + mtxMatrix1._43 * mtxMatrix2._33 + mtxMatrix1._44 * mtxMatrix2._43;
	mtxResult._44 = mtxMatrix1._41 * mtxMatrix2._14 + mtxMatrix1._42 * mtxMatrix2._24 + mtxMatrix1._43 * mtxMatrix2._34 + mtxMatrix1._44 * mtxMatrix2._44;

	return(mtxResult);
}

CMatrix CMatrix::RotationYawPitchRoll(float fYaw, float fPitch, float fRoll)
{
	CMatrix mtxResult;
	if (fPitch)
	{
		mtxResult._22 = mtxResult._33 = float(cos(fPitch));
		mtxResult._23 = float(sin(fPitch));
		mtxResult._32 = -mtxResult._23;
	}
	if (fYaw)
	{
		CMatrix mtxYaw;
		mtxYaw._11 = mtxYaw._33 = float(cos(fYaw));
		mtxYaw._31 = float(sin(fYaw));
		mtxYaw._13 = -mtxYaw._31;
		mtxResult = CMatrix::Multiply(mtxYaw, mtxResult);
	}
	if (fRoll)
	{
		CMatrix mtxRoll;
		mtxRoll._11 = mtxRoll._22 = float(cos(fRoll));
		mtxRoll._12 = float(sin(fRoll));
		mtxRoll._21 = -mtxRoll._12;
		mtxResult = CMatrix::Multiply(mtxRoll, mtxResult);
	}

	return(mtxResult);
}

CMatrix CMatrix::RotationAxis(CVector3& vAxis, float fAngle)
{
	CMatrix mtxResult;
	double fCos = cos(fAngle), fSin = sin(fAngle);
	double fOneSubtractCos = 1 - fCos;
	mtxResult._11 = float(fCos + (vAxis.x * vAxis.x) * fOneSubtractCos);
	mtxResult._22 = float(fCos + (vAxis.y * vAxis.y) * fOneSubtractCos);
	mtxResult._33 = float(fCos + (vAxis.z * vAxis.z) * fOneSubtractCos);
	mtxResult._12 = float(vAxis.x * vAxis.y * fOneSubtractCos - vAxis.z * fSin);
	mtxResult._13 = float(vAxis.x * vAxis.z * fOneSubtractCos + vAxis.y * fSin);
	mtxResult._21 = float(vAxis.y * vAxis.z * fOneSubtractCos + vAxis.z * fSin);
	mtxResult._23 = float(vAxis.y * vAxis.z * fOneSubtractCos - vAxis.x * fSin);
	mtxResult._31 = float(vAxis.z * vAxis.x * fOneSubtractCos - vAxis.y * fSin);
	mtxResult._32 = float(vAxis.z * vAxis.y * fOneSubtractCos + vAxis.x * fSin);

	return(mtxResult);
}

CMatrix CMatrix::PerspectiveFovLH(float fovy, float r, float zn, float zf)
{
	CMatrix mtxResult;
	mtxResult._22 = 1.0f / float(tan(DegreeToRadian(fovy * 0.5f)));
	mtxResult._11 = mtxResult._22 / r;
	mtxResult._33 = zf / (zf - zn);
	mtxResult._43 = -zn*zf / (zf - zn);
	mtxResult._34 = 1.0f;
	mtxResult._44 = 0.0f;

	return(mtxResult);
}

CMatrix CMatrix::LookAtLH(CVector3& vCamera, CVector3& vLookAt, CVector3& vUp)
{
	CVector3 vLook = CVector3::Normalize(CVector3::Subtract(vLookAt, vCamera));
	CVector3 vRight = CVector3::Normalize(CVector3::CrossProduct(vUp, vLook));
	vUp = CVector3::Normalize(CVector3::CrossProduct(vLook, vRight));

	CMatrix mtxResult;
	mtxResult._11 = vRight.x; mtxResult._21 = vRight.y; mtxResult._31 = vRight.z;
	mtxResult._12 = vUp.x; mtxResult._22 = vUp.y; mtxResult._32 = vUp.z;
	mtxResult._13 = vLook.x; mtxResult._23 = vLook.y; mtxResult._33 = vLook.z;
	mtxResult._41 = -CVector3::DotProduct(vCamera, vRight);
	mtxResult._42 = -CVector3::DotProduct(vCamera, vUp);
	mtxResult._43 = -CVector3::DotProduct(vCamera, vLook);

	return(mtxResult);
}

CVector4 CMatrix::Transform(CVector3& vPosition, CMatrix& mtxTransform)
{
	CVector4 vResult;
	vResult.x = vPosition.x * mtxTransform._11 + vPosition.y * mtxTransform._21 + vPosition.z * mtxTransform._31 + mtxTransform._41;
	vResult.y = vPosition.x * mtxTransform._12 + vPosition.y * mtxTransform._22 + vPosition.z * mtxTransform._32 + mtxTransform._42;
	vResult.z = vPosition.x * mtxTransform._13 + vPosition.y * mtxTransform._23 + vPosition.z * mtxTransform._33 + mtxTransform._43;
	vResult.w = vPosition.x * mtxTransform._14 + vPosition.y * mtxTransform._24 + vPosition.z * mtxTransform._34 + mtxTransform._44;

	return(vResult);
}

CVector3 CMatrix::TransformCoord(CVector3& vPosition, CMatrix& mtxTransform)
{
	CVector3 vResult;
	vResult.x = vPosition.x * mtxTransform._11 + vPosition.y * mtxTransform._21 + vPosition.z * mtxTransform._31 + mtxTransform._41;
	vResult.y = vPosition.x * mtxTransform._12 + vPosition.y * mtxTransform._22 + vPosition.z * mtxTransform._32 + mtxTransform._42;
	vResult.z = vPosition.x * mtxTransform._13 + vPosition.y * mtxTransform._23 + vPosition.z * mtxTransform._33 + mtxTransform._43;

	return(vResult);
}

CVector3 CMatrix::TransformNormal(CVector3& vNormal, CMatrix& mtxTransform)
{
	CVector3 vResult;
	vResult.x = vNormal.x * mtxTransform._11 + vNormal.y * mtxTransform._21 + vNormal.z * mtxTransform._31;
	vResult.y = vNormal.x * mtxTransform._12 + vNormal.y * mtxTransform._22 + vNormal.z * mtxTransform._32;
	vResult.z = vNormal.x * mtxTransform._13 + vNormal.y * mtxTransform._23 + vNormal.z * mtxTransform._33;
	vResult = CVector3::Normalize(vResult);

	return(vResult);
}

CPolygon::CPolygon(int nVertices) 
{ 
	if (nVertices > 0)
	{
		m_nVertices = nVertices; 
		m_pVertices = new CVertex[nVertices]; 
	}
}

CPolygon::~CPolygon() 
{ 
	if (m_pVertices) delete [] m_pVertices; 
}

void CPolygon::SetVertex(int nIndex, CVertex vertex)
{
	if ((0 <= nIndex) && (nIndex < m_nVertices) && m_pVertices)
	{
		m_pVertices[nIndex] = vertex;
	}
}

void CPolygon::Draw(HDC hDCFrameBuffer, CMatrix& mtxWorldViewProjection, CCamera *pCamera)
{
    CVector4 vPrevious;
    for (int i = 0; i <= m_nVertices; i++) 
    {
		CVertex vModel = m_pVertices[i % m_nVertices];
//World/View/Projection Transformation(Perspective Divide)
		CVector4 vCurrent = CMatrix::Transform(vModel.m_vPosition, mtxWorldViewProjection);
		if (vCurrent.z != 0.0f) { vCurrent.x = vCurrent.x / vCurrent.w; vCurrent.y = vCurrent.y / vCurrent.w; vCurrent.z = vCurrent.z / vCurrent.w; }
//		if ((vCurrent.z < 0.0f) || (vCurrent.z > 1.0f)) continue;
//Screen Transformation
        vCurrent.x = +vCurrent.x * (pCamera->m_d3dViewport.m_nWidth * 0.5f) + pCamera->m_d3dViewport.m_xStart + (pCamera->m_d3dViewport.m_nWidth * 0.5f);
        vCurrent.y = -vCurrent.y * (pCamera->m_d3dViewport.m_nHeight * 0.5f) + pCamera->m_d3dViewport.m_yStart + (pCamera->m_d3dViewport.m_nHeight * 0.5f);

        if (i != 0) 
		{
			::MoveToEx(hDCFrameBuffer, (long)vPrevious.x, (long)vPrevious.y, NULL);
			::LineTo(hDCFrameBuffer, (long)vCurrent.x, (long)vCurrent.y);
		}
        vPrevious = vCurrent; 
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMesh::CMesh(int nPolygons)
{
	if (nPolygons > 0)
	{
		m_nPolygons = nPolygons;
		m_ppPolygons = new CPolygon * [nPolygons];
		m_nReferences = 0;
	}
}

CMesh::~CMesh(void)
{
	if (m_ppPolygons)
	{
		for (int i = 0; i < m_nPolygons; i++) if (m_ppPolygons[i]) delete m_ppPolygons[i];
		delete [] m_ppPolygons;
	}
}

void CMesh::SetPolygon(int nIndex, CPolygon *pPolygon)
{
	if ((0 <= nIndex) && (nIndex < m_nPolygons) && m_ppPolygons)
	{
		m_ppPolygons[nIndex] = pPolygon;
	}
}

void CMesh::Render(HDC hDCFrameBuffer, CMatrix& mtxWorldViewProjection, CCamera *pCamera)
{
    for (int j = 0; j < m_nPolygons; j++)
    {
        m_ppPolygons[j]->Draw(hDCFrameBuffer, mtxWorldViewProjection, pCamera);
    } 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CCubeMesh::CCubeMesh() : CMesh(6)
{
	CPolygon *pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-2, +2, -2));
	pFrontFace->SetVertex(1, CVertex(+2, +2, -2));
	pFrontFace->SetVertex(2, CVertex(+2, -2, -2));
	pFrontFace->SetVertex(3, CVertex(-2, -2, -2));
	SetPolygon(0, pFrontFace);

	CPolygon *pTopFace = new CPolygon(4);
	pTopFace->SetVertex(0, CVertex(-2, +2, +2));
	pTopFace->SetVertex(1, CVertex(+2, +2, +2));
	pTopFace->SetVertex(2, CVertex(+2, +2, -2));
	pTopFace->SetVertex(3, CVertex(-2, +2, -2));
	SetPolygon(1, pTopFace);

	CPolygon *pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-2, -2, +2));
	pBackFace->SetVertex(1, CVertex(+2, -2, +2));
	pBackFace->SetVertex(2, CVertex(+2, +2, +2));
	pBackFace->SetVertex(3, CVertex(-2, +2, +2));
	SetPolygon(2, pBackFace);

	CPolygon *pBottomFace = new CPolygon(4);
	pBottomFace->SetVertex(0, CVertex(-2, -2, -2));
	pBottomFace->SetVertex(1, CVertex(+2, -2, -2));
	pBottomFace->SetVertex(2, CVertex(+2, -2, +2));
	pBottomFace->SetVertex(3, CVertex(-2, -2, +2));
	SetPolygon(3, pBottomFace);

	CPolygon *pLeftFace = new CPolygon(4);
	pLeftFace->SetVertex(0, CVertex(-2, +2, +2));
	pLeftFace->SetVertex(1, CVertex(-2, +2, -2));
	pLeftFace->SetVertex(2, CVertex(-2, -2, -2));
	pLeftFace->SetVertex(3, CVertex(-2, -2, +2));
	SetPolygon(4, pLeftFace);

	CPolygon *pRightFace = new CPolygon(4);
	pRightFace->SetVertex(0, CVertex(+2, +2, -2));
	pRightFace->SetVertex(1, CVertex(+2, +2, +2));
	pRightFace->SetVertex(2, CVertex(+2, -2, +2));
	pRightFace->SetVertex(3, CVertex(+2, -2, -2));
	SetPolygon(5, pRightFace);
}

CCubeMesh::~CCubeMesh(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CCamera::CCamera()
{
	m_vPosition = CVector3(0.0f, 0.0f, 0.0f);
	m_vRight = CVector3(1.0f, 0.0f, 0.0f);
	m_vUp = CVector3(0.0f, 1.0f, 0.0f);
	m_vLook = CVector3(0.0f, 0.0f, 1.0f);

	CMatrix::Identity(m_mtxView);
	CMatrix::Identity(m_mtxProjection);
	CMatrix::Identity(m_mtxViewProject);

	m_d3dViewport.m_xStart = 0;
	m_d3dViewport.m_yStart = 0;
	m_d3dViewport.m_nWidth = 640;
	m_d3dViewport.m_nHeight = 480;
}

CCamera::~CCamera()
{
}

void CCamera::GenerateViewMatrix()
{
	m_vLook = CVector3::Normalize(m_vLook);
	m_vRight = CVector3::Normalize(CVector3::CrossProduct(m_vUp, m_vLook));
	m_vUp = CVector3::Normalize(CVector3::CrossProduct(m_vLook, m_vRight));

	m_mtxView._11 = m_vRight.x; m_mtxView._12 = m_vUp.x; m_mtxView._13 = m_vLook.x;
	m_mtxView._21 = m_vRight.y; m_mtxView._22 = m_vUp.y; m_mtxView._23 = m_vLook.y;
	m_mtxView._31 = m_vRight.z; m_mtxView._32 = m_vUp.z; m_mtxView._33 = m_vLook.z;
	m_mtxView._41 = -CVector3::DotProduct(m_vPosition, m_vRight);
	m_mtxView._42 = -CVector3::DotProduct(m_vPosition, m_vUp);
	m_mtxView._43 = -CVector3::DotProduct(m_vPosition, m_vLook);

	m_mtxViewProject = CMatrix::Multiply(m_mtxView, m_mtxProjection);
}

void CCamera::SetLookAt(CVector3& vPosition, CVector3& vLookAt, CVector3& vUp)
{
	m_vPosition = vPosition;

	m_mtxView = CMatrix::LookAtLH(vPosition, vLookAt, vUp);
	m_vRight = CVector3(m_mtxView._11, m_mtxView._21, m_mtxView._31);
	m_vUp = CVector3(m_mtxView._12, m_mtxView._22, m_mtxView._32);
	m_vLook = CVector3(m_mtxView._13, m_mtxView._23, m_mtxView._33);

	m_mtxViewProject = CMatrix::Multiply(m_mtxView, m_mtxProjection);
}

void CCamera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight)
{
	m_d3dViewport.m_xStart = xTopLeft;
	m_d3dViewport.m_yStart = yTopLeft;
	m_d3dViewport.m_nWidth = nWidth;
	m_d3dViewport.m_nHeight = nHeight;
}

void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	m_mtxProjection = CMatrix::PerspectiveFovLH(fFOVAngle, fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle)
{
	float fAspectRatio = (float(m_d3dViewport.m_nWidth) / float(m_d3dViewport.m_nHeight));
	m_mtxProjection = CMatrix::PerspectiveFovLH(fFOVAngle, fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

void CCamera::Move(CVector3& vShift)
{
	m_vPosition = CVector3::Add(m_vPosition, vShift);
	GenerateViewMatrix();
}

void CCamera::Move(float x, float y, float z)
{
	Move(CVector3(x, y, z));
}

void CCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	CMatrix mtxRotate;
	if (fPitch != 0.0f)
	{
		mtxRotate = CMatrix::RotationAxis(m_vRight, DegreeToRadian(fPitch));
		m_vLook = CMatrix::TransformNormal(m_vLook, mtxRotate);
		m_vUp = CMatrix::TransformNormal(m_vUp, mtxRotate);
	}
	if (fYaw != 0.0f)
	{
		mtxRotate = CMatrix::RotationAxis(m_vUp, DegreeToRadian(fYaw));
		m_vLook = CMatrix::TransformNormal(m_vLook, mtxRotate);
		m_vRight = CMatrix::TransformNormal(m_vRight, mtxRotate);
	}
	if (fRoll != 0.0f)
	{
		mtxRotate = CMatrix::RotationAxis(m_vLook, DegreeToRadian(fRoll));
		m_vUp = CMatrix::TransformNormal(m_vUp, mtxRotate);
		m_vRight = CMatrix::TransformNormal(m_vRight, mtxRotate);
	}

	GenerateViewMatrix();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CGameObject::CGameObject() 
{ 
	m_pMesh = NULL; 
	CMatrix::Identity(m_mtxWorld);

	m_dwColor = RGB(0, 0, 0);

	m_vMovingDirection = CVector3(0.0f, 0.0f, 1.0f);
	m_fMovingSpeed = 0.0f;
	m_fMovingRange = 0.0f;

	m_vRotationAxis = CVector3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 0.05f;
}

CGameObject::CGameObject(CMesh *pMesh) : CGameObject()
{
	m_pMesh = pMesh; 
}

CGameObject::~CGameObject(void)
{
	if (m_pMesh) m_pMesh->Release();
}

void CGameObject::SetPosition(float x, float y, float z) 
{
	m_mtxWorld._41 = x; 
	m_mtxWorld._42 = y; 
	m_mtxWorld._43 = z; 
}

void CGameObject::SetPosition(CVector3 vPosition) 
{ 
	m_mtxWorld._41 = vPosition.x; 
	m_mtxWorld._42 = vPosition.y; 
	m_mtxWorld._43 = vPosition.z; 
}

CVector3 CGameObject::GetPosition() 
{ 
	return(CVector3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43)); 
}

CVector3 CGameObject::GetRight()
{ 	
	return(CVector3::Normalize(CVector3(m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13)));
}

CVector3 CGameObject::GetUp() 
{ 	
	return(CVector3::Normalize(CVector3(m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23)));
}

CVector3 CGameObject::GetLook() 
{ 	
	return(CVector3::Normalize(CVector3(m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33)));
}

void CGameObject::MoveStrafe(float fDistance)
{
	CVector3 vPosition = GetPosition();
	CVector3 vRight = GetRight();
	CGameObject::SetPosition(CVector3::Add(vPosition, CVector3::ScalarProduct(vRight, fDistance)));
}

void CGameObject::MoveUp(float fDistance)
{
	CVector3 vPosition = GetPosition();
	CVector3 vUp = GetUp();
	CGameObject::SetPosition(CVector3::Add(vPosition, CVector3::ScalarProduct(vUp, fDistance)));
}

void CGameObject::MoveForward(float fDistance)
{
	CVector3 vPosition = GetPosition();
	CVector3 vLook = GetLook();
	CGameObject::SetPosition(CVector3::Add(vPosition, CVector3::ScalarProduct(vLook, fDistance)));
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	CMatrix mtxRotate = CMatrix::RotationYawPitchRoll(DegreeToRadian(fYaw), DegreeToRadian(fPitch), DegreeToRadian(fRoll));
	m_mtxWorld = CMatrix::Multiply(mtxRotate, m_mtxWorld);
}

void CGameObject::Rotate(CVector3& vRotationAxis, float fAngle)
{
	CMatrix mtxRotate = CMatrix::RotationAxis(vRotationAxis, DegreeToRadian(fAngle));
	m_mtxWorld = CMatrix::Multiply(mtxRotate, m_mtxWorld);
}

void CGameObject::Move(CVector3& vDirection, float fSpeed)
{
	SetPosition(m_mtxWorld._41 + vDirection.x * fSpeed, m_mtxWorld._42 + vDirection.y * fSpeed, m_mtxWorld._43 + vDirection.z * fSpeed);
}

void CGameObject::Animate()
{
	if (m_fRotationSpeed != 0.0f) Rotate(m_vRotationAxis, m_fRotationSpeed);
	if (m_fMovingSpeed != 0.0f)
	{
		float fDistance = CVector3::Length(GetPosition());
		if (fDistance >= m_fMovingRange) m_vMovingDirection = CVector3::ScalarProduct(m_vMovingDirection, -1.0f);
		Move(m_vMovingDirection, m_fMovingSpeed);
	}
}

void CGameObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	CMatrix mtxWorldViewProjection = CMatrix::Multiply(m_mtxWorld, pCamera->m_mtxViewProject);

	HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
	if (m_pMesh) m_pMesh->Render(hDCFrameBuffer, mtxWorldViewProjection, pCamera);
	::SelectObject(hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
}


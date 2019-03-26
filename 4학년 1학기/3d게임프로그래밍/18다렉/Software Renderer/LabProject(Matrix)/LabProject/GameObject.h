#pragma once

#define DegreeToRadian(x)	(float((x)*3.1415926535/180.0))

class CVector3
{
public:
	float x, y, z;

	CVector3() { x = y = z = 0.0f; }
	CVector3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

	float Length() { return((float)sqrt(x * x + y * y + z * z)); }
	static float Length(CVector3& vector) { return((float)sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z));  }
	static CVector3 Add(CVector3& vector1, CVector3& vector2) { return(CVector3(vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z)); }
	static CVector3 Subtract(CVector3& vector1, CVector3& vector2) { return(CVector3(vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z)); }
	static CVector3 Normalize(CVector3& vector);
	static CVector3 ScalarProduct(CVector3& vector, float fScalar) { return(CVector3(vector.x*fScalar, vector.y*fScalar, vector.z*fScalar)); }
	static float DotProduct(CVector3& vector1, CVector3& vector2);
	static CVector3 CrossProduct(CVector3& vector1, CVector3& vector2);
};

class CVector4
{
public:
	float x, y, z, w;

	CVector4() { x = y = z = 0.0f; w = 1.0f; }
	CVector4(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; }
};

class CMatrix
{
public:
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	CMatrix() { _11 = 1.0f; _12 = _13 = _14 = 0.0f; _22 = 1.0f; _21 = _23 = _24 = 0.0f; _33 = 1.0f; _31 = _32 = _34 = 0.0f; _44 = 1.0f; _41 = _42 = _43 = 0.0f; }

	static void Identity(CMatrix& mtxMatrix) { mtxMatrix._11 = 1.0f; mtxMatrix._12 = mtxMatrix._13 = mtxMatrix._14 = 0.0f; mtxMatrix._22 = 1.0f; mtxMatrix._21 = mtxMatrix._23 = mtxMatrix._24 = 0.0f; mtxMatrix._33 = 1.0f; mtxMatrix._31 = mtxMatrix._32 = mtxMatrix._34 = 0.0f; mtxMatrix._44 = 1.0f; mtxMatrix._41 = mtxMatrix._42 = mtxMatrix._43 = 0.0f; }
	static CMatrix Multiply(CMatrix& mtxMatrix1, CMatrix& mtxMatrix2);
	static CMatrix RotationYawPitchRoll(float fYaw, float fPitch, float fRoll);
	static CMatrix RotationAxis(CVector3& vAxis, float fAngle);
	static CMatrix PerspectiveFovLH(float fovy, float r, float zn, float zf);
	static CMatrix LookAtLH(CVector3& vPosition, CVector3& vLookAt, CVector3& vUp);
	static CVector4 Transform(CVector3& vPosition, CMatrix& mtxTransform);
	static CVector3 TransformCoord(CVector3& vPosition, CMatrix& mtxTransform);
	static CVector3 TransformNormal(CVector3& vNormal, CMatrix& mtxTransform);
};

struct VIEWPORT
{
	int						m_xStart;
	int						m_yStart;
	int						m_nWidth;
	int						m_nHeight;
};

class CCamera
{
public:
	CCamera();
	virtual ~CCamera();

	CVector3				m_vPosition;             
    CVector3				m_vRight;             
    CVector3				m_vUp;             
    CVector3				m_vLook;   

	CMatrix					m_mtxView;         	
    CMatrix					m_mtxProjection;   
	CMatrix					m_mtxViewProject;

	VIEWPORT				m_d3dViewport;

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle);
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);
	void SetLookAt(CVector3& vPosition, CVector3& vLookAt, CVector3& vUp=CVector3(0.0f, 1.0f, 0.0f));
	void SetViewport(int xStart, int yStart, int nWidth, int nHeight);

	void Move(CVector3& vShift);
    void Move(float x, float y, float z);
	void Rotate(float fPitch=0.0f, float fYaw=0.0f, float fRoll=0.0f);
};

class CVertex
{
public:
	CVertex() { }
    CVertex(float x, float y, float z) { m_vPosition = CVector3(x, y, z); }

    CVector3				m_vPosition;  
};

class CPolygon
{
public:
	CPolygon(int nVertices);
	virtual ~CPolygon();

    int						m_nVertices;     
    CVertex					*m_pVertices;   

	void SetVertex(int nIndex, CVertex vertex);
	void Draw(HDC hDCFrameBuffer, CMatrix& mtxWorldViewProjection, CCamera *pCamera);
};

class CMesh
{
public:
	CMesh(int nPolygons);
	virtual ~CMesh();

private:
	int						m_nReferences;

    int						m_nPolygons;     
    CPolygon				**m_ppPolygons;   

public:
	void AddRef() { m_nReferences++; }
	void Release() { m_nReferences--; if (m_nReferences <= 0) delete this; }

public:
	void SetPolygon(int nIndex, CPolygon *pPolygon);
	virtual void Render(HDC hDCFrameBuffer, CMatrix& mtxWorldViewProjection, CCamera *pCamera);
};

class CCubeMesh : public CMesh
{
public:
	CCubeMesh();
	virtual ~CCubeMesh();
};

class CGameObject
{
public:
	CGameObject(CMesh *pMesh);
	CGameObject();
	virtual ~CGameObject();

public:
    CMesh					*m_pMesh; 
    CMatrix					m_mtxWorld;             

	DWORD					m_dwColor;

	CVector3				m_vMovingDirection;
	float					m_fMovingSpeed;
	float					m_fMovingRange;

	CVector3				m_vRotationAxis;
	float					m_fRotationSpeed;

public:
	void SetMesh(CMesh *pMesh) { m_pMesh = pMesh; if (pMesh) pMesh->AddRef(); }
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	void SetPosition(float x, float y, float z);
	void SetPosition(CVector3 vPosition);

	void SetMovingDirection(CVector3 vMovingDirection) { m_vMovingDirection = CVector3::Normalize(vMovingDirection); }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	void SetRotationAxis(CVector3 vRotationAxis) { m_vRotationAxis = CVector3::Normalize(vRotationAxis); }
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }

	void MoveStrafe(float fDistance=1.0f);
	void MoveUp(float fDistance=1.0f);
	void MoveForward(float fDistance=1.0f);
	void Move(CVector3& vDirection, float fSpeed);

	void Rotate(float fPitch=10.0f, float fYaw=10.0f, float fRoll=10.0f);
	void Rotate(CVector3& vAxis, float fAngle);

	CVector3 GetPosition();
	CVector3 GetLook();
	CVector3 GetUp();
	CVector3 GetRight();

	virtual void Animate();
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};


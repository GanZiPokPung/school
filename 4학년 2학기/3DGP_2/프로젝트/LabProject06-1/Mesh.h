//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------

#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CVertex
{
protected:
    XMFLOAT3						m_xmf3Position;	

public:
	CVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); }
	CVertex(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
	~CVertex() { }
};

class CDiffusedVertex : public CVertex
{
protected:
    XMFLOAT4						m_xmf4Diffuse;		

public:
	CDiffusedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); }
	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; }
	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse; }
	~CDiffusedVertex() { }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMesh
{
public:
    CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
    virtual ~CMesh();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void ReleaseUploadBuffers();

protected:
	ID3D12Resource					*m_pd3dVertexBuffer = NULL;
	ID3D12Resource					*m_pd3dVertexUploadBuffer = NULL;

	ID3D12Resource					*m_pd3dIndexBuffer = NULL;
	ID3D12Resource					*m_pd3dIndexUploadBuffer = NULL;

	D3D12_VERTEX_BUFFER_VIEW		m_d3dVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW			m_d3dIndexBufferView;

	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT							m_nVertices = 0;
	CDiffusedVertex					*m_pVertices = NULL;

	UINT							m_nSlot = 0;
	UINT							m_nStride = 0;
	UINT							m_nOffset = 0;

	UINT							m_nIndices = 0;
	UINT							*m_pnIndices = NULL;

	UINT							m_nStartIndex = 0;
	UINT							m_nBaseVertex = 0;

#ifdef _WITH_OOBB_BOUNDING_BOX
protected:
	BoundingOrientedBox				m_xmBoundingBox;
public:
	BoundingOrientedBox GetBoundingBox() { return(m_xmBoundingBox); }
#else
protected:
	BoundingBox						m_xmBoundingBox;
public:
	BoundingBox GetBoundingBox() { return(m_xmBoundingBox); }
#endif

public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTriangleMeshDiffused : public CMesh
{
public:
    CTriangleMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
    virtual ~CTriangleMeshDiffused();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
};
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CCubeMeshDiffused : public CMesh
{
public:
	CCubeMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshDiffused();
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CSphereMeshDiffused : public CMesh
{
public:
	CSphereMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fRadius=2.0f, int nSlices=20, int nStacks=20);
	virtual ~CSphereMeshDiffused();
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CAirplaneMeshDiffused : public CMesh
{
public:
	CAirplaneMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f, XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CAirplaneMeshDiffused();
};

/*
/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CColliderMesh : public CMesh
{
public:
	CColliderMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, );
	virtual ~CColliderMesh();

	virtual void UpdateColliderMesh(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT3 *pxmf3Minimum, XMFLOAT3 *pxmf3Maximum) { }
};

class CCubeColliderMesh : public CColliderMesh
{
public:
	CCubeColliderMesh(ID3D11Device *pd3dDevice, float fWidth=2.0f, float fHeight=2.0f, float fDepth=2.0f, D3DXCOLOR d3dxColor=D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CCubeColliderMesh();

	virtual void UpdateColliderMesh(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT3 *pxmf3Minimum, XMFLOAT3 *pxmf3Maximum);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CSphereColliderMesh : public CColliderMesh
{
public:
	CSphereColliderMesh(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20, D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CSphereColliderMesh();
};

*/
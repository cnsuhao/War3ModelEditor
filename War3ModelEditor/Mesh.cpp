//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Mesh.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MESH::MESH()
{
	Mesh = NULL;

	VertexBufferLocked = FALSE;
	IndexBufferLocked = FALSE;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MESH::~MESH()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Creates a mesh
//+-----------------------------------------------------------------------------
BOOL MESH::Create(INT NrOfVertices, INT NrOfFaces)
{
	Clear();
	VERTEX Vertex;

	if(FAILED(D3DXCreateMeshFVF(NrOfFaces, NrOfVertices, D3DXMESH_MANAGED, Vertex.FORMAT, Graphics.GetDevice(), &Mesh)))
	{
		Error.SetMessage("Unable to create a mesh!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Clears the mesh
//+-----------------------------------------------------------------------------
VOID MESH::Clear()
{
	UnlockVertexBuffer();
	UnlockIndexBuffer();

	SAFE_RELEASE(Mesh);
}


//+-----------------------------------------------------------------------------
//| Returns the mesh
//+-----------------------------------------------------------------------------
LPD3DXMESH MESH::GetMesh() CONST
{
	return Mesh;
}


//+-----------------------------------------------------------------------------
//| Locks the vertex buffer
//+-----------------------------------------------------------------------------
BOOL MESH::LockVertexBuffer(VERTEX** VertexPointer)
{
	if(VertexBufferLocked) return TRUE;

	if(FAILED(Mesh->LockVertexBuffer(0, reinterpret_cast<VOID**>(VertexPointer))))
	{
		Error.SetMessage("Unable to lock the vertex buffer on the mesh!");
		return FALSE;
	}

	VertexBufferLocked = TRUE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Unlocks the vertex buffer
//+-----------------------------------------------------------------------------
VOID MESH::UnlockVertexBuffer()
{
	if(!VertexBufferLocked) return;

	Mesh->UnlockVertexBuffer();

	VertexBufferLocked = FALSE;
}


//+-----------------------------------------------------------------------------
//| Locks the index buffer
//+-----------------------------------------------------------------------------
BOOL MESH::LockIndexBuffer(WORD** IndexPointer)
{
	if(IndexBufferLocked) return TRUE;

	if(FAILED(Mesh->LockIndexBuffer(0, reinterpret_cast<VOID**>(IndexPointer))))
	{
		Error.SetMessage("Unable lock the index buffer on the mesh!");
		return FALSE;
	}

	IndexBufferLocked = TRUE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Unlocks the index buffer
//+-----------------------------------------------------------------------------
VOID MESH::UnlockIndexBuffer()
{
	if(!IndexBufferLocked) return;

	Mesh->UnlockIndexBuffer();

	IndexBufferLocked = FALSE;
}


//+-----------------------------------------------------------------------------
//| Checks if a ray intersects with the mesh
//+-----------------------------------------------------------------------------
BOOL MESH::RayIntersects(CONST D3DXVECTOR3& RayPosition, CONST D3DXVECTOR3& RayDirection, FLOAT* Distance)
{
	BOOL Intersects;

	if(FAILED(D3DXIntersect(Mesh, &RayPosition, &RayDirection, &Intersects, NULL, NULL, NULL, Distance, NULL, NULL))) return FALSE;

	return Intersects;
}


//+-----------------------------------------------------------------------------
//| Renders the mesh
//+-----------------------------------------------------------------------------
VOID MESH::Render()
{
	if(Mesh) Mesh->DrawSubset(0);
}

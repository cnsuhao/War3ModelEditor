//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MESH_H
#define MAGOS_MESH_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
struct VERTEX;


//+-----------------------------------------------------------------------------
//| Mesh class
//+-----------------------------------------------------------------------------
class MESH
{
	public:
		CONSTRUCTOR MESH();
		DESTRUCTOR ~MESH();

		BOOL Create(INT NrOfVertices, INT NrOfFaces);
		VOID Clear();

		LPD3DXMESH GetMesh() CONST;

		BOOL LockVertexBuffer(VERTEX** VertexPointer);
		VOID UnlockVertexBuffer();

		BOOL LockIndexBuffer(WORD** IndexPointer);
		VOID UnlockIndexBuffer();

		BOOL RayIntersects(CONST D3DXVECTOR3& RayPosition, CONST D3DXVECTOR3& RayDirection, FLOAT* Distance);

		VOID Render();

	protected:
		LPD3DXMESH Mesh;

		BOOL VertexBufferLocked;
		BOOL IndexBufferLocked;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Graphics.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

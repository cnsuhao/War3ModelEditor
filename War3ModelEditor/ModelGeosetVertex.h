//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_GEOSET_VERTEX_H
#define MAGOS_MODEL_GEOSET_VERTEX_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class MODEL_GEOSET_GROUP;
class MODEL_GEOSET_VERTEX;


//+-----------------------------------------------------------------------------
//| Model geoset vertex class
//+-----------------------------------------------------------------------------
class MODEL_GEOSET_VERTEX
{
	public:
		CONSTRUCTOR MODEL_GEOSET_VERTEX();
		DESTRUCTOR ~MODEL_GEOSET_VERTEX();

		VOID Clear();

		D3DXVECTOR3 Position;
		D3DXVECTOR3 Normal;
		D3DXVECTOR2 TexturePosition;

		INT VertexGroup;

		REFERENCE<VOID*, MODEL_GEOSET_GROUP*> GroupNode;
		REFERENCE_OBJECT<VOID*, MODEL_GEOSET_VERTEX*> FaceNodes;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

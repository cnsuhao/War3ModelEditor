//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_GEOSET_GROUP_H
#define MAGOS_MODEL_GEOSET_GROUP_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class MODEL_BASE;


//+-----------------------------------------------------------------------------
//| Model geoset group node structure
//+-----------------------------------------------------------------------------
struct MODEL_GEOSET_GROUP_NODE
{
	MODEL_GEOSET_GROUP_NODE()
	{
		NodeId = INVALID_INDEX;
	}

	INT NodeId;
	REFERENCE<VOID*, MODEL_BASE*> Node;
};


//+-----------------------------------------------------------------------------
//| Model geoset group class
//+-----------------------------------------------------------------------------
class MODEL_GEOSET_GROUP
{
	public:
		CONSTRUCTOR MODEL_GEOSET_GROUP();
		DESTRUCTOR ~MODEL_GEOSET_GROUP();

		VOID Clear();

		INT MatrixListSize;
		SIMPLE_CONTAINER<MODEL_GEOSET_GROUP_NODE*> MatrixList;

		D3DXMATRIX Matrix;

		REFERENCE_OBJECT<VOID*, MODEL_GEOSET_GROUP*> VertexNodes;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

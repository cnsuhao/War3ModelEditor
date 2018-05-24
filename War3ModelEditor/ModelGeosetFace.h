//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_GEOSET_FACE_H
#define MAGOS_MODEL_GEOSET_FACE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class MODEL_GEOSET_VERTEX;


//+-----------------------------------------------------------------------------
//| Model geoset face class
//+-----------------------------------------------------------------------------
class MODEL_GEOSET_FACE
{
	public:
		CONSTRUCTOR MODEL_GEOSET_FACE();
		DESTRUCTOR ~MODEL_GEOSET_FACE();

		VOID Clear();

		INT Index1;
		INT Index2;
		INT Index3;

		REFERENCE<VOID*, MODEL_GEOSET_VERTEX*> Node1;
		REFERENCE<VOID*, MODEL_GEOSET_VERTEX*> Node2;
		REFERENCE<VOID*, MODEL_GEOSET_VERTEX*> Node3;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_BONE_H
#define MAGOS_MODEL_BONE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model bone data structure
//+-----------------------------------------------------------------------------
struct MODEL_BONE_DATA : public MODEL_BASE_DATA
{
	MODEL_BONE_DATA()
	{
		GeosetId = INVALID_INDEX;
		GeosetAnimationId = INVALID_INDEX;
	}

	INT GeosetId;
	INT GeosetAnimationId;
};


//+-----------------------------------------------------------------------------
//| Model bone class
//+-----------------------------------------------------------------------------
class MODEL_BONE : public MODEL_BASE
{
	public:
		CONSTRUCTOR MODEL_BONE();
		DESTRUCTOR ~MODEL_BONE();

		VOID Clear();
		INT GetSize();

		MODEL_BONE_DATA& Data();

	protected:
		MODEL_BONE_DATA BoneData;

	public:
		REFERENCE<MODEL_BONE*, MODEL_GEOSET*> GeosetNode;
		REFERENCE<MODEL_BONE*, MODEL_GEOSET_ANIMATION*> GeosetAnimationNode;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

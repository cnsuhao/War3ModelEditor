//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_GEOSET_ANIMATION_H
#define MAGOS_MODEL_GEOSET_ANIMATION_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model geoset animation data structure
//+-----------------------------------------------------------------------------
struct MODEL_GEOSET_ANIMATION_DATA
{
	MODEL_GEOSET_ANIMATION_DATA()
	{
		GeosetId = INVALID_INDEX;

		Color.SetStaticVector3(D3DXVECTOR3(1.0f, 1.0f, 1.0f), "Color");
		Alpha.SetStaticScalar(1.0f, "Alpha");

		UseColor = FALSE;
		DropShadow = FALSE;

		InternalGeosetAnimationId = INVALID_INDEX;
	}

	INT GeosetId;

	INTERPOLATOR Color;
	INTERPOLATOR Alpha;

	BOOL UseColor;
	BOOL DropShadow;

	INT InternalGeosetAnimationId;
};


//+-----------------------------------------------------------------------------
//| Model geoset animation class
//+-----------------------------------------------------------------------------
class MODEL_GEOSET_ANIMATION
{
	public:
		CONSTRUCTOR MODEL_GEOSET_ANIMATION();
		DESTRUCTOR ~MODEL_GEOSET_ANIMATION();

		VOID Clear();
		INT GetSize();

		MODEL_GEOSET_ANIMATION_DATA& Data();

	protected:
		MODEL_GEOSET_ANIMATION_DATA GeosetAnimationData;

	public:
		REFERENCE<MODEL_GEOSET_ANIMATION*, MODEL_GEOSET*> GeosetNode;
		REFERENCE_OBJECT<MODEL_BONE*, MODEL_GEOSET_ANIMATION*> BoneNodes;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

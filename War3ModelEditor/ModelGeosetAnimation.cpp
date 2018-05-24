//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelGeosetAnimation.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_GEOSET_ANIMATION::MODEL_GEOSET_ANIMATION()
{
	GeosetNode.SetData(this);
	BoneNodes.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_GEOSET_ANIMATION::~MODEL_GEOSET_ANIMATION()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the model geoset animation
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET_ANIMATION::Clear()
{
	GeosetAnimationData = MODEL_GEOSET_ANIMATION_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the model geoset animation
//+-----------------------------------------------------------------------------
INT MODEL_GEOSET_ANIMATION::GetSize()
{
	return GeosetAnimationData.Alpha.GetSize() + GeosetAnimationData.Color.GetSize() + 28;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_GEOSET_ANIMATION_DATA& MODEL_GEOSET_ANIMATION::Data()
{
	return GeosetAnimationData;
}

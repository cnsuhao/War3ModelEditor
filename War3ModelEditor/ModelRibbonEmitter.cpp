//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelRibbonEmitter.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_RIBBON_EMITTER::MODEL_RIBBON_EMITTER()
{
	ModelBaseData = &RibbonEmitterData;
	ModelBaseData->Type = NODE_TYPE_RIBBON_EMITTER;

	MaterialNode.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_RIBBON_EMITTER::~MODEL_RIBBON_EMITTER()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the ribbon emitter
//+-----------------------------------------------------------------------------
VOID MODEL_RIBBON_EMITTER::Clear()
{
	RibbonEmitterData = MODEL_RIBBON_EMITTER_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the ribbon emitter
//+-----------------------------------------------------------------------------
INT MODEL_RIBBON_EMITTER::GetSize()
{
	INT TotalSize;
	INT VisibilitySize;
	INT HeightAboveSize;
	INT HeightBelowSize;

	VisibilitySize = RibbonEmitterData.Visibility.GetSize();
	HeightAboveSize = RibbonEmitterData.HeightAbove.GetSize();
	HeightBelowSize = RibbonEmitterData.HeightBelow.GetSize();

	TotalSize = GetBaseSize() + 56 + VisibilitySize + HeightAboveSize + HeightBelowSize;

	return TotalSize;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_RIBBON_EMITTER_DATA& MODEL_RIBBON_EMITTER::Data()
{
	return RibbonEmitterData;
}

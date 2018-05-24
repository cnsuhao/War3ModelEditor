//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_RIBBON_EMITTER_H
#define MAGOS_MODEL_RIBBON_EMITTER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model ribbon emitter data structure
//+-----------------------------------------------------------------------------
struct MODEL_RIBBON_EMITTER_DATA : public MODEL_BASE_DATA
{
	MODEL_RIBBON_EMITTER_DATA()
	{
		HeightAbove.SetStaticScalar(0.0f, "HeightAbove");
		HeightBelow.SetStaticScalar(0.0f, "HeightBelow");
		Alpha.SetStaticScalar(1.0f, "Alpha");
		Color.SetStaticVector3(D3DXVECTOR3(1.0f, 1.0f, 1.0f), "Color");
		TextureSlot.SetStaticScalar(0.0f, "TextureSlot");
		Visibility.SetStaticScalar(1.0f, "Visibility");

		EmissionRate = 0.0f;
		LifeSpan = 0.0f;
		Gravity = 0.0f;
		Rows = 1;
		Columns = 1;
		MaterialId = INVALID_INDEX;
	}

	INTERPOLATOR HeightAbove;
	INTERPOLATOR HeightBelow;
	INTERPOLATOR Alpha;
	INTERPOLATOR Color;
	INTERPOLATOR TextureSlot;
	INTERPOLATOR Visibility;

	FLOAT EmissionRate;
	FLOAT LifeSpan;
	FLOAT Gravity;
	INT Rows;
	INT Columns;
	INT MaterialId;
};


//+-----------------------------------------------------------------------------
//| Model ribbon emitter class
//+-----------------------------------------------------------------------------
class MODEL_RIBBON_EMITTER : public MODEL_BASE
{
	public:
		CONSTRUCTOR MODEL_RIBBON_EMITTER();
		DESTRUCTOR ~MODEL_RIBBON_EMITTER();

		VOID Clear();
		INT GetSize();

		MODEL_RIBBON_EMITTER_DATA& Data();

	protected:
		MODEL_RIBBON_EMITTER_DATA RibbonEmitterData;

	public:
		REFERENCE<MODEL_RIBBON_EMITTER*, MODEL_MATERIAL*> MaterialNode;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

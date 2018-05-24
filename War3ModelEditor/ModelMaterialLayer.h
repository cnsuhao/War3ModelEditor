//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_MATERIAL_LAYER_H
#define MAGOS_MODEL_MATERIAL_LAYER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model materiallayer data structure
//+-----------------------------------------------------------------------------
struct MODEL_MATERIAL_LAYER_DATA
{
	MODEL_MATERIAL_LAYER_DATA()
	{
		FilterMode = FILTER_MODE_NONE;

		AnimatedTextureId.SetStaticScalarInt(0, "TextureID");
		Alpha.SetStaticScalar(1.0f, "Alpha");

		TextureId = INVALID_INDEX;
		TextureAnimationId = INVALID_INDEX;

		Unshaded = FALSE;
		Unfogged = FALSE;
		TwoSided = FALSE;
		SphereEnvironmentMap = FALSE;
		NoDepthTest = FALSE;
		NoDepthSet = FALSE;
	}

	FILTER_MODE FilterMode;

	INTERPOLATOR AnimatedTextureId;
	INTERPOLATOR Alpha;

	INT TextureId;
	INT TextureAnimationId;

	BOOL Unshaded;
	BOOL Unfogged;
	BOOL TwoSided;
	BOOL SphereEnvironmentMap;
	BOOL NoDepthTest;
	BOOL NoDepthSet;
};


//+-----------------------------------------------------------------------------
//| Model material layer class
//+-----------------------------------------------------------------------------
class MODEL_MATERIAL_LAYER
{
	public:
		CONSTRUCTOR MODEL_MATERIAL_LAYER();
		DESTRUCTOR ~MODEL_MATERIAL_LAYER();

		VOID Clear();
		INT GetSize();

		MODEL_MATERIAL_LAYER_DATA& Data();
		MODEL_MATERIAL_LAYER_DATA& NewData();

		VOID MarkAsUpdated();
		VOID UpdateDataIfNeccessary();

		INT GetRenderOrder();

		VOID UseMaterial(CONST SEQUENCE_TIME& Time);

	protected:
		MODEL_MATERIAL_LAYER_DATA MaterialLayerData;

		BOOL MaterialLayerChanged;
		MODEL_MATERIAL_LAYER_DATA MaterialLayerNewData;

	public:
		REFERENCE<MODEL_MATERIAL_LAYER*, MODEL_TEXTURE*> TextureNode;
		REFERENCE<MODEL_MATERIAL_LAYER*, MODEL_TEXTURE*> NewTextureNode;
		REFERENCE<MODEL_MATERIAL_LAYER*, MODEL_TEXTURE_ANIMATION*> TextureAnimationNode;
		REFERENCE<MODEL_MATERIAL_LAYER*, MODEL_TEXTURE_ANIMATION*> NewTextureAnimationNode;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Graphics.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

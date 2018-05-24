//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_TEXTURE_ANIMATION_H
#define MAGOS_MODEL_TEXTURE_ANIMATION_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model texture animation data structure
//+-----------------------------------------------------------------------------
struct MODEL_TEXTURE_ANIMATION_DATA
{
	MODEL_TEXTURE_ANIMATION_DATA()
	{
		Translation.SetStaticVector3(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "Translation");
		Rotation.SetStaticVector4(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f), "Rotation");
		Scaling.SetStaticVector3(D3DXVECTOR3(1.0f, 1.0f, 1.0f), "Scaling");

		InternalTextureAnimationId = INVALID_INDEX;
	}

	INTERPOLATOR Translation;
	INTERPOLATOR Rotation;
	INTERPOLATOR Scaling;

	INT InternalTextureAnimationId;
};


//+-----------------------------------------------------------------------------
//| Model texture animation class
//+-----------------------------------------------------------------------------
class MODEL_TEXTURE_ANIMATION
{
	public:
		CONSTRUCTOR MODEL_TEXTURE_ANIMATION();
		DESTRUCTOR ~MODEL_TEXTURE_ANIMATION();

		VOID Clear();
		INT GetSize();

		MODEL_TEXTURE_ANIMATION_DATA& Data();

	protected:
		MODEL_TEXTURE_ANIMATION_DATA TextureAnimationData;

	public:
		REFERENCE_OBJECT<MODEL_MATERIAL_LAYER*, MODEL_TEXTURE_ANIMATION*> MaterialLayerNodes;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

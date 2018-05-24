//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelTextureAnimation.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_TEXTURE_ANIMATION::MODEL_TEXTURE_ANIMATION()
{
	MaterialLayerNodes.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_TEXTURE_ANIMATION::~MODEL_TEXTURE_ANIMATION()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the model texture animation
//+-----------------------------------------------------------------------------
VOID MODEL_TEXTURE_ANIMATION::Clear()
{
	TextureAnimationData = MODEL_TEXTURE_ANIMATION_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the model texture animation
//+-----------------------------------------------------------------------------
INT MODEL_TEXTURE_ANIMATION::GetSize()
{
	INT TranslationSize;
	INT RotationSize;
	INT ScalingSize;

	TranslationSize = TextureAnimationData.Translation.GetSize();
	RotationSize = TextureAnimationData.Rotation.GetSize();
	ScalingSize = TextureAnimationData.Scaling.GetSize();

	return TranslationSize + RotationSize + ScalingSize + 4;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_TEXTURE_ANIMATION_DATA& MODEL_TEXTURE_ANIMATION::Data()
{
	return TextureAnimationData;
}

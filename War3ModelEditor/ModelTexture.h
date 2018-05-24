//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_TEXTURE_H
#define MAGOS_MODEL_TEXTURE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model texture data structure
//+-----------------------------------------------------------------------------
struct MODEL_TEXTURE_DATA
{
	MODEL_TEXTURE_DATA()
	{
		ReplaceableId = 0;

		WrapWidth = FALSE;
		WrapHeight = FALSE;

		FileName = "";

		InternalTextureId = INVALID_INDEX;
	}

	INT ReplaceableId;

	BOOL WrapWidth;
	BOOL WrapHeight;

	std::string FileName;

	INT InternalTextureId;
};


//+-----------------------------------------------------------------------------
//| Model texture class
//+-----------------------------------------------------------------------------
class MODEL_TEXTURE
{
	public:
		CONSTRUCTOR MODEL_TEXTURE();
		DESTRUCTOR ~MODEL_TEXTURE();

		VOID Clear();
		INT GetSize();

		MODEL_TEXTURE_DATA& Data();

	protected:
		MODEL_TEXTURE_DATA TextureData;

	public:
		REFERENCE_OBJECT<MODEL_MATERIAL_LAYER*, MODEL_TEXTURE*> MaterialLayerNodes;
		REFERENCE_OBJECT<MODEL_PARTICLE_EMITTER_2*, MODEL_TEXTURE*> ParticleEmitter2Nodes;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

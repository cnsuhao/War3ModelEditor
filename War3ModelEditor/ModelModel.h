//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_MODEL_H
#define MAGOS_MODEL_MODEL_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"

//addedbyme

typedef int MODEL_VALUE_STATIC;
#define MODEL_VALUE_TYPE_INT


//+-----------------------------------------------------------------------------
//| Model model data structure
//+-----------------------------------------------------------------------------
struct MODEL_MODEL_DATA
{
	MODEL_MODEL_DATA()
	{
		/*
		NumGeosets.SetInfo("NumGeosets", MODEL_VALUE_TYPE_INT);
		NumGeosetAnims.SetInfo("NumGeosetAnims", MODEL_VALUE_TYPE_INT);
		NumHelpers.SetInfo("NumHelpers", MODEL_VALUE_TYPE_INT);
		NumBones.SetInfo("NumBones", MODEL_VALUE_TYPE_INT);
		NumLights.SetInfo("NumLights", MODEL_VALUE_TYPE_INT);
		NumAttachments.SetInfo("NumAttachments", MODEL_VALUE_TYPE_INT);
		NumEvents.SetInfo("NumEvents", MODEL_VALUE_TYPE_INT);
		NumParticleEmitters.SetInfo("NumParticleEmitters", MODEL_VALUE_TYPE_INT);
		NumParticleEmitters2.SetInfo("NumParticleEmitters2", MODEL_VALUE_TYPE_INT);
		NumRibbonEmitters.SetInfo("NumRibbonEmitters", MODEL_VALUE_TYPE_INT);
		BlendTime.SetInfo("BlendTime", MODEL_VALUE_TYPE_INT);
		MinimumExtent.SetInfo("MinimumExtent", MODEL_VALUE_TYPE_VECTOR3);
		MaximumExtent.SetInfo("MaximumExtent", MODEL_VALUE_TYPE_VECTOR3);
		BoundsRadius.SetInfo("BoundsRadius", MODEL_VALUE_TYPE_FLOAT);
		*/
	}

	MODEL_VALUE_STATIC NumGeosets;
	MODEL_VALUE_STATIC NumGeosetAnims;
	MODEL_VALUE_STATIC NumHelpers;
	MODEL_VALUE_STATIC NumBones;
	MODEL_VALUE_STATIC NumLights;
	MODEL_VALUE_STATIC NumAttachments;
	MODEL_VALUE_STATIC NumEvents;
	MODEL_VALUE_STATIC NumParticleEmitters;
	MODEL_VALUE_STATIC NumParticleEmitters2;
	MODEL_VALUE_STATIC NumRibbonEmitters;
	MODEL_VALUE_STATIC BlendTime;
	MODEL_VALUE_STATIC MinimumExtent;
	MODEL_VALUE_STATIC MaximumExtent;
	MODEL_VALUE_STATIC BoundsRadius;
};


//+-----------------------------------------------------------------------------
//| Model model class
//+-----------------------------------------------------------------------------
class MODEL_MODEL : public MODEL_BASE
{
	public:
		CONSTRUCTOR MODEL_MODEL();
		DESTRUCTOR ~MODEL_MODEL();

		virtual VOID Clear();

		virtual BOOL Save(TOKEN_OUT_STREAM& TokenStream);
		virtual BOOL Load(TOKEN_IN_STREAM& TokenStream);

		virtual BOOL Save(DATA_OUT_STREAM& DataStream);
		virtual BOOL Load(DATA_IN_STREAM& DataStream);

		virtual INT GetMdxSize();

		MODEL_MODEL_DATA& Data();

	protected:
		MODEL_MODEL_DATA ModelData;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

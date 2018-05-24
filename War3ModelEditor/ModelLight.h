//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_LIGHT_H
#define MAGOS_MODEL_LIGHT_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model light data structure
//+-----------------------------------------------------------------------------
struct MODEL_LIGHT_DATA : public MODEL_BASE_DATA
{
	MODEL_LIGHT_DATA()
	{
		Type = 0;

		AttenuationStart.SetStaticScalar(0, "AttenuationStart");
		AttenuationEnd.SetStaticScalar(0, "AttenuationEnd");
		Color.SetStaticVector3(D3DXVECTOR3(1.0f, 1.0f, 1.0f), "Color");
		Intensity.SetStaticScalar(0.0f, "Intensity");
		AmbientColor.SetStaticVector3(D3DXVECTOR3(1.0f, 1.0f, 1.0f), "AmbColor");
		AmbientIntensity.SetStaticScalar(0.0f, "AmbIntensity");
		Visibility.SetStaticScalar(1.0f, "Visibility");
	}

	DWORD Type;

	INTERPOLATOR AttenuationStart;
	INTERPOLATOR AttenuationEnd;
	INTERPOLATOR Color;
	INTERPOLATOR Intensity;
	INTERPOLATOR AmbientColor;
	INTERPOLATOR AmbientIntensity;
	INTERPOLATOR Visibility;
};


//+-----------------------------------------------------------------------------
//| Model light class
//+-----------------------------------------------------------------------------
class MODEL_LIGHT : public MODEL_BASE
{
	public:
		CONSTRUCTOR MODEL_LIGHT();
		DESTRUCTOR ~MODEL_LIGHT();

		VOID Clear();
		INT GetSize();

		MODEL_LIGHT_DATA& Data();

	protected:
		MODEL_LIGHT_DATA LightData;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

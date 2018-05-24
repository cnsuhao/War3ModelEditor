//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_CAMERA_H
#define MAGOS_MODEL_CAMERA_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model camera data structure
//+-----------------------------------------------------------------------------
struct MODEL_CAMERA_DATA
{
	MODEL_CAMERA_DATA()
	{
		Name = "Name";

		Source = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		SourceTranslation.SetStaticVector3(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "Translation");
		TargetTranslation.SetStaticVector3(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "Translation");
		Rotation.SetStaticScalar(0.0f, "Rotation");

		FieldOfView = GRAPHICS_DEFAULT_FIELD_OF_VIEW;
		NearDistance = GRAPHICS_DEFAULT_NEAR_DISTANCE;
		FarDistance = GRAPHICS_DEFAULT_FAR_DISTANCE;
	}

	std::string Name;

	D3DXVECTOR3 Source;
	D3DXVECTOR3 Target;

	INTERPOLATOR SourceTranslation;
	INTERPOLATOR TargetTranslation;
	INTERPOLATOR Rotation;

	FLOAT FieldOfView;
	FLOAT NearDistance;
	FLOAT FarDistance;
};


//+-----------------------------------------------------------------------------
//| Model camera class
//+-----------------------------------------------------------------------------
class MODEL_CAMERA
{
	public:
		CONSTRUCTOR MODEL_CAMERA();
		DESTRUCTOR ~MODEL_CAMERA();

		VOID Clear();
		INT GetSize();

		MODEL_CAMERA_DATA& Data();

	protected:
		MODEL_CAMERA_DATA CameraData;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

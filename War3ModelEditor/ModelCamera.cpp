//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelCamera.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_CAMERA::MODEL_CAMERA()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_CAMERA::~MODEL_CAMERA()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the camera
//+-----------------------------------------------------------------------------
VOID MODEL_CAMERA::Clear()
{
	CameraData = MODEL_CAMERA_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the camera
//+-----------------------------------------------------------------------------
INT MODEL_CAMERA::GetSize()
{
	INT SourceSize;
	INT TargetSize;
	INT RotationSize;

	SourceSize = CameraData.SourceTranslation.GetSize();
	TargetSize = CameraData.TargetTranslation.GetSize();
	RotationSize = CameraData.Rotation.GetSize();

	return 120 + SourceSize + TargetSize + RotationSize;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_CAMERA_DATA& MODEL_CAMERA::Data()
{
	return CameraData;
}

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_BASE::MODEL_BASE()
{
	D3DXMatrixIdentity(&BaseMatrix);

	ModelBaseData = NULL;

	ParentNode.SetData(this);
	ChildNodes.SetData(this);
	MatrixListNodes.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_BASE::~MODEL_BASE()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Returns the base size
//+-----------------------------------------------------------------------------
INT MODEL_BASE::GetBaseSize()
{
	INT TotalSize;
	INT TranslationSize;
	INT RotationSize;
	INT ScalingSize;

	TranslationSize = ModelBaseData->Translation.GetSize();
	RotationSize = ModelBaseData->Rotation.GetSize();
	ScalingSize = ModelBaseData->Scaling.GetSize();

	TotalSize = 96 + TranslationSize + RotationSize + ScalingSize;

	return TotalSize;
}


//+-----------------------------------------------------------------------------
//| Returns a pointer to the base data
//+-----------------------------------------------------------------------------
MODEL_BASE_DATA* MODEL_BASE::BaseData()
{
	return ModelBaseData;
}


//+-----------------------------------------------------------------------------
//| Builds the matrix
//+-----------------------------------------------------------------------------
VOID MODEL_BASE::BuildMatrix(CONST SEQUENCE_TIME& Time)
{
	D3DXVECTOR3 Translation;
	D3DXQUATERNION Rotation;
	D3DXQUATERNION CameraRotation;
	D3DXVECTOR3 Scaling;
	D3DXVECTOR3 TranslationVector;
	D3DXVECTOR4 RotationVector;
	D3DXVECTOR3 ScalingVector;
	D3DXMATRIX TempMatrix;
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX CameraRotationMatrix;
	D3DXMATRIX CameraRotationMatrixY;
	D3DXMATRIX CameraRotationMatrixZ;
	REFERENCE<MODEL_BASE*, MODEL_BASE*>* CurrentReference;
	REFERENCE<MODEL_BASE*, MODEL_BASE*>* NextReference;

	TranslationVector = ModelBaseData->Translation.GetVector3(Time);
	RotationVector = ModelBaseData->Rotation.GetVector4(Time);
	ScalingVector = ModelBaseData->Scaling.GetVector3(Time);

	D3DXMatrixTransformation(&BaseMatrix, &ModelBaseData->PivotPoint, NULL, &ScalingVector, &ModelBaseData->PivotPoint, reinterpret_cast<D3DXQUATERNION*>(&RotationVector), &TranslationVector);
	if(ParentNode.IsAttached())
	{
		D3DXMatrixMultiply(&BaseMatrix, &BaseMatrix, &ParentNode.GetObjectData()->Matrix());
	}

	if(ModelBaseData->Billboarded)
	{
		D3DXMatrixRotationY(&CameraRotationMatrixY, -Camera.GetPitch());
		D3DXMatrixRotationZ(&CameraRotationMatrixZ, Camera.GetYaw());
		D3DXMatrixMultiply(&CameraRotationMatrix, &CameraRotationMatrixY, &CameraRotationMatrixZ);
		D3DXQuaternionRotationMatrix(&CameraRotation, &CameraRotationMatrix);
		D3DXMatrixTransformation(&CameraRotationMatrix, NULL, NULL, NULL, &ModelBaseData->PivotPoint, &CameraRotation, NULL);

		//D3DXMatrixDecompose(&Scaling, &Rotation, &Translation, &BaseMatrix);
		D3DXMatrixTransformation(&RotationMatrix, NULL, NULL, NULL, &ModelBaseData->PivotPoint, &Rotation, NULL);

		D3DXMatrixMultiply(&TempMatrix, &CameraRotationMatrix, &RotationMatrix);
		D3DXMatrixMultiply(&BaseMatrix, &TempMatrix, &BaseMatrix);
	}
	else if(ModelBaseData->BillboardedLockX)
	{
		//XXXXXXXXXXXXXXXXXXXXXX
		//Not correct

		D3DXMatrixRotationY(&CameraRotationMatrixY, -Camera.GetPitch());
		D3DXMatrixRotationZ(&CameraRotationMatrixZ, Camera.GetYaw());
		D3DXMatrixMultiply(&CameraRotationMatrix, &CameraRotationMatrixY, &CameraRotationMatrixZ);
		D3DXQuaternionRotationMatrix(&CameraRotation, &CameraRotationMatrix);
		D3DXMatrixTransformation(&CameraRotationMatrix, NULL, NULL, NULL, &ModelBaseData->PivotPoint, &CameraRotation, NULL);

		//D3DXMatrixDecompose(&Scaling, &Rotation, &Translation, &BaseMatrix);
		D3DXMatrixTransformation(&RotationMatrix, NULL, NULL, NULL, &ModelBaseData->PivotPoint, &Rotation, NULL);

		D3DXMatrixMultiply(&TempMatrix, &CameraRotationMatrix, &RotationMatrix);
		D3DXMatrixMultiply(&BaseMatrix, &TempMatrix, &BaseMatrix);
	}
	else if(ModelBaseData->BillboardedLockY)
	{
		//XXXXXXXXXXXXXXXXXXXXXX
		//Not correct

		D3DXMatrixRotationY(&CameraRotationMatrixY, -Camera.GetPitch());
		D3DXMatrixRotationZ(&CameraRotationMatrixZ, Camera.GetYaw());
		D3DXMatrixMultiply(&CameraRotationMatrix, &CameraRotationMatrixY, &CameraRotationMatrixZ);
		D3DXQuaternionRotationMatrix(&CameraRotation, &CameraRotationMatrix);
		D3DXMatrixTransformation(&CameraRotationMatrix, NULL, NULL, NULL, &ModelBaseData->PivotPoint, &CameraRotation, NULL);

		//D3DXMatrixDecompose(&Scaling, &Rotation, &Translation, &BaseMatrix);
		D3DXMatrixTransformation(&RotationMatrix, NULL, NULL, NULL, &ModelBaseData->PivotPoint, &Rotation, NULL);

		D3DXMatrixMultiply(&TempMatrix, &CameraRotationMatrix, &RotationMatrix);
		D3DXMatrixMultiply(&BaseMatrix, &TempMatrix, &BaseMatrix);
	}
	else if(ModelBaseData->BillboardedLockZ)
	{
		D3DXMatrixRotationZ(&CameraRotationMatrixZ, Camera.GetYaw());
		D3DXQuaternionRotationMatrix(&CameraRotation, &CameraRotationMatrixZ);
		D3DXMatrixTransformation(&CameraRotationMatrix, NULL, NULL, NULL, &ModelBaseData->PivotPoint, &CameraRotation, NULL);

		//D3DXMatrixDecompose(&Scaling, &Rotation, &Translation, &BaseMatrix);
		D3DXMatrixTransformation(&RotationMatrix, NULL, NULL, NULL, &ModelBaseData->PivotPoint, &Rotation, NULL);

		D3DXMatrixMultiply(&TempMatrix, &CameraRotationMatrix, &RotationMatrix);
		D3DXMatrixMultiply(&BaseMatrix, &TempMatrix, &BaseMatrix);
	}

	CurrentReference = ChildNodes.GetFirstReference();
	while(CurrentReference != NULL)
	{
		NextReference = ChildNodes.GetNextReference(CurrentReference);
		CurrentReference->GetData()->BuildMatrix(Time);
		CurrentReference = NextReference;
	}
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the matrix
//+-----------------------------------------------------------------------------
CONST D3DXMATRIX& MODEL_BASE::Matrix()
{
	return BaseMatrix;
}

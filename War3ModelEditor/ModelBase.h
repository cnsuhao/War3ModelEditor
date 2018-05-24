//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_BASE_H
#define MAGOS_MODEL_BASE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Node flags
//+-----------------------------------------------------------------------------
CONST DWORD NODE_FLAG_HELPER                   = 0;
CONST DWORD NODE_FLAG_DONT_INHERIT_TRANSLATION = 1;
CONST DWORD NODE_FLAG_DONT_INHERIT_ROTATION    = 2;
CONST DWORD NODE_FLAG_DONT_INHERIT_SCALING     = 4;
CONST DWORD NODE_FLAG_BILLBOARDED              = 8;
CONST DWORD NODE_FLAG_BILLBOARDED_LOCK_X       = 16;
CONST DWORD NODE_FLAG_BILLBOARDED_LOCK_Y       = 32;
CONST DWORD NODE_FLAG_BILLBOARDED_LOCK_Z       = 64;
CONST DWORD NODE_FLAG_CAMERA_ANCHORED          = 128;
CONST DWORD NODE_FLAG_BONE                     = 256;
CONST DWORD NODE_FLAG_LIGHT                    = 512;
CONST DWORD NODE_FLAG_EVENT_OBJECT             = 1024;
CONST DWORD NODE_FLAG_ATTACHMENT               = 2048;
CONST DWORD NODE_FLAG_PARTICLE_EMITTER         = 4096;
CONST DWORD NODE_FLAG_COLLISION_SHAPE          = 8192;
CONST DWORD NODE_FLAG_RIBBON_EMITTER           = 16384;
CONST DWORD NODE_FLAG_UNSHADED                 = 32768;
CONST DWORD NODE_FLAG_EMITTER_USES_MDL         = 32768;
CONST DWORD NODE_FLAG_SORT_PRIMITIVES_FAR_Z    = 65536;
CONST DWORD NODE_FLAG_EMITTER_USES_TGA         = 65536;
CONST DWORD NODE_FLAG_LINE_EMITTER             = 131072;
CONST DWORD NODE_FLAG_UNFOGGED                 = 262144;
CONST DWORD NODE_FLAG_MODEL_SPACE              = 524288;
CONST DWORD NODE_FLAG_XY_QUAD                  = 1048576;


//+-----------------------------------------------------------------------------
//| Model base data structure
//+-----------------------------------------------------------------------------
struct MODEL_BASE_DATA
{
	MODEL_BASE_DATA()
	{
		Name = "UNNAMED";

		ObjectId = INVALID_INDEX;
		ParentId = INVALID_INDEX;

		Translation.SetStaticVector3(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "Translation");
		Rotation.SetStaticVector4(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f), "Rotation");
		Scaling.SetStaticVector3(D3DXVECTOR3(1.0f, 1.0f, 1.0f), "Scaling");

		DontInheritTranslation = FALSE;
		DontInheritRotation = FALSE;
		DontInheritScaling = FALSE;
		Billboarded = FALSE;
		BillboardedLockX = FALSE;
		BillboardedLockY = FALSE;
		BillboardedLockZ = FALSE;
		CameraAnchored = FALSE;

		PivotPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		Type = NODE_TYPE_NONE;
		TreeViewItem = NULL;
	}

	std::string Name;

	INT ObjectId;
	INT ParentId;

	INTERPOLATOR Translation;
	INTERPOLATOR Rotation;
	INTERPOLATOR Scaling;

	BOOL DontInheritTranslation;
	BOOL DontInheritRotation;
	BOOL DontInheritScaling;
	BOOL Billboarded;
	BOOL BillboardedLockX;
	BOOL BillboardedLockY;
	BOOL BillboardedLockZ;
	BOOL CameraAnchored;

	D3DXVECTOR3 PivotPoint;

	NODE_TYPE Type;
	HTREEITEM TreeViewItem;
};


//+-----------------------------------------------------------------------------
//| Model base class
//+-----------------------------------------------------------------------------
class MODEL_BASE
{
	public:
		CONSTRUCTOR MODEL_BASE();
		DESTRUCTOR ~MODEL_BASE();

		INT GetBaseSize();

		MODEL_BASE_DATA* BaseData();

		VOID BuildMatrix(CONST SEQUENCE_TIME& Time);
		CONST D3DXMATRIX& Matrix();

	protected:
		MODEL_BASE_DATA* ModelBaseData;

		D3DXMATRIX BaseMatrix;

	public:
		REFERENCE<MODEL_BASE*, MODEL_BASE*> ParentNode;
		REFERENCE_OBJECT<MODEL_BASE*, MODEL_BASE*> ChildNodes;
		REFERENCE_OBJECT<VOID*, MODEL_BASE*> MatrixListNodes;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Model.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

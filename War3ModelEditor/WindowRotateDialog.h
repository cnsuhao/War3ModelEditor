//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_ROTATE_DIALOG_H
#define MAGOS_WINDOW_ROTATE_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Rotation type enumeration
//+-----------------------------------------------------------------------------
enum ROTATION_TYPE
{
	ROTATION_TYPE_X,
	ROTATION_TYPE_Y,
	ROTATION_TYPE_Z,
	ROTATION_TYPE_AXIS,
	ROTATION_TYPE_QUATERNION,
};


//+-----------------------------------------------------------------------------
//| Rotate info structure
//+-----------------------------------------------------------------------------
struct ROTATE_INFO
{
	ROTATE_INFO()
	{
		Type = ROTATION_TYPE_X;

		Angle = 0.0f;
		Axis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		PivotPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
		AroundPivotPoint = TRUE;
		Radians = TRUE;
	}

	ROTATION_TYPE Type;

	FLOAT Angle;
	D3DXVECTOR3 Axis;
	D3DXVECTOR3 PivotPoint;
	D3DXQUATERNION Quaternion;
	BOOL AroundPivotPoint;
	BOOL Radians;
};


//+-----------------------------------------------------------------------------
//| Rotate dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_ROTATE_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_ROTATE_DIALOG();
		DESTRUCTOR ~WINDOW_ROTATE_DIALOG();

		BOOL Display(HWND ParentWindow, ROTATE_INFO& RotateInfo) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID SetEnabledWindows(HWND Window, ROTATION_TYPE Type);

		static ROTATE_INFO StaticRotateInfo;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_ROTATE_DIALOG RotateDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

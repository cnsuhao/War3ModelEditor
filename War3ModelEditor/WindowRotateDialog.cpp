//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowRotateDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_ROTATE_DIALOG RotateDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
ROTATE_INFO WINDOW_ROTATE_DIALOG::StaticRotateInfo;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_ROTATE_DIALOG::WINDOW_ROTATE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_ROTATE_DIALOG::~WINDOW_ROTATE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_ROTATE_DIALOG::Display(HWND ParentWindow, ROTATE_INFO& RotateInfo) CONST
{
	StaticRotateInfo.PivotPoint = RotateInfo.PivotPoint;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogRotate), ParentWindow, DialogMessageHandler, 0))
	{
		RotateInfo = StaticRotateInfo;
		return TRUE;
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_ROTATE_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetFloat(GetDlgItem(Window, DialogRotateEditAngle), StaticRotateInfo.Angle);
			SetFloat(GetDlgItem(Window, DialogRotateEditAxisX), StaticRotateInfo.Axis.x);
			SetFloat(GetDlgItem(Window, DialogRotateEditAxisY), StaticRotateInfo.Axis.y);
			SetFloat(GetDlgItem(Window, DialogRotateEditAxisZ), StaticRotateInfo.Axis.z);
			SetFloat(GetDlgItem(Window, DialogRotateEditPivotX), StaticRotateInfo.PivotPoint.x);
			SetFloat(GetDlgItem(Window, DialogRotateEditPivotY), StaticRotateInfo.PivotPoint.y);
			SetFloat(GetDlgItem(Window, DialogRotateEditPivotZ), StaticRotateInfo.PivotPoint.z);
			SetFloat(GetDlgItem(Window, DialogRotateEditA), StaticRotateInfo.Quaternion.x);
			SetFloat(GetDlgItem(Window, DialogRotateEditB), StaticRotateInfo.Quaternion.y);
			SetFloat(GetDlgItem(Window, DialogRotateEditC), StaticRotateInfo.Quaternion.z);
			SetFloat(GetDlgItem(Window, DialogRotateEditD), StaticRotateInfo.Quaternion.w);

			CheckDlgButton(Window, DialogRotateButtonUsePivotPoint, StaticRotateInfo.AroundPivotPoint);
			CheckDlgButton(Window, DialogRotateButtonUseCentreOfMass, !StaticRotateInfo.AroundPivotPoint);

			CheckDlgButton(Window, DialogRotateButtonRadians, StaticRotateInfo.Radians);
			CheckDlgButton(Window, DialogRotateButtonDegrees, !StaticRotateInfo.Radians);

			if(!StaticRotateInfo.AroundPivotPoint)
			{
				EnableWindow(GetDlgItem(Window, DialogRotateEditPivotX), FALSE);
				EnableWindow(GetDlgItem(Window, DialogRotateEditPivotY), FALSE);
				EnableWindow(GetDlgItem(Window, DialogRotateEditPivotZ), FALSE);
			}

			switch(StaticRotateInfo.Type)
			{
				case ROTATION_TYPE_X:
				{
					CheckDlgButton(Window, DialogRotateButtonX, TRUE);
					break;
				}

				case ROTATION_TYPE_Y:
				{
					CheckDlgButton(Window, DialogRotateButtonY, TRUE);
					break;
				}

				case ROTATION_TYPE_Z:
				{
					CheckDlgButton(Window, DialogRotateButtonZ, TRUE);
					break;
				}

				case ROTATION_TYPE_AXIS:
				{
					CheckDlgButton(Window, DialogRotateButtonAxis, TRUE);
					break;
				}

				case ROTATION_TYPE_QUATERNION:
				{
					CheckDlgButton(Window, DialogRotateButtonQuaternion, TRUE);
					break;
				}
			}

			SetEnabledWindows(Window, StaticRotateInfo.Type);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogRotateButtonUsePivotPoint:
				{
					EnableWindow(GetDlgItem(Window, DialogRotateEditPivotX), TRUE);
					EnableWindow(GetDlgItem(Window, DialogRotateEditPivotY), TRUE);
					EnableWindow(GetDlgItem(Window, DialogRotateEditPivotZ), TRUE);
					return TRUE;
				}

				case DialogRotateButtonUseCentreOfMass:
				{
					EnableWindow(GetDlgItem(Window, DialogRotateEditPivotX), FALSE);
					EnableWindow(GetDlgItem(Window, DialogRotateEditPivotY), FALSE);
					EnableWindow(GetDlgItem(Window, DialogRotateEditPivotZ), FALSE);
					return TRUE;
				}

				case DialogRotateButtonX:
				{
					SetEnabledWindows(Window, ROTATION_TYPE_X);
					return TRUE;
				}

				case DialogRotateButtonY:
				{
					SetEnabledWindows(Window, ROTATION_TYPE_Y);
					return TRUE;
				}

				case DialogRotateButtonZ:
				{
					SetEnabledWindows(Window, ROTATION_TYPE_Z);
					return TRUE;
				}

				case DialogRotateButtonAxis:
				{
					SetEnabledWindows(Window, ROTATION_TYPE_AXIS);
					return TRUE;
				}

				case DialogRotateButtonQuaternion:
				{
					SetEnabledWindows(Window, ROTATION_TYPE_QUATERNION);
					return TRUE;
				}

				case DialogRotateButtonOk:
				{
					if(CheckStateToBool(IsDlgButtonChecked(Window, DialogRotateButtonX)))
					{
						StaticRotateInfo.Type = ROTATION_TYPE_X;
					}
					else if(CheckStateToBool(IsDlgButtonChecked(Window, DialogRotateButtonY)))
					{
						StaticRotateInfo.Type = ROTATION_TYPE_Y;
					}
					else if(CheckStateToBool(IsDlgButtonChecked(Window, DialogRotateButtonZ)))
					{
						StaticRotateInfo.Type = ROTATION_TYPE_Z;
					}
					else if(CheckStateToBool(IsDlgButtonChecked(Window, DialogRotateButtonAxis)))
					{
						StaticRotateInfo.Type = ROTATION_TYPE_AXIS;
					}
					else
					{
						StaticRotateInfo.Type = ROTATION_TYPE_QUATERNION;
					}

					StaticRotateInfo.Angle = GetFloat(GetDlgItem(Window, DialogRotateEditAngle), 0.0f);
					StaticRotateInfo.Axis.x = GetFloat(GetDlgItem(Window, DialogRotateEditAxisX), 1.0f);
					StaticRotateInfo.Axis.y = GetFloat(GetDlgItem(Window, DialogRotateEditAxisY), 0.0f);
					StaticRotateInfo.Axis.z = GetFloat(GetDlgItem(Window, DialogRotateEditAxisZ), 0.0f);
					StaticRotateInfo.PivotPoint.x = GetFloat(GetDlgItem(Window, DialogRotateEditPivotX), 0.0f);
					StaticRotateInfo.PivotPoint.y = GetFloat(GetDlgItem(Window, DialogRotateEditPivotY), 0.0f);
					StaticRotateInfo.PivotPoint.z = GetFloat(GetDlgItem(Window, DialogRotateEditPivotZ), 0.0f);
					StaticRotateInfo.Quaternion.x = GetFloat(GetDlgItem(Window, DialogRotateEditA), 0.0f);
					StaticRotateInfo.Quaternion.y = GetFloat(GetDlgItem(Window, DialogRotateEditB), 0.0f);
					StaticRotateInfo.Quaternion.z = GetFloat(GetDlgItem(Window, DialogRotateEditC), 0.0f);
					StaticRotateInfo.Quaternion.w = GetFloat(GetDlgItem(Window, DialogRotateEditD), 1.0f);

					StaticRotateInfo.AroundPivotPoint = CheckStateToBool(IsDlgButtonChecked(Window, DialogRotateButtonUsePivotPoint));
					StaticRotateInfo.Radians = CheckStateToBool(IsDlgButtonChecked(Window, DialogRotateButtonRadians));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogRotateButtonCancel:
				{
					RemoveWindow(Window);
					EndDialog(Window, 0);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogRotateButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Enables or disables some windows
//+-----------------------------------------------------------------------------
VOID WINDOW_ROTATE_DIALOG::SetEnabledWindows(HWND Window, ROTATION_TYPE Type)
{
	EnableWindow(GetDlgItem(Window, DialogRotateEditAngle), (Type != ROTATION_TYPE_QUATERNION));
	EnableWindow(GetDlgItem(Window, DialogRotateEditAxisX), (Type == ROTATION_TYPE_AXIS));
	EnableWindow(GetDlgItem(Window, DialogRotateEditAxisY), (Type == ROTATION_TYPE_AXIS));
	EnableWindow(GetDlgItem(Window, DialogRotateEditAxisZ), (Type == ROTATION_TYPE_AXIS));
	EnableWindow(GetDlgItem(Window, DialogRotateEditA), (Type == ROTATION_TYPE_QUATERNION));
	EnableWindow(GetDlgItem(Window, DialogRotateEditB), (Type == ROTATION_TYPE_QUATERNION));
	EnableWindow(GetDlgItem(Window, DialogRotateEditC), (Type == ROTATION_TYPE_QUATERNION));
	EnableWindow(GetDlgItem(Window, DialogRotateEditD), (Type == ROTATION_TYPE_QUATERNION));
}

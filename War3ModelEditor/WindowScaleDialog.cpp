//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowScaleDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_SCALE_DIALOG ScaleDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
SCALE_INFO WINDOW_SCALE_DIALOG::StaticScaleInfo;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_SCALE_DIALOG::WINDOW_SCALE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_SCALE_DIALOG::~WINDOW_SCALE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_SCALE_DIALOG::Display(HWND ParentWindow, SCALE_INFO& ScaleInfo) CONST
{
	StaticScaleInfo.PivotPoint = ScaleInfo.PivotPoint;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogScale), ParentWindow, DialogMessageHandler, 0))
	{
		ScaleInfo = StaticScaleInfo;
		return TRUE;
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_SCALE_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetFloat(GetDlgItem(Window, DialogScaleEditX), StaticScaleInfo.Scaling.x);
			SetFloat(GetDlgItem(Window, DialogScaleEditY), StaticScaleInfo.Scaling.y);
			SetFloat(GetDlgItem(Window, DialogScaleEditZ), StaticScaleInfo.Scaling.z);
			SetFloat(GetDlgItem(Window, DialogScaleEditPivotX), StaticScaleInfo.PivotPoint.x);
			SetFloat(GetDlgItem(Window, DialogScaleEditPivotY), StaticScaleInfo.PivotPoint.y);
			SetFloat(GetDlgItem(Window, DialogScaleEditPivotZ), StaticScaleInfo.PivotPoint.z);

			CheckDlgButton(Window, DialogScaleButtonUsePivotPoint, StaticScaleInfo.AroundPivotPoint);
			CheckDlgButton(Window, DialogScaleButtonUseCentreOfMass, !StaticScaleInfo.AroundPivotPoint);

			if(!StaticScaleInfo.AroundPivotPoint)
			{
				EnableWindow(GetDlgItem(Window, DialogScaleEditPivotX), FALSE);
				EnableWindow(GetDlgItem(Window, DialogScaleEditPivotY), FALSE);
				EnableWindow(GetDlgItem(Window, DialogScaleEditPivotZ), FALSE);
			}

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogScaleButtonUsePivotPoint:
				{
					EnableWindow(GetDlgItem(Window, DialogScaleEditPivotX), TRUE);
					EnableWindow(GetDlgItem(Window, DialogScaleEditPivotY), TRUE);
					EnableWindow(GetDlgItem(Window, DialogScaleEditPivotZ), TRUE);
					return TRUE;
				}

				case DialogScaleButtonUseCentreOfMass:
				{
					EnableWindow(GetDlgItem(Window, DialogScaleEditPivotX), FALSE);
					EnableWindow(GetDlgItem(Window, DialogScaleEditPivotY), FALSE);
					EnableWindow(GetDlgItem(Window, DialogScaleEditPivotZ), FALSE);
					return TRUE;
				}

				case DialogScaleButtonOk:
				{
					StaticScaleInfo.Scaling.x = GetFloat(GetDlgItem(Window, DialogScaleEditX), 1.0f);
					StaticScaleInfo.Scaling.y = GetFloat(GetDlgItem(Window, DialogScaleEditY), 1.0f);
					StaticScaleInfo.Scaling.z = GetFloat(GetDlgItem(Window, DialogScaleEditZ), 1.0f);
					StaticScaleInfo.PivotPoint.x = GetFloat(GetDlgItem(Window, DialogScaleEditPivotX), 0.0f);
					StaticScaleInfo.PivotPoint.y = GetFloat(GetDlgItem(Window, DialogScaleEditPivotY), 0.0f);
					StaticScaleInfo.PivotPoint.z = GetFloat(GetDlgItem(Window, DialogScaleEditPivotZ), 0.0f);

					StaticScaleInfo.AroundPivotPoint = CheckStateToBool(IsDlgButtonChecked(Window, DialogScaleButtonUsePivotPoint));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogScaleButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogScaleButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

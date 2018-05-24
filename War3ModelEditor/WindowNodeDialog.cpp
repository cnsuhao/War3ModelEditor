//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowNodeDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_NODE_DIALOG NodeDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_BASE_DATA WINDOW_NODE_DIALOG::StaticData;
BOOL WINDOW_NODE_DIALOG::StaticNoNameChange;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_NODE_DIALOG::WINDOW_NODE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_NODE_DIALOG::~WINDOW_NODE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_NODE_DIALOG::Display(HWND ParentWindow, MODEL_BASE_DATA& Data, BOOL NoNameChange) CONST
{
	StaticData = Data;
	StaticNoNameChange = NoNameChange;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogNode), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_BASE_DATA();
		return TRUE;
	}

	StaticData = MODEL_BASE_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_NODE_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetString(GetDlgItem(Window, DialogNodeEditName), StaticData.Name);
			SetFloat(GetDlgItem(Window, DialogNodeEditX), StaticData.PivotPoint.x);
			SetFloat(GetDlgItem(Window, DialogNodeEditY), StaticData.PivotPoint.y);
			SetFloat(GetDlgItem(Window, DialogNodeEditZ), StaticData.PivotPoint.z);
			CheckDlgButton(Window, DialogNodeButtonDontInheritTranslation, BoolToCheckState(StaticData.DontInheritTranslation));
			CheckDlgButton(Window, DialogNodeButtonDontInheritRotation, BoolToCheckState(StaticData.DontInheritRotation));
			CheckDlgButton(Window, DialogNodeButtonDontInheritScaling, BoolToCheckState(StaticData.DontInheritScaling));
			CheckDlgButton(Window, DialogNodeButtonBillboarded, BoolToCheckState(StaticData.Billboarded));
			CheckDlgButton(Window, DialogNodeButtonBillboardedLockX, BoolToCheckState(StaticData.BillboardedLockX));
			CheckDlgButton(Window, DialogNodeButtonBillboardedLockY, BoolToCheckState(StaticData.BillboardedLockY));
			CheckDlgButton(Window, DialogNodeButtonBillboardedLockZ, BoolToCheckState(StaticData.BillboardedLockZ));
			CheckDlgButton(Window, DialogNodeButtonCameraAnchored, BoolToCheckState(StaticData.CameraAnchored));

			if(StaticNoNameChange) EnableWindow(GetDlgItem(Window, DialogNodeEditName), FALSE);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogNodeButtonTranslation:
				{
					InterpolatorDialog.Display(Window, StaticData.Translation, "Translation");
					return TRUE;
				}

				case DialogNodeButtonRotation:
				{
					InterpolatorDialog.Display(Window, StaticData.Rotation, "Rotation");
					return TRUE;
				}

				case DialogNodeButtonScaling:
				{
					InterpolatorDialog.Display(Window, StaticData.Scaling, "Scaling");
					return TRUE;
				}

				case DialogNodeButtonOk:
				{
					StaticData.Name = GetString(GetDlgItem(Window, DialogNodeEditName));
					if(StaticData.Name == "")
					{
						Error.SetMessage("You must enter a name!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogNodeEditName));
						::SendMessage(GetDlgItem(Window, DialogNodeEditName), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticData.PivotPoint.x = GetFloat(GetDlgItem(Window, DialogNodeEditX));
					StaticData.PivotPoint.y = GetFloat(GetDlgItem(Window, DialogNodeEditY));
					StaticData.PivotPoint.z = GetFloat(GetDlgItem(Window, DialogNodeEditZ));
					StaticData.DontInheritTranslation = CheckStateToBool(IsDlgButtonChecked(Window, DialogNodeButtonDontInheritTranslation));
					StaticData.DontInheritRotation = CheckStateToBool(IsDlgButtonChecked(Window, DialogNodeButtonDontInheritRotation));
					StaticData.DontInheritScaling = CheckStateToBool(IsDlgButtonChecked(Window, DialogNodeButtonDontInheritScaling));
					StaticData.Billboarded = CheckStateToBool(IsDlgButtonChecked(Window, DialogNodeButtonBillboarded));
					StaticData.BillboardedLockX = CheckStateToBool(IsDlgButtonChecked(Window, DialogNodeButtonBillboardedLockX));
					StaticData.BillboardedLockY = CheckStateToBool(IsDlgButtonChecked(Window, DialogNodeButtonBillboardedLockY));
					StaticData.BillboardedLockZ = CheckStateToBool(IsDlgButtonChecked(Window, DialogNodeButtonBillboardedLockZ));
					StaticData.CameraAnchored = CheckStateToBool(IsDlgButtonChecked(Window, DialogNodeButtonCameraAnchored));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogNodeButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogNodeButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

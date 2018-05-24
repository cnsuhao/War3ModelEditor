//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowCameraDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_CAMERA_DIALOG CameraDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_CAMERA_DATA WINDOW_CAMERA_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_CAMERA_DIALOG::WINDOW_CAMERA_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_CAMERA_DIALOG::~WINDOW_CAMERA_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_CAMERA_DIALOG::Display(HWND ParentWindow, MODEL_CAMERA_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogCamera), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_CAMERA_DATA();
		return TRUE;
	}

	StaticData = MODEL_CAMERA_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_CAMERA_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetString(GetDlgItem(Window, DialogCameraEditName), StaticData.Name);
			SetFloat(GetDlgItem(Window, DialogCameraEditSourceX), StaticData.Source.x);
			SetFloat(GetDlgItem(Window, DialogCameraEditSourceY), StaticData.Source.y);
			SetFloat(GetDlgItem(Window, DialogCameraEditSourceZ), StaticData.Source.z);
			SetFloat(GetDlgItem(Window, DialogCameraEditTargetX), StaticData.Target.x);
			SetFloat(GetDlgItem(Window, DialogCameraEditTargetY), StaticData.Target.y);
			SetFloat(GetDlgItem(Window, DialogCameraEditTargetZ), StaticData.Target.z);
			SetFloat(GetDlgItem(Window, DialogCameraEditFieldOfView), StaticData.FieldOfView);
			SetFloat(GetDlgItem(Window, DialogCameraEditNearDistance), StaticData.NearDistance);
			SetFloat(GetDlgItem(Window, DialogCameraEditFarDistance), StaticData.FarDistance);

			CheckDlgButton(Window, DialogCameraButtonAnimatedPosition, BoolToCheckState(!StaticData.SourceTranslation.IsStatic()));
			CheckDlgButton(Window, DialogCameraButtonAnimatedTarget, BoolToCheckState(!StaticData.TargetTranslation.IsStatic()));
			CheckDlgButton(Window, DialogCameraButtonAnimatedRotation, BoolToCheckState(!StaticData.Rotation.IsStatic()));

			EnableWindow(GetDlgItem(Window, DialogCameraButtonPosition), CheckStateToBool(IsDlgButtonChecked(Window, DialogCameraButtonAnimatedPosition)));
			EnableWindow(GetDlgItem(Window, DialogCameraButtonTarget), CheckStateToBool(IsDlgButtonChecked(Window, DialogCameraButtonAnimatedTarget)));
			EnableWindow(GetDlgItem(Window, DialogCameraButtonRotation), CheckStateToBool(IsDlgButtonChecked(Window, DialogCameraButtonAnimatedRotation)));

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogCameraButtonPosition:
				{
					InterpolatorDialog.Display(Window, StaticData.SourceTranslation, "Position Translation");
					return TRUE;
				}

				case DialogCameraButtonTarget:
				{
					InterpolatorDialog.Display(Window, StaticData.TargetTranslation, "Target Position Translation");
					return TRUE;
				}

				case DialogCameraButtonRotation:
				{
					InterpolatorDialog.Display(Window, StaticData.Rotation, "Camera Rotation");
					return TRUE;
				}

				case DialogCameraButtonAnimatedPosition:
				{
					EnableWindow(GetDlgItem(Window, DialogCameraButtonPosition), CheckStateToBool(IsDlgButtonChecked(Window, DialogCameraButtonAnimatedPosition)));
					return TRUE;
				}

				case DialogCameraButtonAnimatedTarget:
				{
					EnableWindow(GetDlgItem(Window, DialogCameraButtonTarget), CheckStateToBool(IsDlgButtonChecked(Window, DialogCameraButtonAnimatedTarget)));
					return TRUE;
				}

				case DialogCameraButtonAnimatedRotation:
				{
					EnableWindow(GetDlgItem(Window, DialogCameraButtonRotation), CheckStateToBool(IsDlgButtonChecked(Window, DialogCameraButtonAnimatedRotation)));
					return TRUE;
				}

				case DialogCameraButtonOk:
				{
					StaticData.Name = GetString(GetDlgItem(Window, DialogCameraEditName));
					if(StaticData.Name == "")
					{
						Error.SetMessage("You must enter a name!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogCameraEditName));
						::SendMessage(GetDlgItem(Window, DialogCameraEditName), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticData.Source.x = GetFloat(GetDlgItem(Window, DialogCameraEditSourceX));
					StaticData.Source.y = GetFloat(GetDlgItem(Window, DialogCameraEditSourceY));
					StaticData.Source.z = GetFloat(GetDlgItem(Window, DialogCameraEditSourceZ));
					StaticData.Target.x = GetFloat(GetDlgItem(Window, DialogCameraEditTargetX));
					StaticData.Target.y = GetFloat(GetDlgItem(Window, DialogCameraEditTargetY));
					StaticData.Target.z = GetFloat(GetDlgItem(Window, DialogCameraEditTargetZ));
					StaticData.FieldOfView = GetFloat(GetDlgItem(Window, DialogCameraEditFieldOfView));
					StaticData.NearDistance = GetFloat(GetDlgItem(Window, DialogCameraEditNearDistance));
					StaticData.FarDistance = GetFloat(GetDlgItem(Window, DialogCameraEditFarDistance));

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogCameraButtonAnimatedPosition)))
					{
						StaticData.SourceTranslation.SetStaticVector3(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogCameraButtonAnimatedTarget)))
					{
						StaticData.TargetTranslation.SetStaticVector3(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogCameraButtonAnimatedRotation)))
					{
						StaticData.Rotation.SetStaticScalar(0.0f);
					}

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogCameraButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogCameraButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

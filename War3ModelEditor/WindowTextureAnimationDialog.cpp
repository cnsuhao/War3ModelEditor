//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowTextureAnimationDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_TEXTURE_ANIMATION_DIALOG TextureAnimationDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_TEXTURE_ANIMATION_DATA WINDOW_TEXTURE_ANIMATION_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_TEXTURE_ANIMATION_DIALOG::WINDOW_TEXTURE_ANIMATION_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_TEXTURE_ANIMATION_DIALOG::~WINDOW_TEXTURE_ANIMATION_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_TEXTURE_ANIMATION_DIALOG::Display(HWND ParentWindow, MODEL_TEXTURE_ANIMATION_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogTextureAnimation), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_TEXTURE_ANIMATION_DATA();
		return TRUE;
	}

	StaticData = MODEL_TEXTURE_ANIMATION_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_TEXTURE_ANIMATION_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			CheckDlgButton(Window, DialogTextureAnimationButtonAnimatedTranslation, BoolToCheckState(!StaticData.Translation.IsStatic()));
			CheckDlgButton(Window, DialogTextureAnimationButtonAnimatedRotation, BoolToCheckState(!StaticData.Rotation.IsStatic()));
			CheckDlgButton(Window, DialogTextureAnimationButtonAnimatedScaling, BoolToCheckState(!StaticData.Scaling.IsStatic()));

			EnableWindow(GetDlgItem(Window, DialogTextureAnimationButtonTranslation), CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureAnimationButtonAnimatedTranslation)));
			EnableWindow(GetDlgItem(Window, DialogTextureAnimationButtonRotation), CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureAnimationButtonAnimatedRotation)));
			EnableWindow(GetDlgItem(Window, DialogTextureAnimationButtonScaling), CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureAnimationButtonAnimatedScaling)));

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogTextureAnimationButtonTranslation:
				{
					InterpolatorDialog.Display(Window, StaticData.Translation, "Translation");
					return TRUE;
				}

				case DialogTextureAnimationButtonRotation:
				{
					InterpolatorDialog.Display(Window, StaticData.Rotation, "Rotation");
					return TRUE;
				}

				case DialogTextureAnimationButtonScaling:
				{
					InterpolatorDialog.Display(Window, StaticData.Scaling, "Scaling");
					return TRUE;
				}

				case DialogTextureAnimationButtonAnimatedTranslation:
				{
					EnableWindow(GetDlgItem(Window, DialogTextureAnimationButtonTranslation), CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureAnimationButtonAnimatedTranslation)));
					return TRUE;
				}

				case DialogTextureAnimationButtonAnimatedRotation:
				{
					EnableWindow(GetDlgItem(Window, DialogTextureAnimationButtonRotation), CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureAnimationButtonAnimatedRotation)));
					return TRUE;
				}

				case DialogTextureAnimationButtonAnimatedScaling:
				{
					EnableWindow(GetDlgItem(Window, DialogTextureAnimationButtonScaling), CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureAnimationButtonAnimatedScaling)));
					return TRUE;
				}

				case DialogTextureAnimationButtonOk:
				{
					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureAnimationButtonAnimatedTranslation)))
					{
						StaticData.Translation.SetStaticVector3(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureAnimationButtonAnimatedRotation)))
					{
						StaticData.Rotation.SetStaticVector4(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureAnimationButtonAnimatedScaling)))
					{
						StaticData.Scaling.SetStaticVector3(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
					}

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogTextureAnimationButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogTextureAnimationButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

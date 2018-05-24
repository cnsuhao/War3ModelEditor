//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowTextureDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_TEXTURE_DIALOG TextureDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_TEXTURE_DATA WINDOW_TEXTURE_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_TEXTURE_DIALOG::WINDOW_TEXTURE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_TEXTURE_DIALOG::~WINDOW_TEXTURE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_TEXTURE_DIALOG::Display(HWND ParentWindow, MODEL_TEXTURE_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogTexture), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_TEXTURE_DATA();
		return TRUE;
	}

	StaticData = MODEL_TEXTURE_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_TEXTURE_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetString(GetDlgItem(Window, DialogTextureEditName), StaticData.FileName);
			SetInt(GetDlgItem(Window, DialogTextureEditReplaceableId), StaticData.ReplaceableId);
			CheckDlgButton(Window, DialogTextureButtonWrapWidth, BoolToCheckState(StaticData.WrapWidth));
			CheckDlgButton(Window, DialogTextureButtonWrapHeight, BoolToCheckState(StaticData.WrapHeight));

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogTextureButtonOk:
				{
					StaticData.ReplaceableId = GetInt(GetDlgItem(Window, DialogTextureEditReplaceableId));
					StaticData.WrapWidth = CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureButtonWrapWidth));
					StaticData.WrapHeight = CheckStateToBool(IsDlgButtonChecked(Window, DialogTextureButtonWrapHeight));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogTextureButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogTextureButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

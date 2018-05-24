//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowLoadingScreenDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_LOADING_SCREEN_DIALOG LoadingScreenDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
std::string WINDOW_LOADING_SCREEN_DIALOG::StaticTextureFileName = "SomeTexture.blp";


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_LOADING_SCREEN_DIALOG::WINDOW_LOADING_SCREEN_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_LOADING_SCREEN_DIALOG::~WINDOW_LOADING_SCREEN_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_LOADING_SCREEN_DIALOG::Display(HWND ParentWindow, std::string& TextureFileName) CONST
{
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogLoadingScreen), ParentWindow, DialogMessageHandler, 0))
	{
		TextureFileName = StaticTextureFileName;
		return TRUE;
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_LOADING_SCREEN_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			CenterWindow(Window);

			SetString(GetDlgItem(Window, DialogLoadingScreenEditTextureFileName), StaticTextureFileName);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogLoadingScreenButtonPick:
				{
					std::string FileName;
					WINDOW_FILE_DIALOG FileDialog;

					FileDialog.SetTitle("Open Texture");
					FileDialog.SetFilter(FILTER_TEXTURE);
					if(!FileDialog.OpenFile(Window, FileName)) return TRUE;

					StaticTextureFileName = FileName;
					SetString(GetDlgItem(Window, DialogLoadingScreenEditTextureFileName), StaticTextureFileName);

					return TRUE;
				}

				case DialogLoadingScreenButtonGenerate:
				{
					std::string FileName;
					WINDOW_FILE_DIALOG FileDialog;

					StaticTextureFileName = GetString(GetDlgItem(Window, DialogLoadingScreenEditTextureFileName));
					FileName = Common.GetName(StaticTextureFileName) + "512x512.blp";

					FileDialog.SetTitle("Save 512x512 Texture");
					FileDialog.SetFilter(FILTER_TEXTURE);
					if(!FileDialog.SaveFile(Window, FileName)) return TRUE;

					if(!TextureManager.ResizeTexture(StaticTextureFileName, FileName, 512, 512))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return 0;
					}

					return TRUE;
				}

				case DialogLoadingScreenButtonOk:
				{
					StaticTextureFileName = GetString(GetDlgItem(Window, DialogLoadingScreenEditTextureFileName));

					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogLoadingScreenButtonCancel:
				{
					EndDialog(Window, 0);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogLoadingScreenButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

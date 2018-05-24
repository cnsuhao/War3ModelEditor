//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowTranslateDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_TRANSLATE_DIALOG TranslateDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
TRANSLATE_INFO WINDOW_TRANSLATE_DIALOG::StaticTranslateInfo;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_TRANSLATE_DIALOG::WINDOW_TRANSLATE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_TRANSLATE_DIALOG::~WINDOW_TRANSLATE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_TRANSLATE_DIALOG::Display(HWND ParentWindow, TRANSLATE_INFO& TranslateInfo) CONST
{
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogTranslate), ParentWindow, DialogMessageHandler, 0))
	{
		TranslateInfo = StaticTranslateInfo;
		return TRUE;
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_TRANSLATE_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetFloat(GetDlgItem(Window, DialogTranslateEditX), StaticTranslateInfo.Translation.x);
			SetFloat(GetDlgItem(Window, DialogTranslateEditY), StaticTranslateInfo.Translation.y);
			SetFloat(GetDlgItem(Window, DialogTranslateEditZ), StaticTranslateInfo.Translation.z);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogTranslateButtonOk:
				{
					StaticTranslateInfo.Translation.x = GetFloat(GetDlgItem(Window, DialogTranslateEditX), 0.0f);
					StaticTranslateInfo.Translation.y = GetFloat(GetDlgItem(Window, DialogTranslateEditY), 0.0f);
					StaticTranslateInfo.Translation.z = GetFloat(GetDlgItem(Window, DialogTranslateEditZ), 0.0f);

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogTranslateButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogTranslateButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

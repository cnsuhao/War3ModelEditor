//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowQualityDialog.h"

//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_QUALITY_DIALOG QualityDialog;



//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
INT WINDOW_QUALITY_DIALOG::StaticQuality = 80;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_QUALITY_DIALOG::WINDOW_QUALITY_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_QUALITY_DIALOG::~WINDOW_QUALITY_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_QUALITY_DIALOG::Display(HWND ParentWindow, INT& Quality) CONST
{
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogQuality), ParentWindow, DialogMessageHandler, 0))
	{
		Quality = StaticQuality;
		return TRUE;
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_QUALITY_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			CenterWindow(Window);

			SetDlgItemInt(Window, DialogQualityEdit, StaticQuality, FALSE);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogQualityButtonOk:
				{
					INT Quality;

					Quality = GetDlgItemInt(Window, DialogQualityEdit, NULL, FALSE);
					if((Quality < 0) || (Quality > 100))
					{
						Error.SetMessage("The quality must be between 0 and 100!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					StaticQuality = Quality;

					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogQualityButtonCancel:
				{
					EndDialog(Window, 0);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogQualityButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

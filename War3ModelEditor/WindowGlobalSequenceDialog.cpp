//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowGlobalSequenceDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_GLOBAL_SEQUENCE_DIALOG GlobalSequenceDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_GLOBAL_SEQUENCE_DATA WINDOW_GLOBAL_SEQUENCE_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_GLOBAL_SEQUENCE_DIALOG::WINDOW_GLOBAL_SEQUENCE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_GLOBAL_SEQUENCE_DIALOG::~WINDOW_GLOBAL_SEQUENCE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_GLOBAL_SEQUENCE_DIALOG::Display(HWND ParentWindow, MODEL_GLOBAL_SEQUENCE_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogGlobalSequence), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_GLOBAL_SEQUENCE_DATA();
		return TRUE;
	}

	StaticData = MODEL_GLOBAL_SEQUENCE_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_GLOBAL_SEQUENCE_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetInt(GetDlgItem(Window, DialogGlobalSequenceEditDuration), StaticData.Duration);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogGlobalSequenceButtonOk:
				{
					StaticData.Duration = GetInt(GetDlgItem(Window, DialogGlobalSequenceEditDuration));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogGlobalSequenceButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogGlobalSequenceButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

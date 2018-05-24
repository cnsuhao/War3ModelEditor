//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowExtentDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_EXTENT_DIALOG ExtentDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
EXTENT* WINDOW_EXTENT_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_EXTENT_DIALOG::WINDOW_EXTENT_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_EXTENT_DIALOG::~WINDOW_EXTENT_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_EXTENT_DIALOG::Display(HWND ParentWindow, EXTENT* Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogExtent), ParentWindow, DialogMessageHandler, 0))
	{
		StaticData = NULL;
		return TRUE;
	}

	StaticData = NULL;
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_EXTENT_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetFloat(GetDlgItem(Window, DialogExtentEditMinX), StaticData->Min.x);
			SetFloat(GetDlgItem(Window, DialogExtentEditMinY), StaticData->Min.y);
			SetFloat(GetDlgItem(Window, DialogExtentEditMinZ), StaticData->Min.z);
			SetFloat(GetDlgItem(Window, DialogExtentEditMaxX), StaticData->Max.x);
			SetFloat(GetDlgItem(Window, DialogExtentEditMaxY), StaticData->Max.y);
			SetFloat(GetDlgItem(Window, DialogExtentEditMaxZ), StaticData->Max.z);
			SetFloat(GetDlgItem(Window, DialogExtentEditBoundsRadius), StaticData->Radius);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogExtentButtonOk:
				{
					StaticData->Min.x = GetFloat(GetDlgItem(Window, DialogExtentEditMinX));
					StaticData->Min.y = GetFloat(GetDlgItem(Window, DialogExtentEditMinY));
					StaticData->Min.z = GetFloat(GetDlgItem(Window, DialogExtentEditMinZ));
					StaticData->Max.x = GetFloat(GetDlgItem(Window, DialogExtentEditMaxX));
					StaticData->Max.y = GetFloat(GetDlgItem(Window, DialogExtentEditMaxY));
					StaticData->Max.z = GetFloat(GetDlgItem(Window, DialogExtentEditMaxZ));
					StaticData->Radius = GetFloat(GetDlgItem(Window, DialogExtentEditBoundsRadius));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogExtentButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogExtentButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

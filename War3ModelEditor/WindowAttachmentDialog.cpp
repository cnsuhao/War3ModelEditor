//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowAttachmentDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_ATTACHMENT_DIALOG AttachmentDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_ATTACHMENT_DATA WINDOW_ATTACHMENT_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_ATTACHMENT_DIALOG::WINDOW_ATTACHMENT_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_ATTACHMENT_DIALOG::~WINDOW_ATTACHMENT_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_ATTACHMENT_DIALOG::Display(HWND ParentWindow, MODEL_ATTACHMENT_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogAttachment), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_ATTACHMENT_DATA();
		return TRUE;
	}

	StaticData = MODEL_ATTACHMENT_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_ATTACHMENT_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetString(GetDlgItem(Window, DialogAttachmentEditPath), StaticData.Path);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogAttachmentButtonVisibility:
				{
					InterpolatorDialog.Display(Window, StaticData.Visibility, "Visibility");
					return TRUE;
				}

				case DialogAttachmentButtonOk:
				{
					StaticData.Path = GetString(GetDlgItem(Window, DialogAttachmentEditPath));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogAttachmentButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogAttachmentButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowSequenceDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_SEQUENCE_DIALOG SequenceDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_SEQUENCE_DATA WINDOW_SEQUENCE_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_SEQUENCE_DIALOG::WINDOW_SEQUENCE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_SEQUENCE_DIALOG::~WINDOW_SEQUENCE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_SEQUENCE_DIALOG::Display(HWND ParentWindow, MODEL_SEQUENCE_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogSequence), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_SEQUENCE_DATA();
		return TRUE;
	}

	StaticData = MODEL_SEQUENCE_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_SEQUENCE_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetString(GetDlgItem(Window, DialogSequenceEditName), StaticData.Name);
			SetFloat(GetDlgItem(Window, DialogSequenceEditFrom), StaticData.Interval.x);
			SetFloat(GetDlgItem(Window, DialogSequenceEditTo), StaticData.Interval.y);
			SetFloat(GetDlgItem(Window, DialogSequenceEditRarity), StaticData.Rarity);
			SetFloat(GetDlgItem(Window, DialogSequenceEditMoveSpeed), StaticData.MoveSpeed);
			SetFloat(GetDlgItem(Window, DialogSequenceEditBoundsRadius), StaticData.Extent.Radius);
			SetFloat(GetDlgItem(Window, DialogSequenceEditMinX), StaticData.Extent.Min.x);
			SetFloat(GetDlgItem(Window, DialogSequenceEditMinY), StaticData.Extent.Min.y);
			SetFloat(GetDlgItem(Window, DialogSequenceEditMinZ), StaticData.Extent.Min.z);
			SetFloat(GetDlgItem(Window, DialogSequenceEditMaxX), StaticData.Extent.Max.x);
			SetFloat(GetDlgItem(Window, DialogSequenceEditMaxY), StaticData.Extent.Max.y);
			SetFloat(GetDlgItem(Window, DialogSequenceEditMaxZ), StaticData.Extent.Max.z);
			CheckDlgButton(Window, DialogSequenceButtonNonLooping, BoolToCheckState(StaticData.NonLooping));

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogSequenceButtonOk:
				{
					StaticData.Name = GetString(GetDlgItem(Window, DialogSequenceEditName));
					if(StaticData.Name == "")
					{
						Error.SetMessage("You must enter a name!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogSequenceEditName));
						::SendMessage(GetDlgItem(Window, DialogSequenceEditName), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticData.Interval.x = GetFloat(GetDlgItem(Window, DialogSequenceEditFrom));
					StaticData.Interval.y = GetFloat(GetDlgItem(Window, DialogSequenceEditTo));
					if(StaticData.Interval.x > StaticData.Interval.y)
					{
						Error.SetMessage("The interval cannot be negative!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogSequenceEditFrom));
						::SendMessage(GetDlgItem(Window, DialogSequenceEditFrom), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticData.Rarity = GetFloat(GetDlgItem(Window, DialogSequenceEditRarity));
					StaticData.MoveSpeed = GetFloat(GetDlgItem(Window, DialogSequenceEditMoveSpeed));
					StaticData.Extent.Radius = GetFloat(GetDlgItem(Window, DialogSequenceEditBoundsRadius));
					StaticData.Extent.Min.x = GetFloat(GetDlgItem(Window, DialogSequenceEditMinX));
					StaticData.Extent.Min.y = GetFloat(GetDlgItem(Window, DialogSequenceEditMinY));
					StaticData.Extent.Min.z = GetFloat(GetDlgItem(Window, DialogSequenceEditMinZ));
					StaticData.Extent.Max.x = GetFloat(GetDlgItem(Window, DialogSequenceEditMaxX));
					StaticData.Extent.Max.y = GetFloat(GetDlgItem(Window, DialogSequenceEditMaxY));
					StaticData.Extent.Max.z = GetFloat(GetDlgItem(Window, DialogSequenceEditMaxZ));
					StaticData.NonLooping = CheckStateToBool(IsDlgButtonChecked(Window, DialogSequenceButtonNonLooping));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogSequenceButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogSequenceButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

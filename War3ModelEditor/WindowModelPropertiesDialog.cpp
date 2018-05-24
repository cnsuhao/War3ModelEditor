//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowModelPropertiesDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_MODEL_PROPERTIES_DIALOG ModelPropertiesDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_INFO_DATA WINDOW_MODEL_PROPERTIES_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_MODEL_PROPERTIES_DIALOG::WINDOW_MODEL_PROPERTIES_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_MODEL_PROPERTIES_DIALOG::~WINDOW_MODEL_PROPERTIES_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_MODEL_PROPERTIES_DIALOG::Display(HWND ParentWindow, MODEL_INFO_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogModelProperties), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_INFO_DATA();
		return TRUE;
	}

	StaticData = MODEL_INFO_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_MODEL_PROPERTIES_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetString(GetDlgItem(Window, DialogModelPropertiesEditName), StaticData.Name);
			SetString(GetDlgItem(Window, DialogModelPropertiesEditAnimationFile), StaticData.AnimationFile);
			SetInt(GetDlgItem(Window, DialogModelPropertiesEditBlendTime), StaticData.BlendTime);
			SetFloat(GetDlgItem(Window, DialogModelPropertiesEditBoundsRadius), StaticData.Extent.Radius);
			SetFloat(GetDlgItem(Window, DialogModelPropertiesEditMinX), StaticData.Extent.Min.x);
			SetFloat(GetDlgItem(Window, DialogModelPropertiesEditMinY), StaticData.Extent.Min.y);
			SetFloat(GetDlgItem(Window, DialogModelPropertiesEditMinZ), StaticData.Extent.Min.z);
			SetFloat(GetDlgItem(Window, DialogModelPropertiesEditMaxX), StaticData.Extent.Max.x);
			SetFloat(GetDlgItem(Window, DialogModelPropertiesEditMaxY), StaticData.Extent.Max.y);
			SetFloat(GetDlgItem(Window, DialogModelPropertiesEditMaxZ), StaticData.Extent.Max.z);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogModelPropertiesButtonOk:
				{
					StaticData.Name = GetString(GetDlgItem(Window, DialogModelPropertiesEditName));
					if(StaticData.Name == "")
					{
						Error.SetMessage("You must enter a name!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogModelPropertiesEditName));
						::SendMessage(GetDlgItem(Window, DialogModelPropertiesEditName), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticData.AnimationFile = GetString(GetDlgItem(Window, DialogModelPropertiesEditAnimationFile));
					StaticData.BlendTime = GetInt(GetDlgItem(Window, DialogModelPropertiesEditBlendTime));
					StaticData.Extent.Radius = GetFloat(GetDlgItem(Window, DialogModelPropertiesEditBoundsRadius));
					StaticData.Extent.Min.x = GetFloat(GetDlgItem(Window, DialogModelPropertiesEditMinX));
					StaticData.Extent.Min.y = GetFloat(GetDlgItem(Window, DialogModelPropertiesEditMinY));
					StaticData.Extent.Min.z = GetFloat(GetDlgItem(Window, DialogModelPropertiesEditMinZ));
					StaticData.Extent.Max.x = GetFloat(GetDlgItem(Window, DialogModelPropertiesEditMaxX));
					StaticData.Extent.Max.y = GetFloat(GetDlgItem(Window, DialogModelPropertiesEditMaxY));
					StaticData.Extent.Max.z = GetFloat(GetDlgItem(Window, DialogModelPropertiesEditMaxZ));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogModelPropertiesButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogModelPropertiesButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

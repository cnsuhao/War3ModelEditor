//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowVertexDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_VERTEX_DIALOG VertexDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_GEOSET_VERTEX* WINDOW_VERTEX_DIALOG::StaticData;
MODEL_GEOSET* WINDOW_VERTEX_DIALOG::StaticGeoset;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_VERTEX_DIALOG::WINDOW_VERTEX_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_VERTEX_DIALOG::~WINDOW_VERTEX_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_VERTEX_DIALOG::Display(HWND ParentWindow, MODEL_GEOSET* Geoset, MODEL_GEOSET_VERTEX* Data) CONST
{
	StaticData = Data;
	StaticGeoset = Geoset;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogVertex), ParentWindow, DialogMessageHandler, 0))
	{
		StaticData = NULL;
		StaticGeoset = NULL;
		return TRUE;
	}

	StaticData = NULL;
	StaticGeoset = NULL;
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_VERTEX_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			BuildGroupIdList(Window);

			SetFloat(GetDlgItem(Window, DialogVertexEditX), StaticData->Position.x);
			SetFloat(GetDlgItem(Window, DialogVertexEditY), StaticData->Position.y);
			SetFloat(GetDlgItem(Window, DialogVertexEditZ), StaticData->Position.z);
			SetFloat(GetDlgItem(Window, DialogVertexEditNormalX), StaticData->Normal.x);
			SetFloat(GetDlgItem(Window, DialogVertexEditNormalY), StaticData->Normal.y);
			SetFloat(GetDlgItem(Window, DialogVertexEditNormalZ), StaticData->Normal.z);
			SetFloat(GetDlgItem(Window, DialogVertexEditTextureX), StaticData->TexturePosition.x);
			SetFloat(GetDlgItem(Window, DialogVertexEditTextureY), StaticData->TexturePosition.y);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogVertexButtonOk:
				{
					StaticData->Position.x = GetFloat(GetDlgItem(Window, DialogVertexEditX));
					StaticData->Position.y = GetFloat(GetDlgItem(Window, DialogVertexEditY));
					StaticData->Position.z = GetFloat(GetDlgItem(Window, DialogVertexEditZ));
					StaticData->Normal.x = GetFloat(GetDlgItem(Window, DialogVertexEditNormalX));
					StaticData->Normal.y = GetFloat(GetDlgItem(Window, DialogVertexEditNormalY));
					StaticData->Normal.z = GetFloat(GetDlgItem(Window, DialogVertexEditNormalZ));
					StaticData->TexturePosition.x = GetFloat(GetDlgItem(Window, DialogVertexEditTextureX));
					StaticData->TexturePosition.y = GetFloat(GetDlgItem(Window, DialogVertexEditTextureY));

					StaticData->VertexGroup = static_cast<INT>(::SendMessage(GetDlgItem(Window, DialogVertexComboGroup), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogVertexComboGroup), CB_GETCURSEL, 0, 0), 0));

					Model.Rebuild();

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogVertexButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogVertexButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the group ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_VERTEX_DIALOG::BuildGroupIdList(HWND Window)
{
	INT i;
	INT Index;
	INT CurrentGroupId;
	INT SelectedGroupId;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogVertexComboGroup);
	if(ComboBox == NULL) return;

	Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(NAME_NO_ID.c_str())));
	::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
	::SendMessage(ComboBox, CB_SETCURSEL, 0, 0);

	CurrentGroupId = 0;
	SelectedGroupId = StaticData->VertexGroup;

	for(i = 0; i < StaticGeoset->Data().GroupContainer.GetTotalSize(); i++)
	{
		if(StaticGeoset->Data().GroupContainer.ValidIndex(i))
		{
			std::stringstream Stream;

			Stream << "Group " << StaticGeoset->Data().GroupContainer[i];
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Stream.str().c_str())));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, static_cast<LPARAM>(CurrentGroupId));

			if(CurrentGroupId == SelectedGroupId)
			{
				::SendMessage(ComboBox, CB_SETCURSEL, Index, 0);
			}
		}

		CurrentGroupId++;
	}
}

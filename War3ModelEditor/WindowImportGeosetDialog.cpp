//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowImportGeosetDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_IMPORT_GEOSET_DIALOG ImportGeosetDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_BASE* WINDOW_IMPORT_GEOSET_DIALOG::StaticNode;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_IMPORT_GEOSET_DIALOG::WINDOW_IMPORT_GEOSET_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_IMPORT_GEOSET_DIALOG::~WINDOW_IMPORT_GEOSET_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_IMPORT_GEOSET_DIALOG::Display(HWND ParentWindow, MODEL_BASE*& Node) CONST
{
	StaticNode = Node;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogImportGeoset), ParentWindow, DialogMessageHandler, 0))
	{
		Node = StaticNode;
		return TRUE;
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_IMPORT_GEOSET_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			BuildNodeIdList(Window);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogImportGeosetButtonOk:
				{
					StaticNode = reinterpret_cast<MODEL_BASE*>(::SendMessage(GetDlgItem(Window, DialogImportGeosetCombo), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogImportGeosetCombo), CB_GETCURSEL, 0, 0), 0));
					if(StaticNode == NULL)
					{
						Error.SetMessage("You have to select a node to attach the geoset to!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogImportGeosetButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogImportGeosetButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the node ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_IMPORT_GEOSET_DIALOG::BuildNodeIdList(HWND Window)
{
	INT i;
	INT Index;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogImportGeosetCombo);
	if(ComboBox == NULL) return;

	Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(NAME_NO_ID.c_str())));
	::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
	::SendMessage(ComboBox, CB_SETCURSEL, 0, 0);

	for(i = 0; i < Model.Data().BaseContainer.GetTotalSize(); i++)
	{
		if(Model.Data().BaseContainer.ValidIndex(i))
		{
			std::stringstream Stream;

			Stream << Model.Data().BaseContainer[i]->BaseData()->Name;
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Stream.str().c_str())));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Model.Data().BaseContainer[i]));

			if(Model.Data().BaseContainer[i] == StaticNode)
			{
				::SendMessage(ComboBox, CB_SETCURSEL, Index, 0);
			}
		}
	}
}

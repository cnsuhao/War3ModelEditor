//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowGroupsDialog.h"
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_GROUPS_DIALOG GroupsDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
SIMPLE_CONTAINER<MODEL_GEOSET_GROUP*>* WINDOW_GROUPS_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_GROUPS_DIALOG::WINDOW_GROUPS_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_GROUPS_DIALOG::~WINDOW_GROUPS_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_GROUPS_DIALOG::Display(HWND ParentWindow, SIMPLE_CONTAINER<MODEL_GEOSET_GROUP*>* Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogGroups), ParentWindow, DialogMessageHandler, 0))
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
BOOL CALLBACK WINDOW_GROUPS_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT i;
			INT Index;
			HWND ListBox;
			MODEL_GEOSET_GROUP* Group;

			AddWindow(Window);
			CenterWindow(Window);

			ListBox = GetDlgItem(Window, DialogGroupsList);

			for(i = 0; i < StaticData->GetTotalSize(); i++)
			{
				if(StaticData->ValidIndex(i))
				{
					std::stringstream Stream;

					Group = (*StaticData)[i];

					Stream << "Group " << Group;
					Index = static_cast<INT>(::SendMessage(ListBox, LB_INSERTSTRING, -1, reinterpret_cast<LPARAM>(Stream.str().c_str())));
					::SendMessage(ListBox, LB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Group));
				}
			}

			return TRUE;
		}

		case WM_CONTEXTMENU:
		{
			INT Index;
			INT MenuId;
			INT NrOfItems;
			HWND ListBox;
			RECT TempRect;
			RECT ListRect;
			POINT MousePosition;
			POINT LocalMousePosition;

			ListBox = GetDlgItem(Window, DialogGroupsList);

			GetWindowRect(ListBox, &ListRect);
			GetCursorPos(&MousePosition);
			LocalMousePosition = MousePosition;
			ScreenToClient(ListBox, &LocalMousePosition);

			if(!Common.PointInsideRect(MousePosition, ListRect)) return TRUE;

			MenuId = GroupMenu2;
			NrOfItems = static_cast<INT>(::SendMessage(ListBox, LB_GETCOUNT, 0, 0));

			Index = 0;
			while(Index < NrOfItems)
			{
				::SendMessage(ListBox, LB_GETITEMRECT, Index, reinterpret_cast<LPARAM>(&TempRect));
				if(Common.PointInsideRect(LocalMousePosition, TempRect))
				{
					::SendMessage(ListBox, LB_SETCURSEL, Index, 0);
					MenuId = GroupMenu;
					break;
				}

				Index++;
			}

			TrackPopupMenuEx(GetSubMenu(LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(MenuId)), 0), TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePosition.x, MousePosition.y, Window, NULL);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogGroupsList:
				{
					switch(HIWORD(W))
					{
						case LBN_DBLCLK:
						{
							if(!Edit(Window))
							{
								Error.DisplayMessage(Window);
								Error.ClearMessage();
								return TRUE;
							}

							return TRUE;
						}
					}

					return TRUE;
				}

				case GroupFileCreate:
				case GroupFileCreate2:
				{
					if(!CreateNew(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case GroupFileRemove:
				case GroupFileRemove2:
				{
					if(!Remove(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case GroupFileEdit:
				case GroupFileEdit2:
				{
					if(!Edit(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case DialogGroupsButtonOk:
				{
					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogGroupsButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogGroupsButtonOk, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Creates a new group
//+-----------------------------------------------------------------------------
BOOL WINDOW_GROUPS_DIALOG::CreateNew(HWND Window)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing group
//+-----------------------------------------------------------------------------
BOOL WINDOW_GROUPS_DIALOG::Remove(HWND Window)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing group
//+-----------------------------------------------------------------------------
BOOL WINDOW_GROUPS_DIALOG::Edit(HWND Window)
{
	INT Index;
	HWND ListBox;
	MODEL_GEOSET_GROUP* Group;

	ListBox = GetDlgItem(Window, DialogGroupsList);

	Index = static_cast<INT>(::SendMessage(ListBox, LB_GETCURSEL, 0, 0));
	if(Index == LB_ERR) return TRUE;

	Group = reinterpret_cast<MODEL_GEOSET_GROUP*>(::SendMessage(ListBox, LB_GETITEMDATA, Index, 0));
	if(Group == NULL) return TRUE;

	if(!GroupDialog.Display(Window, Group)) return TRUE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}

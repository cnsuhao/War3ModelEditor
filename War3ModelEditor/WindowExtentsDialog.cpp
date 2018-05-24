//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowExtentsDialog.h"
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_EXTENTS_DIALOG ExtentsDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
SIMPLE_CONTAINER<EXTENT*>* WINDOW_EXTENTS_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_EXTENTS_DIALOG::WINDOW_EXTENTS_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_EXTENTS_DIALOG::~WINDOW_EXTENTS_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_EXTENTS_DIALOG::Display(HWND ParentWindow, SIMPLE_CONTAINER<EXTENT*>* Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogExtents), ParentWindow, DialogMessageHandler, 0))
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
BOOL CALLBACK WINDOW_EXTENTS_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT i;
			INT Index;
			HWND ListBox;
			EXTENT* Extent;

			AddWindow(Window);
			CenterWindow(Window);

			ListBox = GetDlgItem(Window, DialogExtentsList);

			for(i = 0; i < StaticData->GetTotalSize(); i++)
			{
				if(StaticData->ValidIndex(i))
				{
					std::stringstream Stream;

					Extent = (*StaticData)[i];

					Stream << "Extent " << Extent;
					Index = static_cast<INT>(::SendMessage(ListBox, LB_INSERTSTRING, -1, reinterpret_cast<LPARAM>(Stream.str().c_str())));
					::SendMessage(ListBox, LB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Extent));
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

			ListBox = GetDlgItem(Window, DialogExtentsList);

			GetWindowRect(ListBox, &ListRect);
			GetCursorPos(&MousePosition);
			LocalMousePosition = MousePosition;
			ScreenToClient(ListBox, &LocalMousePosition);

			if(!Common.PointInsideRect(MousePosition, ListRect)) return TRUE;

			MenuId = ExtentMenu2;
			NrOfItems = static_cast<INT>(::SendMessage(ListBox, LB_GETCOUNT, 0, 0));

			Index = 0;
			while(Index < NrOfItems)
			{
				::SendMessage(ListBox, LB_GETITEMRECT, Index, reinterpret_cast<LPARAM>(&TempRect));
				if(Common.PointInsideRect(LocalMousePosition, TempRect))
				{
					::SendMessage(ListBox, LB_SETCURSEL, Index, 0);
					MenuId = ExtentMenu;
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
				case DialogExtentsList:
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

				case ExtentFileCreate:
				case ExtentFileCreate2:
				{
					if(!CreateNew(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case ExtentFileRemove:
				case ExtentFileRemove2:
				{
					if(!Remove(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case ExtentFileEdit:
				case ExtentFileEdit2:
				{
					if(!Edit(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case DialogExtentsButtonOk:
				{
					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogExtentsButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogExtentsButtonOk, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Creates a new extent
//+-----------------------------------------------------------------------------
BOOL WINDOW_EXTENTS_DIALOG::CreateNew(HWND Window)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing extent
//+-----------------------------------------------------------------------------
BOOL WINDOW_EXTENTS_DIALOG::Remove(HWND Window)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing extent
//+-----------------------------------------------------------------------------
BOOL WINDOW_EXTENTS_DIALOG::Edit(HWND Window)
{
	INT Index;
	HWND ListBox;
	EXTENT* Extent;

	ListBox = GetDlgItem(Window, DialogExtentsList);

	Index = static_cast<INT>(::SendMessage(ListBox, LB_GETCURSEL, 0, 0));
	if(Index == LB_ERR) return TRUE;

	Extent = reinterpret_cast<EXTENT*>(::SendMessage(ListBox, LB_GETITEMDATA, Index, 0));
	if(Extent == NULL) return TRUE;

	if(!ExtentDialog.Display(Window, Extent)) return TRUE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}

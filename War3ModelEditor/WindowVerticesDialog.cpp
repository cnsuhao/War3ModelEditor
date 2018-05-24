//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowVerticesDialog.h"
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_VERTICES_DIALOG VerticesDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
SIMPLE_CONTAINER<MODEL_GEOSET_VERTEX*>* WINDOW_VERTICES_DIALOG::StaticData;
MODEL_GEOSET* WINDOW_VERTICES_DIALOG::StaticGeoset;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_VERTICES_DIALOG::WINDOW_VERTICES_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_VERTICES_DIALOG::~WINDOW_VERTICES_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_VERTICES_DIALOG::Display(HWND ParentWindow, MODEL_GEOSET* Geoset, SIMPLE_CONTAINER<MODEL_GEOSET_VERTEX*>* Data) CONST
{
	StaticData = Data;
	StaticGeoset = Geoset;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogVertices), ParentWindow, DialogMessageHandler, 0))
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
BOOL CALLBACK WINDOW_VERTICES_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT i;
			INT Index;
			HWND ListBox;
			MODEL_GEOSET_VERTEX* Vertex;

			AddWindow(Window);
			CenterWindow(Window);

			ListBox = GetDlgItem(Window, DialogVerticesList);

			for(i = 0; i < StaticData->GetTotalSize(); i++)
			{
				if(StaticData->ValidIndex(i))
				{
					std::stringstream Stream;

					Vertex = (*StaticData)[i];

					Stream << "Vertex " << Vertex;
					Index = static_cast<INT>(::SendMessage(ListBox, LB_INSERTSTRING, -1, reinterpret_cast<LPARAM>(Stream.str().c_str())));
					::SendMessage(ListBox, LB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Vertex));
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

			ListBox = GetDlgItem(Window, DialogVerticesList);

			GetWindowRect(ListBox, &ListRect);
			GetCursorPos(&MousePosition);
			LocalMousePosition = MousePosition;
			ScreenToClient(ListBox, &LocalMousePosition);

			if(!Common.PointInsideRect(MousePosition, ListRect)) return TRUE;

			MenuId = VertexMenu2;
			NrOfItems = static_cast<INT>(::SendMessage(ListBox, LB_GETCOUNT, 0, 0));

			Index = 0;
			while(Index < NrOfItems)
			{
				::SendMessage(ListBox, LB_GETITEMRECT, Index, reinterpret_cast<LPARAM>(&TempRect));
				if(Common.PointInsideRect(LocalMousePosition, TempRect))
				{
					::SendMessage(ListBox, LB_SETCURSEL, Index, 0);
					MenuId = VertexMenu;
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
				case DialogVerticesList:
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

				case VertexFileCreate:
				case VertexFileCreate2:
				{
					if(!CreateNew(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case VertexFileRemove:
				case VertexFileRemove2:
				{
					if(!Remove(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case VertexFileEdit:
				case VertexFileEdit2:
				{
					if(!Edit(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case DialogVerticesButtonOk:
				{
					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogVerticesButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogVerticesButtonOk, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Creates a new vertex
//+-----------------------------------------------------------------------------
BOOL WINDOW_VERTICES_DIALOG::CreateNew(HWND Window)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing vertex
//+-----------------------------------------------------------------------------
BOOL WINDOW_VERTICES_DIALOG::Remove(HWND Window)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing vertex
//+-----------------------------------------------------------------------------
BOOL WINDOW_VERTICES_DIALOG::Edit(HWND Window)
{
	INT Index;
	HWND ListBox;
	MODEL_GEOSET_VERTEX* Vertex;

	ListBox = GetDlgItem(Window, DialogVerticesList);

	Index = static_cast<INT>(::SendMessage(ListBox, LB_GETCURSEL, 0, 0));
	if(Index == LB_ERR) return TRUE;

	Vertex = reinterpret_cast<MODEL_GEOSET_VERTEX*>(::SendMessage(ListBox, LB_GETITEMDATA, Index, 0));
	if(Vertex == NULL) return TRUE;

	if(!VertexDialog.Display(Window, StaticGeoset, Vertex)) return TRUE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}

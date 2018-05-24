//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowFacesDialog.h"
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_FACES_DIALOG FacesDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
SIMPLE_CONTAINER<MODEL_GEOSET_FACE*>* WINDOW_FACES_DIALOG::StaticData;
MODEL_GEOSET* WINDOW_FACES_DIALOG::StaticGeoset;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_FACES_DIALOG::WINDOW_FACES_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_FACES_DIALOG::~WINDOW_FACES_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_FACES_DIALOG::Display(HWND ParentWindow, MODEL_GEOSET* Geoset, SIMPLE_CONTAINER<MODEL_GEOSET_FACE*>* Data) CONST
{
	StaticData = Data;
	StaticGeoset = Geoset;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogFaces), ParentWindow, DialogMessageHandler, 0))
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
BOOL CALLBACK WINDOW_FACES_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT i;
			INT Index;
			HWND ListBox;
			MODEL_GEOSET_FACE* Face;

			AddWindow(Window);
			CenterWindow(Window);

			ListBox = GetDlgItem(Window, DialogFacesList);

			for(i = 0; i < StaticData->GetTotalSize(); i++)
			{
				if(StaticData->ValidIndex(i))
				{
					std::stringstream Stream;

					Face = (*StaticData)[i];

					Stream << "Face " << Face;
					Index = static_cast<INT>(::SendMessage(ListBox, LB_INSERTSTRING, -1, reinterpret_cast<LPARAM>(Stream.str().c_str())));
					::SendMessage(ListBox, LB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Face));
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

			ListBox = GetDlgItem(Window, DialogFacesList);

			GetWindowRect(ListBox, &ListRect);
			GetCursorPos(&MousePosition);
			LocalMousePosition = MousePosition;
			ScreenToClient(ListBox, &LocalMousePosition);

			if(!Common.PointInsideRect(MousePosition, ListRect)) return TRUE;

			MenuId = FaceMenu2;
			NrOfItems = static_cast<INT>(::SendMessage(ListBox, LB_GETCOUNT, 0, 0));

			Index = 0;
			while(Index < NrOfItems)
			{
				::SendMessage(ListBox, LB_GETITEMRECT, Index, reinterpret_cast<LPARAM>(&TempRect));
				if(Common.PointInsideRect(LocalMousePosition, TempRect))
				{
					::SendMessage(ListBox, LB_SETCURSEL, Index, 0);
					MenuId = FaceMenu;
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
				case DialogFacesList:
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

				case FaceFileCreate:
				case FaceFileCreate2:
				{
					if(!CreateNew(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case FaceFileRemove:
				case FaceFileRemove2:
				{
					if(!Remove(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case FaceFileEdit:
				case FaceFileEdit2:
				{
					if(!Edit(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case DialogFacesButtonOk:
				{
					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogFacesButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogFacesButtonOk, 0);
			return TRUE;
		}
	}

	return FALSE;
}



//+-----------------------------------------------------------------------------
//| Creates a new face
//+-----------------------------------------------------------------------------
BOOL WINDOW_FACES_DIALOG::CreateNew(HWND Window)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing face
//+-----------------------------------------------------------------------------
BOOL WINDOW_FACES_DIALOG::Remove(HWND Window)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing face
//+-----------------------------------------------------------------------------
BOOL WINDOW_FACES_DIALOG::Edit(HWND Window)
{
	INT Index;
	HWND ListBox;
	MODEL_GEOSET_FACE* Face;

	ListBox = GetDlgItem(Window, DialogFacesList);

	Index = static_cast<INT>(::SendMessage(ListBox, LB_GETCURSEL, 0, 0));
	if(Index == LB_ERR) return TRUE;

	Face = reinterpret_cast<MODEL_GEOSET_FACE*>(::SendMessage(ListBox, LB_GETITEMDATA, Index, 0));
	if(Face == NULL) return TRUE;

	if(!FaceDialog.Display(Window, StaticGeoset, Face)) return TRUE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}

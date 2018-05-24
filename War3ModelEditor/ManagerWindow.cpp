//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MANAGER_WINDOW::MANAGER_WINDOW()
{
	ManagerMenuId1 = INVALID_INDEX;
	ManagerMenuId2 = INVALID_INDEX;

	Title = "";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MANAGER_WINDOW::~MANAGER_WINDOW()
{
	ClearAllNames();
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL MANAGER_WINDOW::Create()
{
	WINDOW_LISTBOX_INFO ListboxInfo;

	FrameInfo.Title = Title;
	FrameInfo.Width = DEFAULT_MANAGER_WINDOW_WIDTH;
	FrameInfo.Height = DEFAULT_MANAGER_WINDOW_HEIGHT;
	FrameInfo.Style &= (~WS_VISIBLE);
	FrameInfo.Style |= (WS_MAXIMIZEBOX | WS_SIZEBOX);

	if(!WINDOW_FRAME::Create()) return FALSE;

	ListboxInfo.Parent = Window;
	ListboxInfo.Width = DEFAULT_MANAGER_WINDOW_WIDTH;
	ListboxInfo.Height = DEFAULT_MANAGER_WINDOW_HEIGHT;
	ListboxInfo.AutoSort = TRUE;
	ListboxInfo.VerticalScroll = TRUE;

	if(!ListBox.Create(ListboxInfo)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID MANAGER_WINDOW::Destroy()
{
	ListBox.Destroy();

	WINDOW_FRAME::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_CONTEXTMENU:
		{
			INT Index;
			INT MenuId;
			INT NrOfItems;
			std::string FileName;
			RECT ListRect;
			RECT TempRect;
			POINT MousePosition;
			POINT LocalMousePosition;

			GetWindowRect(ListBox.GetWindow(), &ListRect);
			GetCursorPos(&MousePosition);
			LocalMousePosition = MousePosition;
			ScreenToClient(ListBox.GetWindow(), &LocalMousePosition);

			if(!Common.PointInsideRect(MousePosition, ListRect)) return 0;

			MenuId = ManagerMenuId2;
			NrOfItems = ListBox.GetNrOfItems();

			Index = 0;
			while(Index < NrOfItems)
			{
				ListBox.GetItemRect(Index, TempRect);

				if(Common.PointInsideRect(LocalMousePosition, TempRect))
				{
					FileName = ListBox.GetString(Index);
					ListBox.SetCurrentSelection(Index);

					MenuId = ManagerMenuId1;
					break;
				}

				Index++;
			}

			if(MenuId == INVALID_INDEX) return 0;

			TrackPopupMenuEx(GetSubMenu(LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(MenuId)), 0), TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePosition.x, MousePosition.y, Window, NULL);

			return 0;
		}

		case WM_SIZE:
		case WM_SIZING:
		{
			ResizeListBox();
			return 0;
		}

		case WM_CLOSE:
		{
			Hide();
			return 0;
		}

		case WM_DESTROY:
		{
			return 0;
		}
	}

	return DefWindowProc(Window, Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT MANAGER_WINDOW::MenuHandler(WORD MenuItem)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window control messages
//+-----------------------------------------------------------------------------
LRESULT MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Adds a name to the list
//+-----------------------------------------------------------------------------
VOID MANAGER_WINDOW::AddName(INT Id, CONST std::string& Name)
{
	INT Index;

	Index = ListBox.AddString((Name == "") ? "..." : Name);
	ListBox.SetData(Index, Id);
}


//+-----------------------------------------------------------------------------
//| Removes a name from the list
//+-----------------------------------------------------------------------------
VOID MANAGER_WINDOW::RemoveName(INT Id)
{
	INT i;
	INT Size;

	Size = ListBox.GetNrOfItems();

	for(i = 0; i < Size; i++)
	{
		if(ListBox.GetData(i) == Id)
		{
			ListBox.RemoveString(i);
			break;
		}
	}
}


//+-----------------------------------------------------------------------------
//| Removes all names from the list
//+-----------------------------------------------------------------------------
VOID MANAGER_WINDOW::ClearAllNames()
{
	ListBox.Clear();
}


//+-----------------------------------------------------------------------------
//| Checks if a name exists in the list
//+-----------------------------------------------------------------------------
BOOL MANAGER_WINDOW::HasName(CONST std::string& Name)
{
	return (ListBox.GetIndex(Name) != INVALID_INDEX);
}


//+-----------------------------------------------------------------------------
//| Resizes the listbox
//+-----------------------------------------------------------------------------
VOID MANAGER_WINDOW::ResizeListBox()
{
	ListBox.SetDimension(GetWidth(), GetHeight());
}

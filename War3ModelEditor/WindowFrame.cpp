//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowFrame.h"
#include "Error.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_FRAME::WINDOW_FRAME()
{
	ClassRegistered = FALSE;
	ClassName = "";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_FRAME::~WINDOW_FRAME()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window frame
//+-----------------------------------------------------------------------------
BOOL WINDOW_FRAME::Create()
{
	RECT WindowRect;
	INT WindowX;
	INT WindowY;
	INT WindowWidth;
	INT WindowHeight;
	WNDCLASSEX ClassInfo;
	std::stringstream Stream;

	Destroy();

	Stream << "MAGOS_WINDOW_" << this;
	ClassName = Stream.str();

	ClassInfo.cbSize = sizeof(WNDCLASSEX);
	ClassInfo.style = CS_HREDRAW | CS_VREDRAW;
	ClassInfo.lpfnWndProc = StaticMessageHandler;
	ClassInfo.cbClsExtra = 0;
	ClassInfo.cbWndExtra = sizeof(WINDOW*);
	ClassInfo.hInstance = GetModuleHandle(NULL);
	ClassInfo.hIcon = FrameInfo.Icon;
	ClassInfo.hCursor = FrameInfo.Cursor;
	ClassInfo.hbrBackground = FrameInfo.Background;
	ClassInfo.lpszMenuName = NULL;
	ClassInfo.lpszClassName = ClassName.c_str();
	ClassInfo.hIconSm = NULL;

	if(!RegisterClassEx(&ClassInfo))
	{
		Error.SetMessage("Unable to register the window class \"" + ClassName + "\"!");
		return FALSE;
	}

	ClassRegistered = TRUE;

	WindowRect = MakeWindowRect(FrameInfo.Width, FrameInfo.Height);
	WindowWidth = WindowRect.right;
	WindowHeight = WindowRect.bottom;

	if(FrameInfo.X == WINDOW_CENTER_POSITION)
	{
		WindowX = (GetSystemMetrics(SM_CXSCREEN) - WindowWidth) / 2;
	}
	else
	{
		WindowX = FrameInfo.X;
	}

	if(FrameInfo.Y == WINDOW_CENTER_POSITION)
	{
		WindowY = (GetSystemMetrics(SM_CYSCREEN) - WindowHeight) / 2;
	}
	else
	{
		WindowY = FrameInfo.Y;
	}

	Window = CreateWindowEx(0, ClassName.c_str(), FrameInfo.Title.c_str(),
							FrameInfo.Style, WindowX, WindowY, WindowWidth, WindowHeight,
							NULL, NULL, GetModuleHandle(NULL), NULL);
	if(Window == NULL)
	{
		Error.SetMessage("Unable to create the window \"" + ClassName + "\"!");
		return FALSE;
	}

	if(FrameInfo.Menu != NULL)
	{
		SetMenu(FrameInfo.Menu);
	}

	if(FrameInfo.Parent != NULL)
	{
		SetParent(FrameInfo.Parent);
	}

	SetWindowLong(Window, 0, reinterpret_cast<LONG>(this));

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window frame
//+-----------------------------------------------------------------------------
VOID WINDOW_FRAME::Destroy()
{
	SAFE_DESTROY(Window);

	if(ClassRegistered)
	{
		UnregisterClass(ClassName.c_str(), GetModuleHandle(NULL));
		ClassRegistered = FALSE;
		ClassName = "";
	}
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT WINDOW_FRAME::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
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
LRESULT WINDOW_FRAME::MenuHandler(WORD MenuItem)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window control messages
//+-----------------------------------------------------------------------------
LRESULT WINDOW_FRAME::ControlHandler(HWND Control, WORD Code)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT WINDOW_FRAME::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Sets a new width for the window frame
//+-----------------------------------------------------------------------------
VOID WINDOW_FRAME::SetWindowWidth(INT NewWidth)
{
	RECT TempRect;

	TempRect = MakeWindowRect(NewWidth, GetHeight());
	::SetWindowPos(Window, NULL, 0, 0, TempRect.right, TempRect.bottom, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
}


//+-----------------------------------------------------------------------------
//| Sets a new height for the window frame
//+-----------------------------------------------------------------------------
VOID WINDOW_FRAME::SetWindowHeight(INT NewHeight)
{
	RECT TempRect;

	TempRect = MakeWindowRect(GetWidth(), NewHeight);
	::SetWindowPos(Window, NULL, 0, 0, TempRect.right, TempRect.bottom, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
}


//+-----------------------------------------------------------------------------
//| Sets a new dimension for the window frame
//+-----------------------------------------------------------------------------
VOID WINDOW_FRAME::SetWindowDimension(INT NewWidth, INT NewHeight)
{
	RECT TempRect;

	TempRect = MakeWindowRect(NewWidth, NewHeight);
	::SetWindowPos(Window, NULL, 0, 0, TempRect.right, TempRect.bottom, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
}


//+-----------------------------------------------------------------------------
//| Sets a new menu for the window frame
//+-----------------------------------------------------------------------------
VOID WINDOW_FRAME::SetMenu(HMENU NewMenu)
{
	::SetMenu(Window, NewMenu);
}


//+-----------------------------------------------------------------------------
//| Returns the window frame menu
//+-----------------------------------------------------------------------------
HMENU WINDOW_FRAME::GetMenu() CONST
{
	return ::GetMenu(Window);
}


//+-----------------------------------------------------------------------------
//| Checks or unchecks a menu item
//+-----------------------------------------------------------------------------
VOID WINDOW_FRAME::CheckMenuItem(INT MenuItem, BOOL Check)
{
	MENUITEMINFO MenuItemInfo;

	ZeroMemory(&MenuItemInfo, sizeof(MENUITEMINFO));
	MenuItemInfo.cbSize = sizeof(MENUITEMINFO);
	MenuItemInfo.fMask = MIIM_STATE;
	MenuItemInfo.fState = Check ? MFS_CHECKED : MFS_UNCHECKED;

	::SetMenuItemInfo(::GetMenu(Window), MenuItem, FALSE, &MenuItemInfo);
}


//+-----------------------------------------------------------------------------
//| Enables or disables a menu item
//+-----------------------------------------------------------------------------
VOID WINDOW_FRAME::EnableMenuItem(INT MenuItem, BOOL Enable)
{
	::EnableMenuItem(::GetMenu(Window), MenuItem, Enable ? MF_ENABLED : MF_DISABLED);
}


//+-----------------------------------------------------------------------------
//| Grays or ungrays a menu item
//+-----------------------------------------------------------------------------
VOID WINDOW_FRAME::GrayMenuItem(INT MenuItem, BOOL Gray)
{
	::EnableMenuItem(::GetMenu(Window), MenuItem, Gray ? MF_GRAYED : MF_ENABLED);
}


//+-----------------------------------------------------------------------------
//| Creates a window frame rectangle given the specified width and height
//+-----------------------------------------------------------------------------
RECT WINDOW_FRAME::MakeWindowRect(INT Width, INT Height) CONST
{
	RECT TempRect;

	TempRect.left = 0;
	TempRect.top = 0;
	TempRect.right = Width;
	TempRect.bottom = Height;

	::AdjustWindowRectEx(&TempRect, FrameInfo.Style, (FrameInfo.Menu != NULL), 0);

	TempRect.right -= TempRect.left;
	TempRect.bottom -= TempRect.top;
	TempRect.left = 0;
	TempRect.top = 0;

	return TempRect;
}


//+-----------------------------------------------------------------------------
//| The static message handler
//+-----------------------------------------------------------------------------
LRESULT CALLBACK WINDOW_FRAME::StaticMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	HWND Control;
	NMHDR* Header;
	WINDOW_FRAME* WindowObject;

	WindowObject = reinterpret_cast<WINDOW_FRAME*>(GetWindowLong(Window, 0));
	if(WindowObject == NULL) return DefWindowProc(Window, Message, W, L);

	switch(Message)
	{
		case WM_COMMAND:
		{
			Control = reinterpret_cast<HWND>(L);
			if(Control == NULL)
			{
				return WindowObject->MenuHandler(LOWORD(W));
			}

			return WindowObject->ControlHandler(Control, HIWORD(W));
		}

		case WM_NOTIFY:
		{
			Header = reinterpret_cast<NMHDR*>(L);

			return WindowObject->NotifyHandler(Header->hwndFrom, Header->code, Header);
		}
	}

	return WindowObject->MessageHandler(Message, W, L);
}

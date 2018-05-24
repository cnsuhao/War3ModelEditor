//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Window.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW::WINDOW()
{
	Window = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW::~WINDOW()
{
	Destroy();
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID WINDOW::Destroy()
{
	SAFE_DESTROY(Window);
}


//+-----------------------------------------------------------------------------
//| Returns the window pointer
//+-----------------------------------------------------------------------------
HWND WINDOW::GetWindow() CONST
{
	return Window;
}


//+-----------------------------------------------------------------------------
//| Sends a message to the window
//+-----------------------------------------------------------------------------
LRESULT WINDOW::SendMessage(UINT Message, WPARAM W, LPARAM L)
{
	return ::SendMessage(Window, Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Sets a new parent for the window
//+-----------------------------------------------------------------------------
VOID WINDOW::SetParent(HWND NewParent)
{
	::SetParent(Window, NewParent);
}


//+-----------------------------------------------------------------------------
//| Sets a new text for the window
//+-----------------------------------------------------------------------------
VOID WINDOW::SetText(CONST std::string& NewText)
{
	::SetWindowText(Window, NewText.c_str());
}


//+-----------------------------------------------------------------------------
//| Returns the window parent
//+-----------------------------------------------------------------------------
HWND WINDOW::GetParent() CONST
{
	return ::GetParent(Window);
}


//+-----------------------------------------------------------------------------
//| Returns the window text
//+-----------------------------------------------------------------------------
std::string WINDOW::GetText() CONST
{
	INT Size;
	std::vector<CHAR> Buffer;

	Size = ::GetWindowTextLength(Window) + 1;
	Buffer.resize(Size + 1);

	::GetWindowText(Window, &Buffer[0], Size);
	Buffer[Size] = '\0';

	return &Buffer[0];
}


//+-----------------------------------------------------------------------------
//| Centers the position on the screen
//+-----------------------------------------------------------------------------
VOID WINDOW::CenterPosition()
{
	INT X;
	INT Y;

	X = (GetSystemMetrics(SM_CXSCREEN) - GetWidth()) / 2;
	Y = (GetSystemMetrics(SM_CYSCREEN) - GetHeight()) / 2;

	if(X < 0) X = 0;
	if(Y < 0) Y = 0;

	SetPosition(X, Y);
}


//+-----------------------------------------------------------------------------
//| Sets a new X-position for the window
//+-----------------------------------------------------------------------------
VOID WINDOW::SetX(INT NewX)
{
	::SetWindowPos(Window, NULL, NewX, GetY(), 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}


//+-----------------------------------------------------------------------------
//| Sets a new Y-position for the iwndow
//+-----------------------------------------------------------------------------
VOID WINDOW::SetY(INT NewY)
{
	::SetWindowPos(Window, NULL, GetX(), NewY, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}


//+-----------------------------------------------------------------------------
//| Sets a new position for the window
//+-----------------------------------------------------------------------------
VOID WINDOW::SetPosition(INT NewX, INT NewY)
{
	::SetWindowPos(Window, NULL, NewX, NewY, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}


//+-----------------------------------------------------------------------------
//| Sets a new width for the window
//+-----------------------------------------------------------------------------
VOID WINDOW::SetWidth(INT NewWidth)
{
	::SetWindowPos(Window, NULL, 0, 0, NewWidth, GetHeight(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
}


//+-----------------------------------------------------------------------------
//| Sets a new height for the window
//+-----------------------------------------------------------------------------
VOID WINDOW::SetHeight(INT NewHeight)
{
	::SetWindowPos(Window, NULL, 0, 0, GetWidth(), NewHeight, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
}


//+-----------------------------------------------------------------------------
//| Sets a new dimension for the window
//+-----------------------------------------------------------------------------
VOID WINDOW::SetDimension(INT NewWidth, INT NewHeight)
{
	::SetWindowPos(Window, NULL, 0, 0, NewWidth, NewHeight, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
}


//+-----------------------------------------------------------------------------
//| Returns the window X-position
//+-----------------------------------------------------------------------------
INT WINDOW::GetX() CONST
{
	RECT TempRect;

	::GetWindowRect(Window, &TempRect);

	return TempRect.left;
}


//+-----------------------------------------------------------------------------
//| Returns the window Y-position
//+-----------------------------------------------------------------------------
INT WINDOW::GetY() CONST
{
	RECT TempRect;

	::GetWindowRect(Window, &TempRect);

	return TempRect.top;
}


//+-----------------------------------------------------------------------------
//| Returns the window width
//+-----------------------------------------------------------------------------
INT WINDOW::GetWidth() CONST
{
	RECT TempRect;

	::GetClientRect(Window, &TempRect);

	return TempRect.right;
}


//+-----------------------------------------------------------------------------
//| Returns the window height
//+-----------------------------------------------------------------------------
INT WINDOW::GetHeight() CONST
{
	RECT TempRect;

	::GetClientRect(Window, &TempRect);

	return TempRect.bottom;
}


//+-----------------------------------------------------------------------------
//| Shows the window
//+-----------------------------------------------------------------------------
VOID WINDOW::Show()
{
	::ShowWindow(Window, SW_SHOW);
}


//+-----------------------------------------------------------------------------
//| Hides the window
//+-----------------------------------------------------------------------------
VOID WINDOW::Hide()
{
	::ShowWindow(Window, SW_HIDE);
}


//+-----------------------------------------------------------------------------
//| Activates the window
//+-----------------------------------------------------------------------------
VOID WINDOW::Activate()
{
	::SetActiveWindow(Window);
}


//+-----------------------------------------------------------------------------
//| Minimizes the window
//+-----------------------------------------------------------------------------
VOID WINDOW::Minimize()
{
	::ShowWindow(Window, SW_MINIMIZE);
}


//+-----------------------------------------------------------------------------
//| Maximizes the window
//+-----------------------------------------------------------------------------
VOID WINDOW::Maximize()
{
	::ShowWindow(Window, SW_MAXIMIZE);
}


//+-----------------------------------------------------------------------------
//| Restores the window
//+-----------------------------------------------------------------------------
VOID WINDOW::Restore()
{
	::ShowWindow(Window, SW_RESTORE);
}


//+-----------------------------------------------------------------------------
//| Enables the window
//+-----------------------------------------------------------------------------
VOID WINDOW::Enable()
{
	::EnableWindow(Window, TRUE);
}


//+-----------------------------------------------------------------------------
//| Disables the window
//+-----------------------------------------------------------------------------
VOID WINDOW::Disable()
{
	::EnableWindow(Window, FALSE);
}

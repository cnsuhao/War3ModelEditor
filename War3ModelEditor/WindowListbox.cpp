//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowListbox.h"
#include "Error.h"
#include "Constants.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_LISTBOX::WINDOW_LISTBOX()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_LISTBOX::~WINDOW_LISTBOX()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a listbox window
//+-----------------------------------------------------------------------------
BOOL WINDOW_LISTBOX::Create(CONST WINDOW_LISTBOX_INFO& NewListboxInfo)
{
	Destroy();

	ListboxInfo = NewListboxInfo;

	if(ListboxInfo.HorizontalScroll) ListboxInfo.Style |= WS_HSCROLL;
	if(ListboxInfo.VerticalScroll) ListboxInfo.Style |= WS_VSCROLL;
	if(ListboxInfo.AutoSort) ListboxInfo.Style |= LBS_SORT;

	Window = CreateWindowEx(0, "LISTBOX", "", ListboxInfo.Style,
							ListboxInfo.X, ListboxInfo.Y,
							ListboxInfo.Width, ListboxInfo.Height,
							ListboxInfo.Parent, NULL, GetModuleHandle(NULL), NULL);
	if(Window == NULL)
	{
		Error.SetMessage("Unable to create a listbox window!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Clears the list
//+-----------------------------------------------------------------------------
VOID WINDOW_LISTBOX::Clear()
{
	::SendMessage(Window, LB_RESETCONTENT, 0, 0);
}


//+-----------------------------------------------------------------------------
//| Adds a string to the list
//+-----------------------------------------------------------------------------
INT WINDOW_LISTBOX::AddString(CONST std::string& String)
{
	return static_cast<INT>(::SendMessage(Window, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(String.c_str())));
}


//+-----------------------------------------------------------------------------
//| Removes a string from the list
//+-----------------------------------------------------------------------------
VOID WINDOW_LISTBOX::RemoveString(INT Index)
{
	::SendMessage(Window, LB_DELETESTRING, Index, 0);
}


//+-----------------------------------------------------------------------------
//| Returns the nr of items in the list
//+-----------------------------------------------------------------------------
INT WINDOW_LISTBOX::GetNrOfItems()
{
	return static_cast<INT>(::SendMessage(Window, LB_GETCOUNT, 0, 0));
}


//+-----------------------------------------------------------------------------
//| Returns the index of a string in the list
//+-----------------------------------------------------------------------------
INT WINDOW_LISTBOX::GetIndex(CONST std::string& String)
{
	INT Index;

	Index = static_cast<INT>(::SendMessage(Window, LB_FINDSTRINGEXACT, -1, reinterpret_cast<LPARAM>(String.c_str())));
	if(Index == LB_ERR) return INVALID_INDEX;

	return Index;
}


//+-----------------------------------------------------------------------------
//| Returns a string from the list
//+-----------------------------------------------------------------------------
std::string WINDOW_LISTBOX::GetString(INT Index)
{
	INT Size;
	std::vector<CHAR> Buffer;

	Size = static_cast<INT>(::SendMessage(Window, LB_GETTEXTLEN, Index, 0));
	if(Size ==  LB_ERR) return "";

	Buffer.resize(Size + 1);

	::SendMessage(Window, LB_GETTEXT, Index, reinterpret_cast<LPARAM>(&Buffer[0]));
	Buffer[Size] = '\0';
	
	return &Buffer[0];
}


//+-----------------------------------------------------------------------------
//| Returns the item data
//+-----------------------------------------------------------------------------
INT WINDOW_LISTBOX::GetData(INT Index)
{
	return static_cast<INT>(::SendMessage(Window, LB_GETITEMDATA, Index, 0));
}


//+-----------------------------------------------------------------------------
//| Sets a new item data
//+-----------------------------------------------------------------------------
VOID WINDOW_LISTBOX::SetData(INT Index, INT NewData)
{
	::SendMessage(Window, LB_SETITEMDATA, Index, NewData);
}


//+-----------------------------------------------------------------------------
//| Returns the current selection
//+-----------------------------------------------------------------------------
INT WINDOW_LISTBOX::GetCurrentSelection()
{
	INT Index;

	Index = static_cast<INT>(::SendMessage(Window, LB_GETCURSEL, 0, 0));
	if(Index == LB_ERR) return INVALID_INDEX;

	return Index;
}


//+-----------------------------------------------------------------------------
//| Sets a new current selection
//+-----------------------------------------------------------------------------
VOID WINDOW_LISTBOX::SetCurrentSelection(INT Index)
{
	::SendMessage(Window, LB_SETCURSEL, Index, 0);
}


//+-----------------------------------------------------------------------------
//| Returns the rect of an item in the list
//+-----------------------------------------------------------------------------
VOID WINDOW_LISTBOX::GetItemRect(INT Index, RECT& Rect)
{
	::SendMessage(Window, LB_GETITEMRECT, Index, reinterpret_cast<LPARAM>(&Rect));
}

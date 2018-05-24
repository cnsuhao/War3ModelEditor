//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowCombobox.h"
#include "Error.h"
#include "Constants.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_COMBOBOX::WINDOW_COMBOBOX()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_COMBOBOX::~WINDOW_COMBOBOX()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a combobox window
//+-----------------------------------------------------------------------------
BOOL WINDOW_COMBOBOX::Create(CONST WINDOW_COMBOBOX_INFO& NewComboboxInfo)
{
	Destroy();

	ComboboxInfo = NewComboboxInfo;

	if(ComboboxInfo.AutoSort) ComboboxInfo.Style |= CBS_SORT;

	switch(ComboboxInfo.ComboboxType)
	{
		case COMBOBOX_TYPE_SIMPLE:
		{
			ComboboxInfo.Style |= CBS_SIMPLE;
			break;
		}

		case COMBOBOX_TYPE_DROPDOWN:
		{
			ComboboxInfo.Style |= CBS_DROPDOWN;
			break;
		}

		case COMBOBOX_TYPE_DROPDOWN_LIST:
		{
			ComboboxInfo.Style |= CBS_DROPDOWNLIST;
			break;
		}
	}

	Window = CreateWindowEx(0, "COMBOBOX", "", ComboboxInfo.Style,
							ComboboxInfo.X, ComboboxInfo.Y,
							ComboboxInfo.Width, ComboboxInfo.Height,
							ComboboxInfo.Parent, NULL, GetModuleHandle(NULL), NULL);
	if(Window == NULL)
	{
		Error.SetMessage("Unable to create a combobox window!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Clears the list
//+-----------------------------------------------------------------------------
VOID WINDOW_COMBOBOX::Clear()
{
	::SendMessage(Window, CB_RESETCONTENT, 0, 0);
}


//+-----------------------------------------------------------------------------
//| Adds a string to the list
//+-----------------------------------------------------------------------------
INT WINDOW_COMBOBOX::AddString(CONST std::string& String)
{
	return static_cast<INT>(::SendMessage(Window, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(String.c_str())));
}


//+-----------------------------------------------------------------------------
//| Removes a string from the list
//+-----------------------------------------------------------------------------
VOID WINDOW_COMBOBOX::RemoveString(INT Index)
{
	::SendMessage(Window, CB_DELETESTRING, Index, 0);
}


//+-----------------------------------------------------------------------------
//| Returns the nr of items in the list
//+-----------------------------------------------------------------------------
INT WINDOW_COMBOBOX::GetNrOfItems()
{
	return static_cast<INT>(::SendMessage(Window, CB_GETCOUNT, 0, 0));
}


//+-----------------------------------------------------------------------------
//| Returns the index of a string in the list
//+-----------------------------------------------------------------------------
INT WINDOW_COMBOBOX::GetIndex(CONST std::string& String)
{
	INT Index;

	Index = static_cast<INT>(::SendMessage(Window, CB_FINDSTRINGEXACT, -1, reinterpret_cast<LPARAM>(String.c_str())));
	if(Index == LB_ERR) return INVALID_INDEX;

	return Index;
}


//+-----------------------------------------------------------------------------
//| Returns a string from the list
//+-----------------------------------------------------------------------------
std::string WINDOW_COMBOBOX::GetString(INT Index)
{
	INT Size;
	std::vector<CHAR> Buffer;

	Size = static_cast<INT>(::SendMessage(Window, CB_GETLBTEXTLEN, Index, 0));
	if(Size ==  LB_ERR) return "";

	Buffer.resize(Size + 1);

	::SendMessage(Window, CB_GETLBTEXT, Index, reinterpret_cast<LPARAM>(&Buffer[0]));
	Buffer[Size] = '\0';
	
	return &Buffer[0];
}


//+-----------------------------------------------------------------------------
//| Returns the item data
//+-----------------------------------------------------------------------------
INT WINDOW_COMBOBOX::GetData(INT Index)
{
	return static_cast<INT>(::SendMessage(Window, CB_GETITEMDATA, Index, 0));
}


//+-----------------------------------------------------------------------------
//| Sets a new item data
//+-----------------------------------------------------------------------------
VOID WINDOW_COMBOBOX::SetData(INT Index, INT NewData)
{
	::SendMessage(Window, CB_SETITEMDATA, Index, NewData);
}


//+-----------------------------------------------------------------------------
//| Returns the current selection
//+-----------------------------------------------------------------------------
INT WINDOW_COMBOBOX::GetCurrentSelection()
{
	INT Index;

	Index = static_cast<INT>(::SendMessage(Window, CB_GETCURSEL, 0, 0));
	if(Index == LB_ERR) return INVALID_INDEX;

	return Index;
}


//+-----------------------------------------------------------------------------
//| Sets a new current selection
//+-----------------------------------------------------------------------------
VOID WINDOW_COMBOBOX::SetCurrentSelection(INT Index)
{
	::SendMessage(Window, CB_SETCURSEL, Index, 0);
}

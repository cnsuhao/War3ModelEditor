//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_COMBOBOX_H
#define MAGOS_WINDOW_COMBOBOX_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Window.h"


//+-----------------------------------------------------------------------------
//| Enumeration of the combobox types
//+-----------------------------------------------------------------------------
enum COMBOBOX_TYPE
{
	COMBOBOX_TYPE_SIMPLE,
	COMBOBOX_TYPE_DROPDOWN,
	COMBOBOX_TYPE_DROPDOWN_LIST,
};


//+-----------------------------------------------------------------------------
//| Window combobox info structure
//+-----------------------------------------------------------------------------
struct WINDOW_COMBOBOX_INFO
{
	WINDOW_COMBOBOX_INFO()
	{
		X = 0;
		Y = 0;
		Width = 100;
		Height = 100;

		AutoSort = FALSE;

		ComboboxType = COMBOBOX_TYPE_DROPDOWN_LIST;

		Parent = NULL;
		Style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DISABLENOSCROLL | CBS_HASSTRINGS | CBS_NOINTEGRALHEIGHT;
	}

	INT X;
	INT Y;
	INT Width;
	INT Height;

	BOOL AutoSort;

	COMBOBOX_TYPE ComboboxType;

	HWND Parent;
	DWORD Style;
};


//+-----------------------------------------------------------------------------
//| Window combobox class
//+-----------------------------------------------------------------------------
class WINDOW_COMBOBOX : public WINDOW
{
	public:
		CONSTRUCTOR WINDOW_COMBOBOX();
		DESTRUCTOR ~WINDOW_COMBOBOX();

		virtual BOOL Create(CONST WINDOW_COMBOBOX_INFO& NewComboboxInfo);

		VOID Clear();

		INT AddString(CONST std::string& String);
		VOID RemoveString(INT Index);

		INT GetNrOfItems();
		INT GetIndex(CONST std::string& String);
		std::string GetString(INT Index);

		INT GetData(INT Index);
		VOID SetData(INT Index, INT NewData);

		INT GetCurrentSelection();
		VOID SetCurrentSelection(INT Index);

	protected:
		WINDOW_COMBOBOX_INFO ComboboxInfo;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

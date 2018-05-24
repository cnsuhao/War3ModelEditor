//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_LISTBOX_H
#define MAGOS_WINDOW_LISTBOX_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Window.h"


//+-----------------------------------------------------------------------------
//| Window listbox info structure
//+-----------------------------------------------------------------------------
struct WINDOW_LISTBOX_INFO
{
	WINDOW_LISTBOX_INFO()
	{
		X = 0;
		Y = 0;
		Width = 100;
		Height = 100;

		HorizontalScroll = FALSE;
		VerticalScroll = FALSE;
		AutoSort = FALSE;

		Parent = NULL;
		Style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | LBS_DISABLENOSCROLL | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY;
	}

	INT X;
	INT Y;
	INT Width;
	INT Height;

	BOOL HorizontalScroll;
	BOOL VerticalScroll;
	BOOL AutoSort;

	HWND Parent;
	DWORD Style;
};


//+-----------------------------------------------------------------------------
//| Window listbox class
//+-----------------------------------------------------------------------------
class WINDOW_LISTBOX : public WINDOW
{
	public:
		CONSTRUCTOR WINDOW_LISTBOX();
		DESTRUCTOR ~WINDOW_LISTBOX();

		virtual BOOL Create(CONST WINDOW_LISTBOX_INFO& NewListboxInfo);

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

		VOID GetItemRect(INT Index, RECT& Rect);

	protected:
		WINDOW_LISTBOX_INFO ListboxInfo;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

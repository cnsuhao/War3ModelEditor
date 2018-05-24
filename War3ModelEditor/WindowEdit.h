//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_EDIT_H
#define MAGOS_WINDOW_EDIT_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Window.h"


//+-----------------------------------------------------------------------------
//| Window edit info structure
//+-----------------------------------------------------------------------------
struct WINDOW_EDIT_INFO
{
	WINDOW_EDIT_INFO()
	{
		Text = "";

		X = 0;
		Y = 0;
		Width = 100;
		Height = 100;

		HorizontalScroll = FALSE;
		VerticalScroll = FALSE;
		Multiline = FALSE;
		ReadOnly = FALSE;

		Parent = NULL;
		Style = 0;
	}

	std::string Text;

	INT X;
	INT Y;
	INT Width;
	INT Height;

	BOOL HorizontalScroll;
	BOOL VerticalScroll;
	BOOL Multiline;
	BOOL ReadOnly;

	HWND Parent;
	DWORD Style;
};


//+-----------------------------------------------------------------------------
//| Window edit class
//+-----------------------------------------------------------------------------
class WINDOW_EDIT : public WINDOW
{
	public:
		CONSTRUCTOR WINDOW_EDIT();
		DESTRUCTOR ~WINDOW_EDIT();

		virtual BOOL Create(CONST WINDOW_EDIT_INFO& NewEditInfo);

	protected:
		WINDOW_EDIT_INFO EditInfo;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

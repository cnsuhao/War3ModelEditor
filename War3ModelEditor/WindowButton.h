//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_BUTTON_H
#define MAGOS_WINDOW_BUTTON_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Window.h"


//+-----------------------------------------------------------------------------
//| Enumeration of the button types
//+-----------------------------------------------------------------------------
enum BUTTON_TYPE
{
	BUTTON_TYPE_PUSHBUTTON,
	BUTTON_TYPE_CHECKBOX,
	BUTTON_TYPE_CHECKBOX_3STATE,
	BUTTON_TYPE_RADIOBUTTON,
};


//+-----------------------------------------------------------------------------
//| Window button info structure
//+-----------------------------------------------------------------------------
struct WINDOW_BUTTON_INFO
{
	WINDOW_BUTTON_INFO()
	{
		Text = "";

		X = 0;
		Y = 0;
		Width = 100;
		Height = 28;

		ButtonType = BUTTON_TYPE_PUSHBUTTON;

		Parent = NULL;
		Style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE;
	}

	std::string Text;

	INT X;
	INT Y;
	INT Width;
	INT Height;

	BUTTON_TYPE ButtonType;

	HWND Parent;
	DWORD Style;
};


//+-----------------------------------------------------------------------------
//| Window button class
//+-----------------------------------------------------------------------------
class WINDOW_BUTTON : public WINDOW
{
	public:
		CONSTRUCTOR WINDOW_BUTTON();
		DESTRUCTOR ~WINDOW_BUTTON();

		virtual BOOL Create(CONST WINDOW_BUTTON_INFO& NewButtonInfo);

	protected:
		WINDOW_BUTTON_INFO ButtonInfo;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

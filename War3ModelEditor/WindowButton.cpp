//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowButton.h"

//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_BUTTON::WINDOW_BUTTON()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_BUTTON::~WINDOW_BUTTON()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a button window
//+-----------------------------------------------------------------------------
BOOL WINDOW_BUTTON::Create(CONST WINDOW_BUTTON_INFO& NewButtonInfo)
{
	Destroy();

	ButtonInfo = NewButtonInfo;

	switch(ButtonInfo.ButtonType)
	{
		case BUTTON_TYPE_PUSHBUTTON:
		{
			ButtonInfo.Style |= BS_PUSHBUTTON;
			break;
		}

		case BUTTON_TYPE_CHECKBOX:
		{
			ButtonInfo.Style |= BS_AUTOCHECKBOX;
			break;
		}

		case BUTTON_TYPE_CHECKBOX_3STATE:
		{
			ButtonInfo.Style |= BS_AUTO3STATE;
			break;
		}

		case BUTTON_TYPE_RADIOBUTTON:
		{
			ButtonInfo.Style |= BS_AUTORADIOBUTTON;
			break;
		}
	}

	Window = CreateWindowEx(0, "BUTTON", ButtonInfo.Text.c_str(), ButtonInfo.Style,
							ButtonInfo.X, ButtonInfo.Y,
							ButtonInfo.Width, ButtonInfo.Height,
							ButtonInfo.Parent, NULL, GetModuleHandle(NULL), NULL);
	if(Window == NULL)
	{
		Error.SetMessage("Unable to create a button window!");
		return FALSE;
	}

	return TRUE;
}

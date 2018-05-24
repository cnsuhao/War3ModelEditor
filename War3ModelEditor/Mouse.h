//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MOUSE_H
#define MAGOS_MOUSE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST INT NR_OF_MOUSE_BUTTONS = 8;


//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST INT BUTTON_1      = 0x00;
CONST INT BUTTON_2      = 0x01;
CONST INT BUTTON_3      = 0x02;
CONST INT BUTTON_4      = 0x03;
CONST INT BUTTON_5      = 0x04;
CONST INT BUTTON_6      = 0x05;
CONST INT BUTTON_7      = 0x06;
CONST INT BUTTON_8      = 0x07;
CONST INT BUTTON_LEFT   = 0x00;
CONST INT BUTTON_RIGHT  = 0x01;
CONST INT BUTTON_MIDDLE = 0x02;


//+-----------------------------------------------------------------------------
//| Mouse button structure
//+-----------------------------------------------------------------------------
struct MOUSE_BUTTON_STATE
{
	MOUSE_BUTTON_STATE()
	{
		ButtonDown = FALSE;
		ButtonPressed = FALSE;
		ButtonReleased = FALSE;
	}

	BOOL ButtonDown;
	BOOL ButtonPressed;
	BOOL ButtonReleased;
};


//+-----------------------------------------------------------------------------
//| Mouse class
//+-----------------------------------------------------------------------------
class MOUSE
{
	public:
		CONSTRUCTOR MOUSE();
		DESTRUCTOR ~MOUSE();

		BOOL Setup(HWND Window, BOOL Exclusive = FALSE);
		VOID Shutdown();

		VOID Update();

		BOOL ButtonDown(INT Button) CONST;
		BOOL ButtonPressed(INT Button) CONST;
		BOOL ButtonReleased(INT Button) CONST;

		BOOL AnyButtonDown() CONST;
		BOOL AnyButtonPressed() CONST;
		BOOL AnyButtonReleased() CONST;

		INT GetDX() CONST;
		INT GetDY() CONST;
		INT GetDZ() CONST;

	protected:
		BOOL ValidButton(INT Button) CONST;

		LPDIRECTINPUT8 DirectInput;
		LPDIRECTINPUTDEVICE8 MouseDevice;

		MOUSE_BUTTON_STATE AnyButtonState;
		MOUSE_BUTTON_STATE ButtonState[NR_OF_MOUSE_BUTTONS];

		INT DX;
		INT DY;
		INT DZ;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MOUSE Mouse;
extern MOUSE ModelMouse;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

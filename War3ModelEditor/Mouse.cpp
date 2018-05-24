//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Mouse.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MOUSE Mouse;
MOUSE ModelMouse;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MOUSE::MOUSE()
{
	DirectInput = NULL;
	MouseDevice = NULL;

	DX = 0;
	DY = 0;
	DZ = 0;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MOUSE::~MOUSE()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Sets up the mouse
//+-----------------------------------------------------------------------------
BOOL MOUSE::Setup(HWND Window, BOOL Exclusive)
{
	DWORD Flags;

	Shutdown();

	if(FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<VOID**>(&DirectInput), NULL)))
	{
		Error.SetMessage("Unable to create a DirectInput object!");
		return FALSE;
	}

	if(FAILED(DirectInput->CreateDevice(GUID_SysMouse, &MouseDevice, NULL)))
	{
		Error.SetMessage("Unable to create a DirectInput mouse device!");
		return FALSE;
	}

	if(FAILED(MouseDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		Error.SetMessage("Unable to set the DirectInput mouse data format!");
		return FALSE;
	}

	Flags = DISCL_FOREGROUND | (Exclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE);

	if(FAILED(MouseDevice->SetCooperativeLevel(Window, Flags)))
	{
		Error.SetMessage("Unable to set the DirectInput mouse cooperative level!");
		return FALSE;
	}

	MouseDevice->Acquire();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Shuts down the mouse
//+-----------------------------------------------------------------------------
VOID MOUSE::Shutdown()
{
	if(MouseDevice != NULL) 
	{
		MouseDevice->Unacquire();
		MouseDevice->Release();
		MouseDevice = NULL;
	}

	if(DirectInput != NULL)
	{
		DirectInput->Release();
		DirectInput = NULL;
	}

	DX = 0;
	DY = 0;
	DZ = 0;
}


//+-----------------------------------------------------------------------------
//| Updates the mouse state
//+-----------------------------------------------------------------------------
VOID MOUSE::Update()
{
	INT i;
	DIMOUSESTATE2 MouseState;

	if(MouseDevice == NULL) return;

	ZeroMemory(&MouseState, sizeof(DIMOUSESTATE2));
    if(FAILED(MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		MouseDevice->Acquire();

		ZeroMemory(&MouseState, sizeof(DIMOUSESTATE2));
		if(FAILED(MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
		{
			return;
		}
    }

	DX = MouseState.lX;
	DY = MouseState.lY;
	DZ = MouseState.lZ;

	AnyButtonState = MOUSE_BUTTON_STATE();

	for(i = 0; i < NR_OF_MOUSE_BUTTONS; i++)
	{
		ButtonState[i].ButtonPressed = FALSE;
		ButtonState[i].ButtonReleased = FALSE;

		if(MouseState.rgbButtons[i])
		{
			if(!ButtonState[i].ButtonDown)
			{
				ButtonState[i].ButtonPressed = TRUE;
				AnyButtonState.ButtonPressed = TRUE;
			}

			ButtonState[i].ButtonDown = TRUE;
			AnyButtonState.ButtonDown = TRUE;
		}
		else
		{
			if(ButtonState[i].ButtonDown)
			{
				ButtonState[i].ButtonReleased = TRUE;
				AnyButtonState.ButtonReleased = TRUE;
			}

			ButtonState[i].ButtonDown = FALSE;
		}
	}
}


//+-----------------------------------------------------------------------------
//| Checks if a mouse button is down
//+-----------------------------------------------------------------------------
BOOL MOUSE::ButtonDown(INT Button) CONST
{
	if(!ValidButton(Button)) return FALSE;

	return ButtonState[Button].ButtonDown;
}


//+-----------------------------------------------------------------------------
//| Checks if a mouse button has just been pressed
//+-----------------------------------------------------------------------------
BOOL MOUSE::ButtonPressed(INT Button) CONST
{
	if(!ValidButton(Button)) return FALSE;

	return ButtonState[Button].ButtonPressed;
}


//+-----------------------------------------------------------------------------
//| Checks if a mouse button has just been released
//+-----------------------------------------------------------------------------
BOOL MOUSE::ButtonReleased(INT Button) CONST
{
	if(!ValidButton(Button)) return FALSE;

	return ButtonState[Button].ButtonReleased;
}


//+-----------------------------------------------------------------------------
//| Checks if any mouse button is down
//+-----------------------------------------------------------------------------
BOOL MOUSE::AnyButtonDown() CONST
{
	return AnyButtonState.ButtonDown;
}


//+-----------------------------------------------------------------------------
//| Checks if any mouse button has just been pressed
//+-----------------------------------------------------------------------------
BOOL MOUSE::AnyButtonPressed() CONST
{
	return AnyButtonState.ButtonPressed;
}


//+-----------------------------------------------------------------------------
//| Checks if any mouse button has just been released
//+-----------------------------------------------------------------------------
BOOL MOUSE::AnyButtonReleased() CONST
{
	return AnyButtonState.ButtonReleased;
}


//+-----------------------------------------------------------------------------
//| Returns the mouse DX
//+-----------------------------------------------------------------------------
INT MOUSE::GetDX() CONST
{
	return DX;
}


//+-----------------------------------------------------------------------------
//| Returns the mouse DY
//+-----------------------------------------------------------------------------
INT MOUSE::GetDY() CONST
{
	return DY;
}


//+-----------------------------------------------------------------------------
//| Returns the mouse DZ
//+-----------------------------------------------------------------------------
INT MOUSE::GetDZ() CONST
{
	return DZ;
}


//+-----------------------------------------------------------------------------
//| Checks if a mouse button is valid
//+-----------------------------------------------------------------------------
BOOL MOUSE::ValidButton(INT Button) CONST
{
	return (Button >= 0) && (Button < NR_OF_MOUSE_BUTTONS);
}

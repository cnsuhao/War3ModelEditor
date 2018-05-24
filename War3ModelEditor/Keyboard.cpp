//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Keyboard.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
KEYBOARD Keyboard;
KEYBOARD ModelKeyboard;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
KEYBOARD::KEYBOARD()
{
	DirectInput = NULL;
	KeyboardDevice = NULL;

	Delay = 0;

	AddKeyInfo(KEY_0, '0');
	AddKeyInfo(KEY_1, '1');
	AddKeyInfo(KEY_2, '2');
	AddKeyInfo(KEY_3, '3');
	AddKeyInfo(KEY_4, '4');
	AddKeyInfo(KEY_5, '5');
	AddKeyInfo(KEY_6, '6');
	AddKeyInfo(KEY_7, '7');
	AddKeyInfo(KEY_8, '8');
	AddKeyInfo(KEY_9, '9');

	AddKeyInfo(KEY_NUMPAD0, '0');
	AddKeyInfo(KEY_NUMPAD1, '1');
	AddKeyInfo(KEY_NUMPAD2, '2');
	AddKeyInfo(KEY_NUMPAD3, '3');
	AddKeyInfo(KEY_NUMPAD4, '4');
	AddKeyInfo(KEY_NUMPAD5, '5');
	AddKeyInfo(KEY_NUMPAD6, '6');
	AddKeyInfo(KEY_NUMPAD7, '7');
	AddKeyInfo(KEY_NUMPAD8, '8');
	AddKeyInfo(KEY_NUMPAD9, '9');

	AddKeyInfo(KEY_A, 'a', TRUE);
	AddKeyInfo(KEY_B, 'b', TRUE);
	AddKeyInfo(KEY_C, 'c', TRUE);
	AddKeyInfo(KEY_D, 'd', TRUE);
	AddKeyInfo(KEY_E, 'e', TRUE);
	AddKeyInfo(KEY_F, 'f', TRUE);
	AddKeyInfo(KEY_G, 'g', TRUE);
	AddKeyInfo(KEY_H, 'h', TRUE);
	AddKeyInfo(KEY_I, 'i', TRUE);
	AddKeyInfo(KEY_J, 'j', TRUE);
	AddKeyInfo(KEY_K, 'k', TRUE);
	AddKeyInfo(KEY_L, 'l', TRUE);
	AddKeyInfo(KEY_M, 'm', TRUE);
	AddKeyInfo(KEY_N, 'n', TRUE);
	AddKeyInfo(KEY_O, 'o', TRUE);
	AddKeyInfo(KEY_P, 'p', TRUE);
	AddKeyInfo(KEY_Q, 'q', TRUE);
	AddKeyInfo(KEY_R, 'r', TRUE);
	AddKeyInfo(KEY_S, 's', TRUE);
	AddKeyInfo(KEY_T, 't', TRUE);
	AddKeyInfo(KEY_U, 'u', TRUE);
	AddKeyInfo(KEY_V, 'v', TRUE);
	AddKeyInfo(KEY_W, 'w', TRUE);
	AddKeyInfo(KEY_X, 'x', TRUE);
	AddKeyInfo(KEY_Y, 'y', TRUE);
	AddKeyInfo(KEY_Z, 'z', TRUE);

	AddKeyInfo(KEY_SPACE,     ' ');
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
KEYBOARD::~KEYBOARD()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Sets up the keyboard
//+-----------------------------------------------------------------------------
BOOL KEYBOARD::Setup(HWND Window, BOOL Exclusive)
{
	DWORD Flags;

	Shutdown();

	if(FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<VOID**>(&DirectInput), NULL)))
	{
		Error.SetMessage("Unable to create a DirectInput object!");
		return FALSE;
	}

	if(FAILED(DirectInput->CreateDevice(GUID_SysKeyboard, &KeyboardDevice, NULL)))
	{
		Error.SetMessage("Unable to create a DirectInput keyboard device!");
		return FALSE;
	}

	if(FAILED(KeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		Error.SetMessage("Unable to set the DirectInput keyboard data format!");
		return FALSE;
	}

	Flags = DISCL_FOREGROUND | (Exclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE);

	if(FAILED(KeyboardDevice->SetCooperativeLevel(Window, Flags)))
	{
		Error.SetMessage("Unable to set the DirectInput keyboard cooperative level!");
		return FALSE;
	}

	KeyboardDevice->Acquire();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Shuts down the keyboard
//+-----------------------------------------------------------------------------
VOID KEYBOARD::Shutdown()
{
	if(KeyboardDevice != NULL) 
	{
		KeyboardDevice->Unacquire();
		KeyboardDevice->Release();
		KeyboardDevice = NULL;
	}

	if(DirectInput != NULL)
	{
		DirectInput->Release();
		DirectInput = NULL;
	}

	Delay = 0;
}


//+-----------------------------------------------------------------------------
//| Updates the keyboard state
//+-----------------------------------------------------------------------------
VOID KEYBOARD::Update()
{
	INT i;
	BYTE KeyboardState[NR_OF_KEYBOARD_KEYS];

	if(KeyboardDevice == NULL) return;

	ZeroMemory(&KeyboardState, sizeof(KeyboardState));
	if(FAILED(KeyboardDevice->GetDeviceState(sizeof(KeyboardState), &KeyboardState)))
	{
		KeyboardDevice->Acquire();

		ZeroMemory(&KeyboardState, sizeof(KeyboardState));
		if(FAILED(KeyboardDevice->GetDeviceState(sizeof(KeyboardState), &KeyboardState)))
		{
			return;
		}
	}

	AnyKeyState = KEYBOARD_KEY_STATE();

	for(i = 0; i < NR_OF_KEYBOARD_KEYS; i++)
	{
		KeyState[i].KeyPressed = FALSE;
		KeyState[i].KeyReleased = FALSE;

		if(KeyState[i].Delay > 0) KeyState[i].Delay--;

		if(KeyboardState[i] & 0x80)
		{
			if(!KeyState[i].KeyDown)
			{
				KeyState[i].Delay = Delay;
				KeyState[i].KeyPressed = TRUE;
				AnyKeyState.KeyPressed = TRUE;
			}

			KeyState[i].KeyDown = TRUE;

			if(ValidKeyDelay(KeyState[i].Delay))
			{
				AnyKeyState.KeyDown = TRUE;
			}
		}
		else
		{
			if(KeyState[i].KeyDown)
			{
				KeyState[i].Delay = 0;
				KeyState[i].KeyReleased = TRUE;
				AnyKeyState.KeyReleased = TRUE;
			}

			KeyState[i].KeyDown = FALSE;
		}
	}
}


//+-----------------------------------------------------------------------------
//| Checks if a keyboard key is down
//+-----------------------------------------------------------------------------
BOOL KEYBOARD::KeyDown(INT Key) CONST
{
	if(!ValidKey(Key)) return FALSE;

	return KeyState[Key].KeyDown && ValidKeyDelay(KeyState[Key].Delay);
}


//+-----------------------------------------------------------------------------
//| Checks if a keyboard key has just been pressed
//+-----------------------------------------------------------------------------
BOOL KEYBOARD::KeyPressed(INT Key) CONST
{
	if(!ValidKey(Key)) return FALSE;

	return KeyState[Key].KeyPressed;
}


//+-----------------------------------------------------------------------------
//| Checks if a keyboard key has just been released
//+-----------------------------------------------------------------------------
BOOL KEYBOARD::KeyReleased(INT Key) CONST
{
	if(!ValidKey(Key)) return FALSE;

	return KeyState[Key].KeyReleased;
}


//+-----------------------------------------------------------------------------
//| Checks if any keyboard key is down
//+-----------------------------------------------------------------------------
BOOL KEYBOARD::AnyKeyDown() CONST
{
	return AnyKeyState.KeyDown;
}


//+-----------------------------------------------------------------------------
//| Checks if any keyboard key has just been pressed
//+-----------------------------------------------------------------------------
BOOL KEYBOARD::AnyKeyPressed() CONST
{
	return AnyKeyState.KeyPressed;
}


//+-----------------------------------------------------------------------------
//| Checks if any keyboard key has just been released
//+-----------------------------------------------------------------------------
BOOL KEYBOARD::AnyKeyReleased() CONST
{
	return AnyKeyState.KeyReleased;
}


//+-----------------------------------------------------------------------------
//| Sets a new keyboard delay
//+-----------------------------------------------------------------------------
VOID KEYBOARD::SetDelay(INT NewDelay)
{
	Delay = (NewDelay > 0) ? NewDelay : 0;
}


//+-----------------------------------------------------------------------------
//| Returns the current keyboard delay
//+-----------------------------------------------------------------------------
INT KEYBOARD::GetDelay() CONST
{
	return Delay;
}


//+-----------------------------------------------------------------------------
//| Checks if a keyboard key is valid
//+-----------------------------------------------------------------------------
BOOL KEYBOARD::ValidKey(INT Key) CONST
{
	return (Key >= 0) && (Key < NR_OF_KEYBOARD_KEYS);
}


//+-----------------------------------------------------------------------------
//| Checks if a keyboard key delay is valid
//+-----------------------------------------------------------------------------
BOOL KEYBOARD::ValidKeyDelay(INT KeyDelay) CONST
{
	return (KeyDelay <= 0) || (KeyDelay >= Delay);
}


//+-----------------------------------------------------------------------------
//| Adds character information for certain keyboard keys
//+-----------------------------------------------------------------------------
VOID KEYBOARD::AddKeyInfo(INT Key, CHAR Character, BOOL Letter)
{
	KeyInfo[Key].Valid = TRUE;
	KeyInfo[Key].Letter = Letter;
	KeyInfo[Key].Character = Character;
}

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_KEYBOARD_H
#define MAGOS_KEYBOARD_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST INT NR_OF_KEYBOARD_KEYS = 256;


//+-----------------------------------------------------------------------------
//| Keyboard key constants
//+-----------------------------------------------------------------------------
CONST INT KEY_ESC             = 0x01;
CONST INT KEY_1               = 0x02;
CONST INT KEY_2               = 0x03;
CONST INT KEY_3               = 0x04;
CONST INT KEY_4               = 0x05;
CONST INT KEY_5               = 0x06;
CONST INT KEY_6               = 0x07;
CONST INT KEY_7               = 0x08;
CONST INT KEY_8               = 0x09;
CONST INT KEY_9               = 0x0A;
CONST INT KEY_0               = 0x0B;
CONST INT KEY_MINUS           = 0x0C;
CONST INT KEY_EQUALS          = 0x0D;
CONST INT KEY_BACKSPACE       = 0x0E;
CONST INT KEY_TAB             = 0x0F;
CONST INT KEY_Q               = 0x10;
CONST INT KEY_W               = 0x11;
CONST INT KEY_E               = 0x12;
CONST INT KEY_R               = 0x13;
CONST INT KEY_T               = 0x14;
CONST INT KEY_Y               = 0x15;
CONST INT KEY_U               = 0x16;
CONST INT KEY_I               = 0x17;
CONST INT KEY_O               = 0x18;
CONST INT KEY_P               = 0x19;
CONST INT KEY_LEFTBRACKET     = 0x1A;
CONST INT KEY_RIGHTBRACKET    = 0x1B;
CONST INT KEY_ENTER           = 0x1C;
CONST INT KEY_LEFTCTRL        = 0x1D;
CONST INT KEY_A               = 0x1E;
CONST INT KEY_S               = 0x1F;
CONST INT KEY_D               = 0x20;
CONST INT KEY_F               = 0x21;
CONST INT KEY_G               = 0x22;
CONST INT KEY_H               = 0x23;
CONST INT KEY_J               = 0x24;
CONST INT KEY_K               = 0x25;
CONST INT KEY_L               = 0x26;
CONST INT KEY_SEMICOLON       = 0x27;
CONST INT KEY_APOSTROPHE      = 0x28;
CONST INT KEY_ACCENTGRAVE     = 0x29;
CONST INT KEY_LEFTSHIFT       = 0x2A;
CONST INT KEY_BACKSLASH       = 0x2B;
CONST INT KEY_Z               = 0x2C;
CONST INT KEY_X               = 0x2D;
CONST INT KEY_C               = 0x2E;
CONST INT KEY_V               = 0x2F;
CONST INT KEY_B               = 0x30;
CONST INT KEY_N               = 0x31;
CONST INT KEY_M               = 0x32;
CONST INT KEY_COMMA           = 0x33;
CONST INT KEY_PERIOD          = 0x34;
CONST INT KEY_SLASH           = 0x35;
CONST INT KEY_RIGHTSHIFT      = 0x36;
CONST INT KEY_MULTIPLY        = 0x37;
CONST INT KEY_LEFTALT         = 0x38;
CONST INT KEY_SPACE           = 0x39;
CONST INT KEY_CAPITAL         = 0x3A;
CONST INT KEY_F1              = 0x3B;
CONST INT KEY_F2              = 0x3C;
CONST INT KEY_F3              = 0x3D;
CONST INT KEY_F4              = 0x3E;
CONST INT KEY_F5              = 0x3F;
CONST INT KEY_F6              = 0x40;
CONST INT KEY_F7              = 0x41;
CONST INT KEY_F8              = 0x42;
CONST INT KEY_F9              = 0x43;
CONST INT KEY_F10             = 0x44;
CONST INT KEY_NUMLOCK         = 0x45;
CONST INT KEY_SCROLL          = 0x46;
CONST INT KEY_NUMPAD7         = 0x47;
CONST INT KEY_NUMPAD8         = 0x48;
CONST INT KEY_NUMPAD9         = 0x49;
CONST INT KEY_SUBTRACT        = 0x4A;
CONST INT KEY_NUMPAD4         = 0x4B;
CONST INT KEY_NUMPAD5         = 0x4C;
CONST INT KEY_NUMPAD6         = 0x4D;
CONST INT KEY_ADD             = 0x4E;
CONST INT KEY_NUMPAD1         = 0x4F;
CONST INT KEY_NUMPAD2         = 0x50;
CONST INT KEY_NUMPAD3         = 0x51;
CONST INT KEY_NUMPAD0         = 0x52;
CONST INT KEY_DECIMAL         = 0x53;
CONST INT KEY_PIPE            = 0x56;
CONST INT KEY_F11             = 0x57;
CONST INT KEY_F12             = 0x58;
CONST INT KEY_F13             = 0x64;
CONST INT KEY_F14             = 0x65;
CONST INT KEY_F15             = 0x66;
CONST INT KEY_NUMPADEQUALS    = 0x8D;
CONST INT KEY_PREVTRACK       = 0x90;
CONST INT KEY_AT              = 0x91;
CONST INT KEY_COLON           = 0x92;
CONST INT KEY_UNDERLINE       = 0x93;
CONST INT KEY_STOP            = 0x95;
CONST INT KEY_NEXTTRACK       = 0x99;
CONST INT KEY_NUMPADENTER     = 0x9C;
CONST INT KEY_RIGHTCTRL       = 0x9D;
CONST INT KEY_MUTE            = 0xA0;
CONST INT KEY_CALCULATOR      = 0xA1;
CONST INT KEY_PLAYPAUSE       = 0xA2;
CONST INT KEY_MEDIASTOP       = 0xA4;
CONST INT KEY_VOLUMEDOWN      = 0xAE;
CONST INT KEY_VOLUMEUP        = 0xB0;
CONST INT KEY_WEBHOME         = 0xB2;
CONST INT KEY_NUMPADCOMMA     = 0xB3;
CONST INT KEY_DIVIDE          = 0xB5;
CONST INT KEY_SYSRQ           = 0xB7;
CONST INT KEY_RIGHTALT        = 0xB8;
CONST INT KEY_PAUSE           = 0xC5;
CONST INT KEY_HOME            = 0xC7;
CONST INT KEY_UP              = 0xC8;
CONST INT KEY_PRIOR           = 0xC9;
CONST INT KEY_LEFT            = 0xCB;
CONST INT KEY_RIGHT           = 0xCD;
CONST INT KEY_END             = 0xCF;
CONST INT KEY_DOWN            = 0xD0;
CONST INT KEY_NEXT            = 0xD1;
CONST INT KEY_INSERT          = 0xD2;
CONST INT KEY_DELETE          = 0xD3;
CONST INT KEY_LEFTWINDOWS     = 0xDB;
CONST INT KEY_RIGHTWINDOWS    = 0xDC;
CONST INT KEY_APPS            = 0xDD;
CONST INT KEY_POWER           = 0xDE;
CONST INT KEY_SLEEP           = 0xDF;
CONST INT KEY_WAKE            = 0xE3;
CONST INT KEY_WEBSEARCH       = 0xE5;
CONST INT KEY_WEBFAVORITES    = 0xE6;
CONST INT KEY_WEBREFRESH      = 0xE7;
CONST INT KEY_WEBSTOP         = 0xE8;
CONST INT KEY_WEBFORWARD      = 0xE9;
CONST INT KEY_WEBBACK         = 0xEA;
CONST INT KEY_MYCOMPUTER      = 0xEB;
CONST INT KEY_MAIL            = 0xEC;
CONST INT KEY_MEDIASELECT     = 0xED;
CONST INT KEY_NUMPADSTAR      = KEY_MULTIPLY;
CONST INT KEY_CAPSLOCK        = KEY_CAPITAL;
CONST INT KEY_NUMPADMINUS     = KEY_SUBTRACT;
CONST INT KEY_NUMPADPLUS      = KEY_ADD;
CONST INT KEY_NUMPADPERIOD    = KEY_DECIMAL;
CONST INT KEY_NUMPADSLASH     = KEY_DIVIDE;
CONST INT KEY_UPARROW         = KEY_UP;
CONST INT KEY_PAGEUP          = KEY_PRIOR;
CONST INT KEY_LEFTARROW       = KEY_LEFT;
CONST INT KEY_RIGHTARROW      = KEY_RIGHT;
CONST INT KEY_DOWNARROW       = KEY_DOWN;
CONST INT KEY_PAGEDOWN        = KEY_NEXT;
CONST INT KEY_CIRCUMFLEX      = KEY_PREVTRACK;


//+-----------------------------------------------------------------------------
//| Keyboard key info structure
//+-----------------------------------------------------------------------------
struct KEYBOARD_KEY_INFO
{
	KEYBOARD_KEY_INFO()
	{
		Valid = FALSE;
		Letter = FALSE;
		Character = ' ';
	}

	BOOL Valid;
	BOOL Letter;
	CHAR Character;
};


//+-----------------------------------------------------------------------------
//| Keyboard key state structure
//+-----------------------------------------------------------------------------
struct KEYBOARD_KEY_STATE
{
	KEYBOARD_KEY_STATE()
	{
		Delay = 0;
		KeyDown = FALSE;
		KeyPressed = FALSE;
		KeyReleased = FALSE;
	}

	INT Delay;
	BOOL KeyDown;
	BOOL KeyPressed;
	BOOL KeyReleased;
};


//+-----------------------------------------------------------------------------
//| Keyboard class
//+-----------------------------------------------------------------------------
class KEYBOARD
{
	public:
		CONSTRUCTOR KEYBOARD();
		DESTRUCTOR ~KEYBOARD();

		BOOL Setup(HWND Window, BOOL Exclusive = FALSE);
		VOID Shutdown();

		VOID Update();

		BOOL KeyDown(INT Key) CONST;
		BOOL KeyPressed(INT Key) CONST;
		BOOL KeyReleased(INT Key) CONST;

		BOOL AnyKeyDown() CONST;
		BOOL AnyKeyPressed() CONST;
		BOOL AnyKeyReleased() CONST;

		VOID SetDelay(INT NewDelay);
		INT GetDelay() CONST;

	protected:
		BOOL ValidKey(INT Key) CONST;
		BOOL ValidKeyDelay(INT KeyDelay) CONST;

		VOID AddKeyInfo(INT Key, CHAR Character, BOOL Letter = FALSE);

		LPDIRECTINPUT8 DirectInput;
		LPDIRECTINPUTDEVICE8 KeyboardDevice;

		INT Delay;
		KEYBOARD_KEY_STATE AnyKeyState;
		KEYBOARD_KEY_STATE KeyState[NR_OF_KEYBOARD_KEYS];

		KEYBOARD_KEY_INFO KeyInfo[NR_OF_KEYBOARD_KEYS];
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern KEYBOARD Keyboard;
extern KEYBOARD ModelKeyboard;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

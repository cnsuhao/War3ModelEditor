//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_FRAME_H
#define MAGOS_WINDOW_FRAME_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Window.h"


//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST INT WINDOW_CENTER_POSITION = 13371337;


//+-----------------------------------------------------------------------------
//| Window frame info structure
//+-----------------------------------------------------------------------------
struct WINDOW_FRAME_INFO
{
	WINDOW_FRAME_INFO()
	{
		Title = "";

		X = WINDOW_CENTER_POSITION;
		Y = WINDOW_CENTER_POSITION;
		Width = 400;
		Height = 300;

		Parent = NULL;
		Menu = NULL;
		Icon = LoadIcon(NULL, IDI_APPLICATION);
		Cursor = LoadCursor(NULL, IDC_ARROW);
		Background = NULL;
		Style = WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE;
	}

	std::string Title;

	INT X;
	INT Y;
	INT Width;
	INT Height;

	HWND Parent;
	HMENU Menu;
	HICON Icon;
	HCURSOR Cursor;
	HBRUSH Background;
	DWORD Style;
};


//+-----------------------------------------------------------------------------
//| Window frame class
//+-----------------------------------------------------------------------------
class WINDOW_FRAME : public WINDOW
{
	public:
		CONSTRUCTOR WINDOW_FRAME();
		DESTRUCTOR ~WINDOW_FRAME();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

		VOID SetWindowWidth(INT NewWidth);
		VOID SetWindowHeight(INT NewHeight);
		VOID SetWindowDimension(INT NewWidth, INT NewHeight);

		VOID SetMenu(HMENU NewMenu);
		HMENU GetMenu() CONST;

		VOID CheckMenuItem(INT MenuItem, BOOL Check = TRUE);
		VOID EnableMenuItem(INT MenuItem, BOOL Enable = TRUE);
		VOID GrayMenuItem(INT MenuItem, BOOL Gray = TRUE);

	protected:
		RECT MakeWindowRect(INT Width, INT Height) CONST;

		WINDOW_FRAME_INFO FrameInfo;

		BOOL ClassRegistered;
		std::string ClassName;

		static LRESULT CALLBACK StaticMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

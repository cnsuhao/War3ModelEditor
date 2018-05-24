//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowRegisterDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_REGISTER_DIALOG RegisterDialog;



//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
REGISTER_INFO WINDOW_REGISTER_DIALOG::StaticRegisterInfo;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_REGISTER_DIALOG::WINDOW_REGISTER_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_REGISTER_DIALOG::~WINDOW_REGISTER_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_REGISTER_DIALOG::Display(HWND ParentWindow, REGISTER_INFO& RegisterInfo) CONST
{
	StaticRegisterInfo = RegisterInfo;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogRegister), ParentWindow, DialogMessageHandler, 0))
	{
		RegisterInfo = StaticRegisterInfo;
		return TRUE;
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_REGISTER_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			CenterWindow(Window);

			CheckDlgButton(Window, DialogRegisterButtonMdl, BoolToCheckState(StaticRegisterInfo.Mdl));
			CheckDlgButton(Window, DialogRegisterButtonMdx, BoolToCheckState(StaticRegisterInfo.Mdx));
			CheckDlgButton(Window, DialogRegisterButtonMpq, BoolToCheckState(StaticRegisterInfo.Mpq));
			CheckDlgButton(Window, DialogRegisterButtonW3m, BoolToCheckState(StaticRegisterInfo.W3m));
			CheckDlgButton(Window, DialogRegisterButtonW3x, BoolToCheckState(StaticRegisterInfo.W3x));
			CheckDlgButton(Window, DialogRegisterButtonW3n, BoolToCheckState(StaticRegisterInfo.W3n));
			CheckDlgButton(Window, DialogRegisterButtonBmp, BoolToCheckState(StaticRegisterInfo.Bmp));
			CheckDlgButton(Window, DialogRegisterButtonTga, BoolToCheckState(StaticRegisterInfo.Tga));
			CheckDlgButton(Window, DialogRegisterButtonPng, BoolToCheckState(StaticRegisterInfo.Png));
			CheckDlgButton(Window, DialogRegisterButtonJpg, BoolToCheckState(StaticRegisterInfo.Jpg));
			CheckDlgButton(Window, DialogRegisterButtonJpeg, BoolToCheckState(StaticRegisterInfo.Jpeg));
			CheckDlgButton(Window, DialogRegisterButtonPcx, BoolToCheckState(StaticRegisterInfo.Pcx));
			CheckDlgButton(Window, DialogRegisterButtonBlp, BoolToCheckState(StaticRegisterInfo.Blp));

			CheckDlgButton(Window, DialogRegisterButtonOpenWith, BoolToCheckState(StaticRegisterInfo.OpenWith));

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogRegisterButtonOk:
				{
					StaticRegisterInfo.Mdl = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonMdl));
					StaticRegisterInfo.Mdx = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonMdx));
					StaticRegisterInfo.Mpq = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonMpq));
					StaticRegisterInfo.W3m = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonW3m));
					StaticRegisterInfo.W3x = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonW3x));
					StaticRegisterInfo.W3n = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonW3n));
					StaticRegisterInfo.Bmp = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonBmp));
					StaticRegisterInfo.Tga = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonTga));
					StaticRegisterInfo.Png = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonPng));
					StaticRegisterInfo.Jpg = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonJpg));
					StaticRegisterInfo.Jpeg = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonJpeg));
					StaticRegisterInfo.Pcx = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonPcx));
					StaticRegisterInfo.Blp = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonBlp));

					StaticRegisterInfo.OpenWith = CheckStateToBool(IsDlgButtonChecked(Window, DialogRegisterButtonOpenWith));

					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogRegisterButtonCancel:
				{
					EndDialog(Window, 0);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogRegisterButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

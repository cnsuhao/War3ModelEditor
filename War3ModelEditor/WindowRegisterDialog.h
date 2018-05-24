//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_REGISTER_DIALOG_H
#define MAGOS_WINDOW_REGISTER_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"


//+-----------------------------------------------------------------------------
//| Register info structure
//+-----------------------------------------------------------------------------
struct REGISTER_INFO
{
	REGISTER_INFO()
	{
		Mdl = TRUE;
		Mdx = TRUE;
		Mpq = TRUE;
		W3m = FALSE;
		W3x = FALSE;
		W3n = FALSE;
		Bmp = FALSE;
		Tga = FALSE;
		Png = FALSE;
		Jpg = FALSE;
		Jpeg = FALSE;
		Pcx = FALSE;
		Blp = TRUE;

		OpenWith = TRUE;
	}

	BOOL Mdl;
	BOOL Mdx;
	BOOL Mpq;
	BOOL W3m;
	BOOL W3x;
	BOOL W3n;
	BOOL Bmp;
	BOOL Tga;
	BOOL Png;
	BOOL Jpg;
	BOOL Jpeg;
	BOOL Pcx;
	BOOL Blp;

	BOOL OpenWith;
};


//+-----------------------------------------------------------------------------
//| Register dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_REGISTER_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_REGISTER_DIALOG();
		DESTRUCTOR ~WINDOW_REGISTER_DIALOG();

		BOOL Display(HWND ParentWindow, REGISTER_INFO& RegisterInfo) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static REGISTER_INFO StaticRegisterInfo;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_REGISTER_DIALOG RegisterDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

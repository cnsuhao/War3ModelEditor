//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_COLORED_TEXT_DIALOG_H
#define MAGOS_WINDOW_COLORED_TEXT_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"


//+-----------------------------------------------------------------------------
//| Colored text dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_COLORED_TEXT_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_COLORED_TEXT_DIALOG();
		DESTRUCTOR ~WINDOW_COLORED_TEXT_DIALOG();

		BOOL Display(HWND ParentWindow) CONST;

	protected:
		static BOOL ColorizeSolidText(std::string& Text);
		static BOOL ColorizeGradientText(std::string& Text);

		static std::string ColorToString(D3DCOLOR Color);
		static std::string NumberToString(INT Number);
		static CHAR DecToHex(INT Number);
		static D3DCOLOR InterpolateColor(INT Index, INT Size);

		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static BOOL Gradient;
		static D3DCOLOR Color1;
		static D3DCOLOR Color2;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_COLORED_TEXT_DIALOG ColoredTextDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

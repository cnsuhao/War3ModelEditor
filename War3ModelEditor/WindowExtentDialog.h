//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_EXTENT_DIALOG_H
#define MAGOS_WINDOW_EXTENT_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Extent dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_EXTENT_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_EXTENT_DIALOG();
		DESTRUCTOR ~WINDOW_EXTENT_DIALOG();

		BOOL Display(HWND ParentWindow, EXTENT* Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static EXTENT* StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_EXTENT_DIALOG ExtentDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

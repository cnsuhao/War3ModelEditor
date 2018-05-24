//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_MODEL_INFO_DIALOG_H
#define MAGOS_WINDOW_MODEL_INFO_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Model info dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_MODEL_INFO_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_MODEL_INFO_DIALOG();
		DESTRUCTOR ~WINDOW_MODEL_INFO_DIALOG();

		VOID Display(HWND ParentWindow) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_MODEL_INFO_DIALOG ModelInfoDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

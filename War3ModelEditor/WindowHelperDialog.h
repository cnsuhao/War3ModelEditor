//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_HELPER_DIALOG_H
#define MAGOS_WINDOW_HELPER_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Helper dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_HELPER_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_HELPER_DIALOG();
		DESTRUCTOR ~WINDOW_HELPER_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_HELPER_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_HELPER_DATA StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_HELPER_DIALOG HelperDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

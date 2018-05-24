//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_GLOBAL_SEQUENCE_DIALOG_H
#define MAGOS_WINDOW_GLOBAL_SEQUENCE_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Global sequence dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_GLOBAL_SEQUENCE_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_GLOBAL_SEQUENCE_DIALOG();
		DESTRUCTOR ~WINDOW_GLOBAL_SEQUENCE_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_GLOBAL_SEQUENCE_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_GLOBAL_SEQUENCE_DATA StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_GLOBAL_SEQUENCE_DIALOG GlobalSequenceDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

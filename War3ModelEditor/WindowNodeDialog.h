//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_NODE_DIALOG_H
#define MAGOS_WINDOW_NODE_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Node dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_NODE_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_NODE_DIALOG();
		DESTRUCTOR ~WINDOW_NODE_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_BASE_DATA& Data, BOOL NoNameChange) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_BASE_DATA StaticData;
		static BOOL StaticNoNameChange;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_NODE_DIALOG NodeDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

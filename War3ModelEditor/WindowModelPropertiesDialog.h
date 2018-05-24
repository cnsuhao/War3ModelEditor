//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_MODEL_PROPERTIES_DIALOG_H
#define MAGOS_WINDOW_MODEL_PROPERTIES_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Model properties dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_MODEL_PROPERTIES_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_MODEL_PROPERTIES_DIALOG();
		DESTRUCTOR ~WINDOW_MODEL_PROPERTIES_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_INFO_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_INFO_DATA StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_MODEL_PROPERTIES_DIALOG ModelPropertiesDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

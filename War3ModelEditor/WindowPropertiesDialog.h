//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_PROPERTIES_DIALOG_H
#define MAGOS_WINDOW_PROPERTIES_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "WindowFileDialog.h"


//+-----------------------------------------------------------------------------
//| Properties dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_PROPERTIES_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_PROPERTIES_DIALOG();
		DESTRUCTOR ~WINDOW_PROPERTIES_DIALOG();

		BOOL Display(HWND ParentWindow, PROPERTIES_INFO& PropertiesInfo) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static PROPERTIES_INFO StaticPropertiesInfo;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_PROPERTIES_DIALOG PropertiesDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

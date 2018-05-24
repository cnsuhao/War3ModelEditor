//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_QUALITY_DIALOG_H
#define MAGOS_WINDOW_QUALITY_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"


//+-----------------------------------------------------------------------------
//| Quality dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_QUALITY_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_QUALITY_DIALOG();
		DESTRUCTOR ~WINDOW_QUALITY_DIALOG();

		BOOL Display(HWND ParentWindow, INT& Quality) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static INT StaticQuality;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_QUALITY_DIALOG QualityDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

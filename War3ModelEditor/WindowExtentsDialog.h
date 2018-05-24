//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_EXTENTS_DIALOG_H
#define MAGOS_WINDOW_EXTENTS_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowExtentDialog.h"


//+-----------------------------------------------------------------------------
//| Extents dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_EXTENTS_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_EXTENTS_DIALOG();
		DESTRUCTOR ~WINDOW_EXTENTS_DIALOG();

		BOOL Display(HWND ParentWindow, SIMPLE_CONTAINER<EXTENT*>* Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static BOOL CreateNew(HWND Window);
		static BOOL Remove(HWND Window);
		static BOOL Edit(HWND Window);

		static SIMPLE_CONTAINER<EXTENT*>* StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_EXTENTS_DIALOG ExtentsDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

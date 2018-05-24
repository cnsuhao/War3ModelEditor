//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_GROUPS_DIALOG_H
#define MAGOS_WINDOW_GROUPS_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowGroupDialog.h"


//+-----------------------------------------------------------------------------
//| Groups dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_GROUPS_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_GROUPS_DIALOG();
		DESTRUCTOR ~WINDOW_GROUPS_DIALOG();

		BOOL Display(HWND ParentWindow, SIMPLE_CONTAINER<MODEL_GEOSET_GROUP*>* Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static BOOL CreateNew(HWND Window);
		static BOOL Remove(HWND Window);
		static BOOL Edit(HWND Window);

		static SIMPLE_CONTAINER<MODEL_GEOSET_GROUP*>* StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_GROUPS_DIALOG GroupsDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

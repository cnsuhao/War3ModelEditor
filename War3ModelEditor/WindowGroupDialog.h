//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_GROUP_DIALOG_H
#define MAGOS_WINDOW_GROUP_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Group dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_GROUP_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_GROUP_DIALOG();
		DESTRUCTOR ~WINDOW_GROUP_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_GEOSET_GROUP* Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildList(HWND Window);
		static VOID ExtractList(HWND Window);

		static MODEL_GEOSET_GROUP* StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_GROUP_DIALOG GroupDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

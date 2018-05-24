//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_IMPORT_GEOSET_DIALOG_H
#define MAGOS_WINDOW_IMPORT_GEOSET_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Import geoset dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_IMPORT_GEOSET_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_IMPORT_GEOSET_DIALOG();
		DESTRUCTOR ~WINDOW_IMPORT_GEOSET_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_BASE*& Node) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildNodeIdList(HWND Window);

		static MODEL_BASE* StaticNode;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_IMPORT_GEOSET_DIALOG ImportGeosetDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

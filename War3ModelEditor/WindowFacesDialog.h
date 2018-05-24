//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_FACES_DIALOG_H
#define MAGOS_WINDOW_FACES_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowFaceDialog.h"


//+-----------------------------------------------------------------------------
//| Faces dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_FACES_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_FACES_DIALOG();
		DESTRUCTOR ~WINDOW_FACES_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_GEOSET* Geoset, SIMPLE_CONTAINER<MODEL_GEOSET_FACE*>* Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static BOOL CreateNew(HWND Window);
		static BOOL Remove(HWND Window);
		static BOOL Edit(HWND Window);

		static SIMPLE_CONTAINER<MODEL_GEOSET_FACE*>* StaticData;
		static MODEL_GEOSET* StaticGeoset;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_FACES_DIALOG FacesDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

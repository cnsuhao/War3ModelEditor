//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_VERTICES_DIALOG_H
#define MAGOS_WINDOW_VERTICES_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowVertexDialog.h"


//+-----------------------------------------------------------------------------
//| Vertices dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_VERTICES_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_VERTICES_DIALOG();
		DESTRUCTOR ~WINDOW_VERTICES_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_GEOSET* Geoset, SIMPLE_CONTAINER<MODEL_GEOSET_VERTEX*>* Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static BOOL CreateNew(HWND Window);
		static BOOL Remove(HWND Window);
		static BOOL Edit(HWND Window);

		static SIMPLE_CONTAINER<MODEL_GEOSET_VERTEX*>* StaticData;
		static MODEL_GEOSET* StaticGeoset;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_VERTICES_DIALOG VerticesDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_VERTEX_DIALOG_H
#define MAGOS_WINDOW_VERTEX_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Vertex dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_VERTEX_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_VERTEX_DIALOG();
		DESTRUCTOR ~WINDOW_VERTEX_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_GEOSET* Geoset, MODEL_GEOSET_VERTEX* Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildGroupIdList(HWND Window);

		static MODEL_GEOSET_VERTEX* StaticData;
		static MODEL_GEOSET* StaticGeoset;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_VERTEX_DIALOG VertexDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

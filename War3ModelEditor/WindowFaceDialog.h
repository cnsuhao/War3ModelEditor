//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_FACE_DIALOG_H
#define MAGOS_WINDOW_FACE_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Face dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_FACE_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_FACE_DIALOG();
		DESTRUCTOR ~WINDOW_FACE_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_GEOSET* Geoset, MODEL_GEOSET_FACE* Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildVertexIdList(HWND Window, UINT ComboId, INT SelectedVertexId);

		static MODEL_GEOSET_FACE* StaticData;
		static MODEL_GEOSET* StaticGeoset;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_FACE_DIALOG FaceDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_GEOSET_DIALOG_H
#define MAGOS_WINDOW_GEOSET_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Geoset dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_GEOSET_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_GEOSET_DIALOG();
		DESTRUCTOR ~WINDOW_GEOSET_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_GEOSET* Geoset, MODEL_GEOSET_DATA& Data, MODEL_MATERIAL*& Material) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildMaterialIdList(HWND Window);

		static MODEL_GEOSET_DATA StaticData;
		static MODEL_GEOSET* StaticGeoset;
		static MODEL_MATERIAL* StaticMaterial;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_GEOSET_DIALOG GeosetDialog;


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "WindowVertexDialog.h"
#include "WindowFaceDialog.h"
#include "WindowGroupDialog.h"
#include "WindowExtentDialog.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

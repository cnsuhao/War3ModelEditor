//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_GEOSET_ANIMATION_DIALOG_H
#define MAGOS_WINDOW_GEOSET_ANIMATION_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Geoset animation dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_GEOSET_ANIMATION_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_GEOSET_ANIMATION_DIALOG();
		DESTRUCTOR ~WINDOW_GEOSET_ANIMATION_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_GEOSET_ANIMATION_DATA& Data, MODEL_GEOSET*& Geoset) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildGeosetIdList(HWND Window);

		static MODEL_GEOSET_ANIMATION_DATA StaticData;
		static MODEL_GEOSET* StaticGeoset;
		static D3DCOLOR StaticColor;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_GEOSET_ANIMATION_DIALOG GeosetAnimationDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

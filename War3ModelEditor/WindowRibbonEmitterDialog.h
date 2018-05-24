//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_RIBBON_EMITTER_DIALOG_H
#define MAGOS_WINDOW_RIBBON_EMITTER_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Ribbon emitter dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_RIBBON_EMITTER_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_RIBBON_EMITTER_DIALOG();
		DESTRUCTOR ~WINDOW_RIBBON_EMITTER_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_RIBBON_EMITTER_DATA& Data, MODEL_MATERIAL*& Material) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildMaterialIdList(HWND Window);

		static MODEL_RIBBON_EMITTER_DATA StaticData;
		static MODEL_MATERIAL* StaticMaterial;
		static D3DCOLOR StaticColor;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_RIBBON_EMITTER_DIALOG RibbonEmitterDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

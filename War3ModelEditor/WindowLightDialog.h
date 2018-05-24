//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_LIGHT_DIALOG_H
#define MAGOS_WINDOW_LIGHT_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Light dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_LIGHT_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_LIGHT_DIALOG();
		DESTRUCTOR ~WINDOW_LIGHT_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_LIGHT_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_LIGHT_DATA StaticData;
		static D3DCOLOR StaticColor;
		static D3DCOLOR StaticAmbientColor;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_LIGHT_DIALOG LightDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

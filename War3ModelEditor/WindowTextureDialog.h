//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_TEXTURE_DIALOG_H
#define MAGOS_WINDOW_TEXTURE_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Texture dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_TEXTURE_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_TEXTURE_DIALOG();
		DESTRUCTOR ~WINDOW_TEXTURE_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_TEXTURE_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_TEXTURE_DATA StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_TEXTURE_DIALOG TextureDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

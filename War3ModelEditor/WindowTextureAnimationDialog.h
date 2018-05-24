//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_TEXTURE_ANIMATION_DIALOG_H
#define MAGOS_WINDOW_TEXTURE_ANIMATION_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Texture animation dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_TEXTURE_ANIMATION_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_TEXTURE_ANIMATION_DIALOG();
		DESTRUCTOR ~WINDOW_TEXTURE_ANIMATION_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_TEXTURE_ANIMATION_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_TEXTURE_ANIMATION_DATA StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_TEXTURE_ANIMATION_DIALOG TextureAnimationDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

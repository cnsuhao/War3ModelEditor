//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_LOADING_SCREEN_DIALOG_H
#define MAGOS_WINDOW_LOADING_SCREEN_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "WindowFileDialog.h"


//+-----------------------------------------------------------------------------
//| Loading screen dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_LOADING_SCREEN_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_LOADING_SCREEN_DIALOG();
		DESTRUCTOR ~WINDOW_LOADING_SCREEN_DIALOG();

		BOOL Display(HWND ParentWindow, std::string& TextureFileName) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static std::string StaticTextureFileName;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_LOADING_SCREEN_DIALOG LoadingScreenDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

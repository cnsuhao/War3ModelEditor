//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_TRANSLATE_DIALOG_H
#define MAGOS_WINDOW_TRANSLATE_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Translate info structure
//+-----------------------------------------------------------------------------
struct TRANSLATE_INFO
{
	TRANSLATE_INFO()
	{
		Translation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	D3DXVECTOR3 Translation;
};


//+-----------------------------------------------------------------------------
//| Translate dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_TRANSLATE_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_TRANSLATE_DIALOG();
		DESTRUCTOR ~WINDOW_TRANSLATE_DIALOG();

		BOOL Display(HWND ParentWindow, TRANSLATE_INFO& TranslateInfo) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static TRANSLATE_INFO StaticTranslateInfo;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_TRANSLATE_DIALOG TranslateDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_ATTACHMENT_DIALOG_H
#define MAGOS_WINDOW_ATTACHMENT_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Attachment dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_ATTACHMENT_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_ATTACHMENT_DIALOG();
		DESTRUCTOR ~WINDOW_ATTACHMENT_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_ATTACHMENT_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_ATTACHMENT_DATA StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_ATTACHMENT_DIALOG AttachmentDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

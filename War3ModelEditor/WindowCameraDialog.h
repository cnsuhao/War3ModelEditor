//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_CAMERA_DIALOG_H
#define MAGOS_WINDOW_CAMERA_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Camera dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_CAMERA_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_CAMERA_DIALOG();
		DESTRUCTOR ~WINDOW_CAMERA_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_CAMERA_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_CAMERA_DATA StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_CAMERA_DIALOG CameraDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_SCALE_DIALOG_H
#define MAGOS_WINDOW_SCALE_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Scale info structure
//+-----------------------------------------------------------------------------
struct SCALE_INFO
{
	SCALE_INFO()
	{
		Scaling = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		PivotPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		AroundPivotPoint = TRUE;
	}

	D3DXVECTOR3 Scaling;
	D3DXVECTOR3 PivotPoint;
	BOOL AroundPivotPoint;
};


//+-----------------------------------------------------------------------------
//| Scale dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_SCALE_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_SCALE_DIALOG();
		DESTRUCTOR ~WINDOW_SCALE_DIALOG();

		BOOL Display(HWND ParentWindow, SCALE_INFO& ScaleInfo) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static SCALE_INFO StaticScaleInfo;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_SCALE_DIALOG ScaleDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

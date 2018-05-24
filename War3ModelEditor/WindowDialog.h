//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_DIALOG_H
#define MAGOS_WINDOW_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_DIALOG();
		DESTRUCTOR ~WINDOW_DIALOG();

		static VOID CloseAllOpenDialogs();

	protected:
		static VOID CenterWindow(HWND Window);

		static VOID SetInt(HWND Control, INT NewInt);
		static VOID SetFloat(HWND Control, FLOAT NewFloat);
		static VOID SetString(HWND Control, CONST std::string& NewString);

		static INT GetInt(HWND Control, INT DefaultInt = 0);
		static FLOAT GetFloat(HWND Control, FLOAT DefaultFloat = 0.0f);
		static std::string GetString(HWND Control);

		static UINT BoolToCheckState(BOOL Bool);
		static BOOL CheckStateToBool(UINT CheckState);

		static BOOL SelectColor(HWND Window, D3DCOLOR& Color);
		static VOID RenderColorBox(HWND Window, HWND Control, D3DCOLOR Color);

		static VOID AddWindow(HWND Window);
		static VOID RemoveWindow(HWND Window);

		static std::set<HWND> OpenDialogSet;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "WindowInterpolatorDialog.h"
#include "WindowMaterialLayerDialog.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

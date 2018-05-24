//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_INTERPOLATOR_DIALOG_H
#define MAGOS_WINDOW_INTERPOLATOR_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Interpolator dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_INTERPOLATOR_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_INTERPOLATOR_DIALOG();
		DESTRUCTOR ~WINDOW_INTERPOLATOR_DIALOG();

		BOOL Display(HWND ParentWindow, INTERPOLATOR& Data, CONST std::string& Title) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildGlobalSequenceIdList(HWND Window);

		static VOID BuildList(HWND ListBox);
		static BOOL RetrieveList(HWND ListBox);

		static VOID BuildVector(std::stringstream& Stream, CONST D3DXVECTOR4& Vector);
		static BOOL RetrieveVector(std::stringstream& Stream, D3DXVECTOR4& Vector);
		static BOOL ExpectChar(std::stringstream& Stream, CHAR Char);

		static INTERPOLATOR StaticData;
		static MODEL_GLOBAL_SEQUENCE* StaticGlobalSequence;
		static std::string StaticTitle;

		static HFONT StaticFont;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_INTERPOLATOR_DIALOG InterpolatorDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

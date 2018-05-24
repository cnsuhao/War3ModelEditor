//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_MATERIAL_DIALOG_H
#define MAGOS_WINDOW_MATERIAL_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Material dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_MATERIAL_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_MATERIAL_DIALOG();
		DESTRUCTOR ~WINDOW_MATERIAL_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_MATERIAL_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static BOOL CreateNew(HWND Window);
		static BOOL Edit(HWND Window);
		static BOOL Remove(HWND Window);
		static BOOL MoveUp(HWND Window);
		static BOOL MoveDown(HWND Window);

		static MODEL_MATERIAL_DATA StaticData;
		static std::list<MODEL_MATERIAL_LAYER*> StaticLayerList;
		static std::list<MODEL_MATERIAL_LAYER*> StaticAddedLayerList;
		static std::list<MODEL_MATERIAL_LAYER*> StaticRemovedLayerList;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_MATERIAL_DIALOG MaterialDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

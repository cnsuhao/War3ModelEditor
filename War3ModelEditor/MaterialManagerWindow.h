//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MATERIAL_MANAGER_WINDOW_H
#define MAGOS_MATERIAL_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Material manager window class
//+-----------------------------------------------------------------------------
class MATERIAL_MANAGER_WINDOW : public MANAGER_WINDOW
{
	public:
		CONSTRUCTOR MATERIAL_MANAGER_WINDOW();
		DESTRUCTOR ~MATERIAL_MANAGER_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

		VOID SelectMaterial(MODEL_MATERIAL* Material);

	protected:
		BOOL CreateNew();
		BOOL Remove(HWND Window);
		BOOL Edit();
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MATERIAL_MANAGER_WINDOW MaterialManagerWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

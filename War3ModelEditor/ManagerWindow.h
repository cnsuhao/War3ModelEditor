//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MANAGER_WINDOW_H
#define MAGOS_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Window.h"


//+-----------------------------------------------------------------------------
//| Manager window class
//+-----------------------------------------------------------------------------
class MANAGER_WINDOW : public WINDOW_FRAME
{
	public:
		CONSTRUCTOR MANAGER_WINDOW();
		DESTRUCTOR ~MANAGER_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

		VOID AddName(INT Id, CONST std::string& Name);
		VOID RemoveName(INT Id);
		VOID ClearAllNames();

		BOOL HasName(CONST std::string& Name);

	protected:
		VOID ResizeListBox();

		WINDOW_LISTBOX ListBox;

		INT ManagerMenuId1;
		INT ManagerMenuId2;

		std::string Title;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

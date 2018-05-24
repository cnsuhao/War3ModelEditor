//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_EVENT_OBJECT_MANAGER_WINDOW_H
#define MAGOS_EVENT_OBJECT_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Event object manager window class
//+-----------------------------------------------------------------------------
class EVENT_OBJECT_MANAGER_WINDOW : public MANAGER_WINDOW
{
	public:
		CONSTRUCTOR EVENT_OBJECT_MANAGER_WINDOW();
		DESTRUCTOR ~EVENT_OBJECT_MANAGER_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

	protected:
		BOOL CreateNew();
		BOOL Remove();
		BOOL Edit();
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern EVENT_OBJECT_MANAGER_WINDOW EventObjectManagerWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

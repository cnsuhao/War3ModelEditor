//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_GLOBAL_SEQUENCE_MANAGER_WINDOW_H
#define MAGOS_GLOBAL_SEQUENCE_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global sequence manager window class
//+-----------------------------------------------------------------------------
class GLOBAL_SEQUENCE_MANAGER_WINDOW : public MANAGER_WINDOW
{
	public:
		CONSTRUCTOR GLOBAL_SEQUENCE_MANAGER_WINDOW();
		DESTRUCTOR ~GLOBAL_SEQUENCE_MANAGER_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

	protected:
		BOOL CreateNew();
		BOOL Remove(HWND Window);
		BOOL Edit();
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern GLOBAL_SEQUENCE_MANAGER_WINDOW GlobalSequenceManagerWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

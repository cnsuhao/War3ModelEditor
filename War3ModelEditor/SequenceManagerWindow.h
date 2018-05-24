//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_SEQUENCE_MANAGER_WINDOW_H
#define MAGOS_SEQUENCE_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Sequence manager window class
//+-----------------------------------------------------------------------------
class SEQUENCE_MANAGER_WINDOW : public MANAGER_WINDOW
{
	public:
		CONSTRUCTOR SEQUENCE_MANAGER_WINDOW();
		DESTRUCTOR ~SEQUENCE_MANAGER_WINDOW();

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
extern SEQUENCE_MANAGER_WINDOW SequenceManagerWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

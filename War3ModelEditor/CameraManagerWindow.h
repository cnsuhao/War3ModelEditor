//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_CAMERA_MANAGER_WINDOW_H
#define MAGOS_CAMERA_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Camera manager window class
//+-----------------------------------------------------------------------------
class CAMERA_MANAGER_WINDOW : public MANAGER_WINDOW
{
	public:
		CONSTRUCTOR CAMERA_MANAGER_WINDOW();
		DESTRUCTOR ~CAMERA_MANAGER_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

	protected:
		BOOL CreateNew();
		BOOL CreateNewFromView();
		BOOL Remove();
		BOOL Edit();
		BOOL View();
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern CAMERA_MANAGER_WINDOW CameraManagerWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

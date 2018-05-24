//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_GEOSET_ANIMATION_MANAGER_WINDOW_H
#define MAGOS_GEOSET_ANIMATION_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Geoset animation manager window class
//+-----------------------------------------------------------------------------
class GEOSET_ANIMATION_MANAGER_WINDOW : public MANAGER_WINDOW
{
	public:
		CONSTRUCTOR GEOSET_ANIMATION_MANAGER_WINDOW();
		DESTRUCTOR ~GEOSET_ANIMATION_MANAGER_WINDOW();

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
extern GEOSET_ANIMATION_MANAGER_WINDOW GeosetAnimationManagerWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

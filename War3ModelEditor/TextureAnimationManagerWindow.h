//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TEXTURE_ANIMATION_MANAGER_WINDOW_H
#define MAGOS_TEXTURE_ANIMATION_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Texture animation manager window class
//+-----------------------------------------------------------------------------
class TEXTURE_ANIMATION_MANAGER_WINDOW : public MANAGER_WINDOW
{
	public:
		CONSTRUCTOR TEXTURE_ANIMATION_MANAGER_WINDOW();
		DESTRUCTOR ~TEXTURE_ANIMATION_MANAGER_WINDOW();

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
extern TEXTURE_ANIMATION_MANAGER_WINDOW TextureAnimationManagerWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

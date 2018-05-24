//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TEXTURE_MANAGER_WINDOW_H
#define MAGOS_TEXTURE_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Texture manager window class
//+-----------------------------------------------------------------------------
class TEXTURE_MANAGER_WINDOW : public MANAGER_WINDOW
{
	public:
		CONSTRUCTOR TEXTURE_MANAGER_WINDOW();
		DESTRUCTOR ~TEXTURE_MANAGER_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

		BOOL UseTextureFromMpq(CONST std::string& FileName);

		VOID HideTextureWindow();

	protected:
		BOOL CreateEmpty();
		BOOL Import();
		BOOL Export();
		BOOL Remove(HWND Window);
		BOOL View();
		BOOL Edit();
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern TEXTURE_MANAGER_WINDOW TextureManagerWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

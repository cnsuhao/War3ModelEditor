//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TEXTURE_WINDOW_H
#define MAGOS_TEXTURE_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| Texture window class
//+-----------------------------------------------------------------------------
class TEXTURE_WINDOW : public WINDOW_FRAME
{
	public:
		CONSTRUCTOR TEXTURE_WINDOW();
		DESTRUCTOR ~TEXTURE_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

		VOID SetFileName(CONST std::string& NewFileName, INT NewReplaceableId = INVALID_INDEX);

		VOID UpdateAndRender();

	protected:
		VOID Render();

		GRAPHICS_WINDOW GraphicsWindow;

		std::string FileName;
		INT ReplaceableId;

		BOOL WindowActive;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern TEXTURE_WINDOW TextureWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_ANIMATION_WINDOW_H
#define MAGOS_ANIMATION_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| Animation window class
//+-----------------------------------------------------------------------------
class ANIMATION_WINDOW : public WINDOW_FRAME
{
	public:
		CONSTRUCTOR ANIMATION_WINDOW();
		DESTRUCTOR ~ANIMATION_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

		VOID AddAnimation(INT Id, CONST std::string& Name);
		VOID RemoveAnimation(INT Id);
		VOID ClearAllAnimations();

		VOID ChangeName(MODEL_SEQUENCE* Sequence, CONST std::string& NewName);

	protected:
		WINDOW_COMBOBOX ComboBox;
		WINDOW_BUTTON PlayButton;
		WINDOW_BUTTON DefaultLoopButton;
		WINDOW_BUTTON AlwaysLoopButton;
		WINDOW_BUTTON NeverLoopButton;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern ANIMATION_WINDOW AnimationWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

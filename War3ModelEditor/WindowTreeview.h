//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_TREEVIEW_H
#define MAGOS_WINDOW_TREEVIEW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Window.h"


//+-----------------------------------------------------------------------------
//| Window treeview info structure
//+-----------------------------------------------------------------------------
struct WINDOW_TREEVIEW_INFO
{
	WINDOW_TREEVIEW_INFO()
	{
		X = 0;
		Y = 0;
		Width = 100;
		Height = 100;

		Parent = NULL;
		Style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	}

	INT X;
	INT Y;
	INT Width;
	INT Height;

	HWND Parent;
	DWORD Style;
};


//+-----------------------------------------------------------------------------
//| Window treeview class
//+-----------------------------------------------------------------------------
class WINDOW_TREEVIEW : public WINDOW
{
	public:
		CONSTRUCTOR WINDOW_TREEVIEW();
		DESTRUCTOR ~WINDOW_TREEVIEW();

		virtual BOOL Create(CONST WINDOW_TREEVIEW_INFO& NewTreeviewInfo);

		VOID Clear();

	protected:
		WINDOW_TREEVIEW_INFO TreeviewInfo;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowTreeview.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_TREEVIEW::WINDOW_TREEVIEW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_TREEVIEW::~WINDOW_TREEVIEW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a treeview window
//+-----------------------------------------------------------------------------
BOOL WINDOW_TREEVIEW::Create(CONST WINDOW_TREEVIEW_INFO& NewTreeviewInfo)
{
	Destroy();

	TreeviewInfo = NewTreeviewInfo;

	Window = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TREEVIEW, "", TreeviewInfo.Style,
							TreeviewInfo.X, TreeviewInfo.Y,
							TreeviewInfo.Width, TreeviewInfo.Height,
							TreeviewInfo.Parent, NULL, GetModuleHandle(NULL), NULL);
	if(Window == NULL)
	{
		Error.SetMessage("Unable to create a treeview window!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Clears the treeview
//+-----------------------------------------------------------------------------
VOID WINDOW_TREEVIEW::Clear()
{
	TreeView_DeleteAllItems(Window);
}

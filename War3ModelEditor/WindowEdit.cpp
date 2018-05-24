//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowEdit.h"
#include "Error.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_EDIT::WINDOW_EDIT()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_EDIT::~WINDOW_EDIT()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates an edit window
//+-----------------------------------------------------------------------------
BOOL WINDOW_EDIT::Create(CONST WINDOW_EDIT_INFO& NewEditInfo)
{
	HFONT Font;

	Destroy();

	EditInfo = NewEditInfo;

	if(EditInfo.HorizontalScroll) EditInfo.Style |= (WS_HSCROLL | ES_AUTOHSCROLL);
	if(EditInfo.VerticalScroll) EditInfo.Style |= (WS_VSCROLL | ES_AUTOVSCROLL);
	if(EditInfo.Multiline) EditInfo.Style |= (ES_MULTILINE | ES_WANTRETURN);
	if(EditInfo.ReadOnly) EditInfo.Style |= ES_READONLY;

	Window = CreateWindowEx(0, "EDIT", EditInfo.Text.c_str(), EditInfo.Style,
							EditInfo.X, EditInfo.Y, EditInfo.Width, EditInfo.Height,
							EditInfo.Parent, NULL, GetModuleHandle(NULL), NULL);
	if(Window == NULL)
	{
		Error.SetMessage("Unable to create an edit window!");
		return FALSE;
	}

	Font = CreateFont(0, 8, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, "Fixedsys");
	if(Font != NULL) ::SendMessage(Window, WM_SETFONT, reinterpret_cast<WPARAM>(Font), 0);

	return TRUE;
}

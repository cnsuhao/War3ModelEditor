//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GlobalSequenceManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
GLOBAL_SEQUENCE_MANAGER_WINDOW GlobalSequenceManagerWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
GLOBAL_SEQUENCE_MANAGER_WINDOW::GLOBAL_SEQUENCE_MANAGER_WINDOW()
{
	ManagerMenuId1 = ManagerMenu;
	ManagerMenuId2 = ManagerMenu2;

	Title = "Global Sequence Manager";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
GLOBAL_SEQUENCE_MANAGER_WINDOW::~GLOBAL_SEQUENCE_MANAGER_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL GLOBAL_SEQUENCE_MANAGER_WINDOW::Create()
{
	if(!MANAGER_WINDOW::Create()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID GLOBAL_SEQUENCE_MANAGER_WINDOW::Destroy()
{
	MANAGER_WINDOW::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT GLOBAL_SEQUENCE_MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	return MANAGER_WINDOW::MessageHandler(Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT GLOBAL_SEQUENCE_MANAGER_WINDOW::MenuHandler(WORD MenuItem)
{
	switch(MenuItem)
	{
		case ManagerFileCreate:
		case ManagerFileCreate2:
		{
			if(!CreateNew())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case ManagerFileRemove:
		case ManagerFileRemove2:
		{
			if(!Remove(Window))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case ManagerFileEdit:
		case ManagerFileEdit2:
		{
			if(!Edit())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}
	}

	return MANAGER_WINDOW::MenuHandler(MenuItem);
}


//+-----------------------------------------------------------------------------
//| Handles the window control messages
//+-----------------------------------------------------------------------------
LRESULT GLOBAL_SEQUENCE_MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	if(Control == ListBox.GetWindow())
	{
		if(Code == LBN_DBLCLK)
		{
			SendMessage(WM_COMMAND, ManagerFileEdit, 0);
			return 0;
		}
	}

	return MANAGER_WINDOW::ControlHandler(Control, Code);
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT GLOBAL_SEQUENCE_MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return MANAGER_WINDOW::NotifyHandler(Control, Code, Header);
}


//+-----------------------------------------------------------------------------
//| Creates a new global sequence
//+-----------------------------------------------------------------------------
BOOL GLOBAL_SEQUENCE_MANAGER_WINDOW::CreateNew()
{
	MODEL_GLOBAL_SEQUENCE_DATA Data;
	MODEL_GLOBAL_SEQUENCE* GlobalSequence;

	if(!GlobalSequenceDialog.Display(Window, Data)) return TRUE;

	GlobalSequence = new MODEL_GLOBAL_SEQUENCE();
	if(GlobalSequence == NULL)
	{
		Error.SetMessage("Unable to create a new global sequence, memory allocation failed!");
		return FALSE;
	}

	GlobalSequence->Data() = Data;

	if(!Model.AddGlobalSequence(GlobalSequence))
	{
		delete GlobalSequence;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing global sequence
//+-----------------------------------------------------------------------------
BOOL GLOBAL_SEQUENCE_MANAGER_WINDOW::Remove(HWND Window)
{
	INT Index;
	MODEL_GLOBAL_SEQUENCE* GlobalSequence;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	GlobalSequence = reinterpret_cast<MODEL_GLOBAL_SEQUENCE*>(ListBox.GetData(Index));
	if(GlobalSequence == NULL) return TRUE;

	if(!Model.RemoveGlobalSequence(GlobalSequence, Window)) return FALSE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing global sequence
//+-----------------------------------------------------------------------------
BOOL GLOBAL_SEQUENCE_MANAGER_WINDOW::Edit()
{
	INT Index;
	MODEL_GLOBAL_SEQUENCE_DATA Data;
	MODEL_GLOBAL_SEQUENCE* GlobalSequence;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	GlobalSequence = reinterpret_cast<MODEL_GLOBAL_SEQUENCE*>(ListBox.GetData(Index));
	if(GlobalSequence == NULL) return TRUE;

	Data = GlobalSequence->Data();

	if(!GlobalSequenceDialog.Display(Window, Data)) return TRUE;

	GlobalSequence->Data() = Data;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}

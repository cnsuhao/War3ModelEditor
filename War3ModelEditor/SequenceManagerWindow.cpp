//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "SequenceManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
SEQUENCE_MANAGER_WINDOW SequenceManagerWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
SEQUENCE_MANAGER_WINDOW::SEQUENCE_MANAGER_WINDOW()
{
	ManagerMenuId1 = ManagerMenu;
	ManagerMenuId2 = ManagerMenu2;

	Title = "Sequence Manager";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
SEQUENCE_MANAGER_WINDOW::~SEQUENCE_MANAGER_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL SEQUENCE_MANAGER_WINDOW::Create()
{
	if(!MANAGER_WINDOW::Create()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID SEQUENCE_MANAGER_WINDOW::Destroy()
{
	MANAGER_WINDOW::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT SEQUENCE_MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	return MANAGER_WINDOW::MessageHandler(Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT SEQUENCE_MANAGER_WINDOW::MenuHandler(WORD MenuItem)
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
			if(!Remove())
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
LRESULT SEQUENCE_MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
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
LRESULT SEQUENCE_MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return MANAGER_WINDOW::NotifyHandler(Control, Code, Header);
}


//+-----------------------------------------------------------------------------
//| Creates a new sequence
//+-----------------------------------------------------------------------------
BOOL SEQUENCE_MANAGER_WINDOW::CreateNew()
{
	MODEL_SEQUENCE_DATA Data;
	MODEL_SEQUENCE* Sequence;

	if(!SequenceDialog.Display(Window, Data)) return TRUE;

	Sequence = new MODEL_SEQUENCE();
	if(Sequence == NULL)
	{
		Error.SetMessage("Unable to create a new sequence, memory allocation failed!");
		return FALSE;
	}

	Sequence->Data() = Data;

	if(!Model.AddSequence(Sequence))
	{
		delete Sequence;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing sequence
//+-----------------------------------------------------------------------------
BOOL SEQUENCE_MANAGER_WINDOW::Remove()
{
	INT Index;
	MODEL_SEQUENCE* Sequence;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Sequence = reinterpret_cast<MODEL_SEQUENCE*>(ListBox.GetData(Index));
	if(Sequence == NULL) return TRUE;

	if(!Model.RemoveSequence(Sequence)) return FALSE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing sequence
//+-----------------------------------------------------------------------------
BOOL SEQUENCE_MANAGER_WINDOW::Edit()
{
	INT Index;
	std::string Name;
	MODEL_SEQUENCE_DATA Data;
	MODEL_SEQUENCE* Sequence;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Sequence = reinterpret_cast<MODEL_SEQUENCE*>(ListBox.GetData(Index));
	if(Sequence == NULL) return TRUE;

	Data = Sequence->Data();
	Name = Data.Name;

	if(!SequenceDialog.Display(Window, Data)) return TRUE;

	if(Data.Name != Name)
	{
		ListBox.RemoveString(Index);
		Index = ListBox.AddString(Data.Name);
		ListBox.SetData(Index, reinterpret_cast<INT>(Sequence));

		AnimationWindow.ChangeName(Sequence, Data.Name);
	}

	Sequence->Data() = Data;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}

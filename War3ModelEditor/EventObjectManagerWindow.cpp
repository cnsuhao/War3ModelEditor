//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "EventObjectManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
EVENT_OBJECT_MANAGER_WINDOW EventObjectManagerWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
EVENT_OBJECT_MANAGER_WINDOW::EVENT_OBJECT_MANAGER_WINDOW()
{
	ManagerMenuId1 = ManagerMenu;
	ManagerMenuId2 = ManagerMenu2;

	Title = "Event Object Manager";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
EVENT_OBJECT_MANAGER_WINDOW::~EVENT_OBJECT_MANAGER_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL EVENT_OBJECT_MANAGER_WINDOW::Create()
{
	if(!MANAGER_WINDOW::Create()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID EVENT_OBJECT_MANAGER_WINDOW::Destroy()
{
	MANAGER_WINDOW::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT EVENT_OBJECT_MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	return MANAGER_WINDOW::MessageHandler(Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT EVENT_OBJECT_MANAGER_WINDOW::MenuHandler(WORD MenuItem)
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
LRESULT EVENT_OBJECT_MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
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
LRESULT EVENT_OBJECT_MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return MANAGER_WINDOW::NotifyHandler(Control, Code, Header);
}


//+-----------------------------------------------------------------------------
//| Creates a new event object
//+-----------------------------------------------------------------------------
BOOL EVENT_OBJECT_MANAGER_WINDOW::CreateNew()
{
	MODEL_EVENT_OBJECT_DATA Data;
	MODEL_EVENT_OBJECT* EventObject;

	//if(!EventObjectDialog.Display(Window, Data)) return TRUE;

	EventObject = new MODEL_EVENT_OBJECT();
	if(EventObject == NULL)
	{
		Error.SetMessage("Unable to create \"" + Data.Name + "\", memory allocation failed!");
		return FALSE;
	}

	EventObject->Data() = Data;

	if(!Model.AddEventObject(EventObject))
	{
		delete EventObject;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing event object
//+-----------------------------------------------------------------------------
BOOL EVENT_OBJECT_MANAGER_WINDOW::Remove()
{
	INT Index;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	//if(!Model.RemoveEventObject(Index)) return FALSE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing event object
//+-----------------------------------------------------------------------------
BOOL EVENT_OBJECT_MANAGER_WINDOW::Edit()
{
	INT Index;
	INT Index2;
	std::string Name;
	MODEL_EVENT_OBJECT_DATA Data;
	MODEL_EVENT_OBJECT* EventObject;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Name = ListBox.GetString(Index);
	//Index2 = Model.Data().EventObjectContainer.GetIndex(Name);
	if(Index2 == INVALID_INDEX)
	{
		Error.SetMessage("Unable to retrieve the event object \"" + Name + "\"!");
		return FALSE;
	}

	EventObject = Model.Data().EventObjectContainer[Index2];
	Data = EventObject->Data();

	//if(!EventObjectDialog.Display(Window, Data)) return TRUE;

	if(Data.Name != Name)
	{
		//if(!Model.Data().EventObjectContainer.Rename(Index, Data.Name))
		{
			Error.SetMessage("Unable to rename \"" + Name + "\" into \"" + Data.Name + "\", an event object with that name already exists!");
			return FALSE;
		}

		ListBox.RemoveString(Index);
		ListBox.AddString(Data.Name);
	}

	EventObject->Data() = Data;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}

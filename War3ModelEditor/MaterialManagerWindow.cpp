//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "MaterialManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MATERIAL_MANAGER_WINDOW MaterialManagerWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MATERIAL_MANAGER_WINDOW::MATERIAL_MANAGER_WINDOW()
{
	ManagerMenuId1 = ManagerMenu;
	ManagerMenuId2 = ManagerMenu2;

	Title = "Material Manager";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MATERIAL_MANAGER_WINDOW::~MATERIAL_MANAGER_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL MATERIAL_MANAGER_WINDOW::Create()
{
	if(!MANAGER_WINDOW::Create()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID MATERIAL_MANAGER_WINDOW::Destroy()
{
	MANAGER_WINDOW::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT MATERIAL_MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	return MANAGER_WINDOW::MessageHandler(Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT MATERIAL_MANAGER_WINDOW::MenuHandler(WORD MenuItem)
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
LRESULT MATERIAL_MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
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
LRESULT MATERIAL_MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return MANAGER_WINDOW::NotifyHandler(Control, Code, Header);
}


//+-----------------------------------------------------------------------------
//| Selects a material
//+-----------------------------------------------------------------------------
VOID MATERIAL_MANAGER_WINDOW::SelectMaterial(MODEL_MATERIAL* Material)
{
	INT Size;
	INT Index;

	Size = ListBox.GetNrOfItems();
	Index = 0;

	while(Index < Size)
	{
		if(reinterpret_cast<MODEL_MATERIAL*>(ListBox.GetData(Index)) == Material)
		{
			ListBox.SetCurrentSelection(Index);
			return;
		}

		Index++;
	}

	ListBox.SetCurrentSelection(-1);
}


//+-----------------------------------------------------------------------------
//| Creates a new material
//+-----------------------------------------------------------------------------
BOOL MATERIAL_MANAGER_WINDOW::CreateNew()
{
	MODEL_MATERIAL_DATA Data;
	MODEL_MATERIAL* Material;

	if(!MaterialDialog.Display(Window, Data)) return TRUE;

	Material = new MODEL_MATERIAL();
	if(Material == NULL)
	{
		Error.SetMessage("Unable to create a new material, memory allocation failed!");
		return FALSE;
	}

	Material->Data() = Data;

	if(!Model.AddMaterial(Material))
	{
		delete Material;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing material
//+-----------------------------------------------------------------------------
BOOL MATERIAL_MANAGER_WINDOW::Remove(HWND Window)
{
	INT Index;
	MODEL_MATERIAL* Material;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Material = reinterpret_cast<MODEL_MATERIAL*>(ListBox.GetData(Index));
	if(Material == NULL) return TRUE;

	if(!Model.RemoveMaterial(Material, Window)) return FALSE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing material
//+-----------------------------------------------------------------------------
BOOL MATERIAL_MANAGER_WINDOW::Edit()
{
	INT Index;
	MODEL_MATERIAL_DATA Data;
	MODEL_MATERIAL* Material;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Material = reinterpret_cast<MODEL_MATERIAL*>(ListBox.GetData(Index));
	if(Material == NULL) return TRUE;

	Data = Material->Data();

	if(!MaterialDialog.Display(Window, Data)) return TRUE;

	Material->Data() = Data;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}

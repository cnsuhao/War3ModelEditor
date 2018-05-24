//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GeosetAnimationManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
GEOSET_ANIMATION_MANAGER_WINDOW GeosetAnimationManagerWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
GEOSET_ANIMATION_MANAGER_WINDOW::GEOSET_ANIMATION_MANAGER_WINDOW()
{
	ManagerMenuId1 = ManagerMenu;
	ManagerMenuId2 = ManagerMenu2;

	Title = "Geoset Animation Manager";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
GEOSET_ANIMATION_MANAGER_WINDOW::~GEOSET_ANIMATION_MANAGER_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL GEOSET_ANIMATION_MANAGER_WINDOW::Create()
{
	if(!MANAGER_WINDOW::Create()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID GEOSET_ANIMATION_MANAGER_WINDOW::Destroy()
{
	MANAGER_WINDOW::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT GEOSET_ANIMATION_MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	return MANAGER_WINDOW::MessageHandler(Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT GEOSET_ANIMATION_MANAGER_WINDOW::MenuHandler(WORD MenuItem)
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
LRESULT GEOSET_ANIMATION_MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
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
LRESULT GEOSET_ANIMATION_MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return MANAGER_WINDOW::NotifyHandler(Control, Code, Header);
}


//+-----------------------------------------------------------------------------
//| Creates a new geoset animation
//+-----------------------------------------------------------------------------
BOOL GEOSET_ANIMATION_MANAGER_WINDOW::CreateNew()
{
	MODEL_GEOSET_ANIMATION_DATA Data;
	MODEL_GEOSET_ANIMATION* GeosetAnimation;
	MODEL_GEOSET* Geoset;

	Geoset = NULL;

	if(!GeosetAnimationDialog.Display(Window, Data, Geoset)) return TRUE;

	GeosetAnimation = new MODEL_GEOSET_ANIMATION();
	if(GeosetAnimation == NULL)
	{
		Error.SetMessage("Unable to create a new geoset animation, memory allocation failed!");
		return FALSE;
	}

	GeosetAnimation->Data() = Data;
	if(Geoset != NULL)
	{
		GeosetAnimation->GeosetNode.Attach(Geoset->GeosetAnimationNodes);
	}
	else
	{
		GeosetAnimation->GeosetNode.Detach();
	}

	if(!Model.AddGeosetAnimation(GeosetAnimation))
	{
		delete GeosetAnimation;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing geoset animation
//+-----------------------------------------------------------------------------
BOOL GEOSET_ANIMATION_MANAGER_WINDOW::Remove(HWND Window)
{
	INT Index;
	MODEL_GEOSET_ANIMATION* GeosetAnimation;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	GeosetAnimation = reinterpret_cast<MODEL_GEOSET_ANIMATION*>(ListBox.GetData(Index));
	if(GeosetAnimation == NULL) return TRUE;

	if(!Model.RemoveGeosetAnimation(GeosetAnimation, Window)) return FALSE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing geoset animation
//+-----------------------------------------------------------------------------
BOOL GEOSET_ANIMATION_MANAGER_WINDOW::Edit()
{
	INT Index;
	MODEL_GEOSET_ANIMATION_DATA Data;
	MODEL_GEOSET_ANIMATION* GeosetAnimation;
	MODEL_GEOSET* Geoset;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	GeosetAnimation = reinterpret_cast<MODEL_GEOSET_ANIMATION*>(ListBox.GetData(Index));
	if(GeosetAnimation == NULL) return TRUE;

	Data = GeosetAnimation->Data();
	Geoset = GeosetAnimation->GeosetNode.IsAttached() ? GeosetAnimation->GeosetNode.GetObjectData() : NULL;

	if(!GeosetAnimationDialog.Display(Window, Data, Geoset)) return TRUE;

	GeosetAnimation->Data() = Data;
	if(Geoset != NULL)
	{
		GeosetAnimation->GeosetNode.Attach(Geoset->GeosetAnimationNodes);
	}
	else
	{
		GeosetAnimation->GeosetNode.Detach();
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}

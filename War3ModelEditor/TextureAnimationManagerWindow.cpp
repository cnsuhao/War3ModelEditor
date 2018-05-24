//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TextureAnimationManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEXTURE_ANIMATION_MANAGER_WINDOW TextureAnimationManagerWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEXTURE_ANIMATION_MANAGER_WINDOW::TEXTURE_ANIMATION_MANAGER_WINDOW()
{
	ManagerMenuId1 = ManagerMenu;
	ManagerMenuId2 = ManagerMenu2;

	Title = "Texture Animation Manager";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEXTURE_ANIMATION_MANAGER_WINDOW::~TEXTURE_ANIMATION_MANAGER_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL TEXTURE_ANIMATION_MANAGER_WINDOW::Create()
{
	if(!MANAGER_WINDOW::Create()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID TEXTURE_ANIMATION_MANAGER_WINDOW::Destroy()
{
	MANAGER_WINDOW::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT TEXTURE_ANIMATION_MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	return MANAGER_WINDOW::MessageHandler(Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT TEXTURE_ANIMATION_MANAGER_WINDOW::MenuHandler(WORD MenuItem)
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
LRESULT TEXTURE_ANIMATION_MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
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
LRESULT TEXTURE_ANIMATION_MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return MANAGER_WINDOW::NotifyHandler(Control, Code, Header);
}


//+-----------------------------------------------------------------------------
//| Creates a new texture animation
//+-----------------------------------------------------------------------------
BOOL TEXTURE_ANIMATION_MANAGER_WINDOW::CreateNew()
{
	MODEL_TEXTURE_ANIMATION_DATA Data;
	MODEL_TEXTURE_ANIMATION* TextureAnimation;

	if(!TextureAnimationDialog.Display(Window, Data)) return TRUE;

	TextureAnimation = new MODEL_TEXTURE_ANIMATION();
	if(TextureAnimation == NULL)
	{
		Error.SetMessage("Unable to create a new texture animation, memory allocation failed!");
		return FALSE;
	}

	TextureAnimation->Data() = Data;

	if(!Model.AddTextureAnimation(TextureAnimation))
	{
		delete TextureAnimation;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing texture animation
//+-----------------------------------------------------------------------------
BOOL TEXTURE_ANIMATION_MANAGER_WINDOW::Remove(HWND Window)
{
	INT Index;
	MODEL_TEXTURE_ANIMATION* TextureAnimation;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	TextureAnimation = reinterpret_cast<MODEL_TEXTURE_ANIMATION*>(ListBox.GetData(Index));
	if(TextureAnimation == NULL) return TRUE;

	if(!Model.RemoveTextureAnimation(TextureAnimation, Window)) return FALSE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing texture animation
//+-----------------------------------------------------------------------------
BOOL TEXTURE_ANIMATION_MANAGER_WINDOW::Edit()
{
	INT Index;
	MODEL_TEXTURE_ANIMATION_DATA Data;
	MODEL_TEXTURE_ANIMATION* TextureAnimation;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	TextureAnimation = reinterpret_cast<MODEL_TEXTURE_ANIMATION*>(ListBox.GetData(Index));
	if(TextureAnimation == NULL) return TRUE;

	Data = TextureAnimation->Data();

	if(!TextureAnimationDialog.Display(Window, Data)) return TRUE;

	TextureAnimation->Data() = Data;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CameraManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
CAMERA_MANAGER_WINDOW CameraManagerWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
CAMERA_MANAGER_WINDOW::CAMERA_MANAGER_WINDOW()
{
	ManagerMenuId1 = CameraMenu;
	ManagerMenuId2 = CameraMenu2;

	Title = "Camera Manager";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
CAMERA_MANAGER_WINDOW::~CAMERA_MANAGER_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL CAMERA_MANAGER_WINDOW::Create()
{
	if(!MANAGER_WINDOW::Create()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID CAMERA_MANAGER_WINDOW::Destroy()
{
	MANAGER_WINDOW::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT CAMERA_MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	return MANAGER_WINDOW::MessageHandler(Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT CAMERA_MANAGER_WINDOW::MenuHandler(WORD MenuItem)
{
	switch(MenuItem)
	{
		case CameraFileCreate:
		case CameraFileCreate2:
		{
			if(!CreateNew())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case CameraFileCreateView:
		case CameraFileCreateView2:
		{
			if(!CreateNewFromView())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case CameraFileRemove:
		case CameraFileRemove2:
		{
			if(!Remove())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case CameraFileEdit:
		case CameraFileEdit2:
		{
			if(!Edit())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case CameraFileView:
		case CameraFileView2:
		{
			if(!View())
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
LRESULT CAMERA_MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	if(Control == ListBox.GetWindow())
	{
		if(Code == LBN_DBLCLK)
		{
			SendMessage(WM_COMMAND, CameraFileEdit, 0);
			return 0;
		}
	}

	return MANAGER_WINDOW::ControlHandler(Control, Code);
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT CAMERA_MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return MANAGER_WINDOW::NotifyHandler(Control, Code, Header);
}


//+-----------------------------------------------------------------------------
//| Creates a new camera
//+-----------------------------------------------------------------------------
BOOL CAMERA_MANAGER_WINDOW::CreateNew()
{
	MODEL_CAMERA_DATA Data;
	MODEL_CAMERA* Camera;

	if(!CameraDialog.Display(Window, Data)) return TRUE;

	Camera = new MODEL_CAMERA();
	if(Camera == NULL)
	{
		Error.SetMessage("Unable to create a new camera, memory allocation failed!");
		return FALSE;
	}

	Camera->Data() = Data;

	if(!Model.AddCamera(Camera))
	{
		delete Camera;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new camera from the current view
//+-----------------------------------------------------------------------------
BOOL CAMERA_MANAGER_WINDOW::CreateNewFromView()
{
	MODEL_CAMERA_DATA Data;
	MODEL_CAMERA* Camera;

	Data.Source = ::Camera.GetSource();
	Data.Target = ::Camera.GetTarget();

	if(!CameraDialog.Display(Window, Data)) return TRUE;

	Camera = new MODEL_CAMERA();
	if(Camera == NULL)
	{
		Error.SetMessage("Unable to create a new camera, memory allocation failed!");
		return FALSE;
	}

	Camera->Data() = Data;

	if(!Model.AddCamera(Camera))
	{
		delete Camera;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing camera
//+-----------------------------------------------------------------------------
BOOL CAMERA_MANAGER_WINDOW::Remove()
{
	INT Index;
	MODEL_CAMERA* Camera;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Camera = reinterpret_cast<MODEL_CAMERA*>(ListBox.GetData(Index));
	if(Camera == NULL) return TRUE;

	if(!Model.RemoveCamera(Camera)) return FALSE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing camera
//+-----------------------------------------------------------------------------
BOOL CAMERA_MANAGER_WINDOW::Edit()
{
	INT Index;
	MODEL_CAMERA_DATA Data;
	MODEL_CAMERA* Camera;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Camera = reinterpret_cast<MODEL_CAMERA*>(ListBox.GetData(Index));
	if(Camera == NULL) return TRUE;

	Data = Camera->Data();

	if(!CameraDialog.Display(Window, Data)) return TRUE;

	Camera->Data() = Data;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Views an existing camera
//+-----------------------------------------------------------------------------
BOOL CAMERA_MANAGER_WINDOW::View()
{
	INT Index;
	MODEL_CAMERA* CameraObject;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	CameraObject = reinterpret_cast<MODEL_CAMERA*>(ListBox.GetData(Index));
	if(CameraObject == NULL) return TRUE;

	Camera.SetPosition(CameraObject->Data().Source, CameraObject->Data().Target);

	return TRUE;
}

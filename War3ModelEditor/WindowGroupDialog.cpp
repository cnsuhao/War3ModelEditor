//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowGroupDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_GROUP_DIALOG GroupDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_GEOSET_GROUP* WINDOW_GROUP_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_GROUP_DIALOG::WINDOW_GROUP_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_GROUP_DIALOG::~WINDOW_GROUP_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_GROUP_DIALOG::Display(HWND ParentWindow, MODEL_GEOSET_GROUP* Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogGroup), ParentWindow, DialogMessageHandler, 0))
	{
		StaticData = NULL;
		return TRUE;
	}

	StaticData = NULL;
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_GROUP_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			BuildList(Window);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogGroupButtonOk:
				{
					ExtractList(Window);

					Model.Rebuild();

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogGroupButtonCancel:
				{
					RemoveWindow(Window);
					EndDialog(Window, 0);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogGroupButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the list
//+-----------------------------------------------------------------------------
VOID WINDOW_GROUP_DIALOG::BuildList(HWND Window)
{
	INT i;
	INT j;
	INT Index;
	std::stringstream Stream;

	for(i = 0; i < StaticData->MatrixList.GetTotalSize(); i++)
	{
		if(StaticData->MatrixList.ValidIndex(i))
		{
			Index = 0;

			for(j = 0; j < Model.Data().BaseContainer.GetTotalSize(); j++)
			{
				if(Model.Data().BaseContainer.ValidIndex(j))
				{
					if(StaticData->MatrixList[i]->Node.IsAttached())
					{
						if(StaticData->MatrixList[i]->Node.GetObjectData() == Model.Data().BaseContainer[j])
						{
							Index = j;
							break;
						}
					}
				}
			}

			Stream << Index << "\r\n";
		}
	}

	SetString(GetDlgItem(Window, DialogGroupEdit), Stream.str());
}


//+-----------------------------------------------------------------------------
//| Extracts the information from the list
//+-----------------------------------------------------------------------------
VOID WINDOW_GROUP_DIALOG::ExtractList(HWND Window)
{
	INT Index;
	std::string String;
	std::stringstream Stream;
	std::stringstream TempStream;
	MODEL_GEOSET_GROUP_NODE* Node;

	SAFE_CLEAR(StaticData->MatrixList);
	Stream.str(GetString(GetDlgItem(Window, DialogGroupEdit)));

	while(!Stream.eof())
	{
		String = "";
		Stream >> String;

		Index = -1;
		TempStream.clear();
		TempStream.str(String);
		TempStream >> Index;

		if(Index < 0) break;

		if(Model.Data().BaseContainer.ValidIndex(Index))
		{
			Node = new MODEL_GEOSET_GROUP_NODE();
			if(Node == NULL) return;

			Node->Node.Attach(Model.Data().BaseContainer[Index]->MatrixListNodes);
			StaticData->MatrixList.Add(Node);
		}
	}
}

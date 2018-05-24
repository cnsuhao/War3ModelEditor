//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowFaceDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_FACE_DIALOG FaceDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_GEOSET_FACE* WINDOW_FACE_DIALOG::StaticData;
MODEL_GEOSET* WINDOW_FACE_DIALOG::StaticGeoset;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_FACE_DIALOG::WINDOW_FACE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_FACE_DIALOG::~WINDOW_FACE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_FACE_DIALOG::Display(HWND ParentWindow, MODEL_GEOSET* Geoset, MODEL_GEOSET_FACE* Data) CONST
{
	StaticData = Data;
	StaticGeoset = Geoset;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogFace), ParentWindow, DialogMessageHandler, 0))
	{
		StaticData = NULL;
		StaticGeoset = NULL;
		return TRUE;
	}

	StaticData = NULL;
	StaticGeoset = NULL;
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_FACE_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			BuildVertexIdList(Window, DialogFaceComboIndex1, StaticData->Index1);
			BuildVertexIdList(Window, DialogFaceComboIndex2, StaticData->Index2);
			BuildVertexIdList(Window, DialogFaceComboIndex3, StaticData->Index3);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogFaceButtonOk:
				{
					StaticData->Index1 = static_cast<INT>(::SendMessage(GetDlgItem(Window, DialogFaceComboIndex1), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogFaceComboIndex1), CB_GETCURSEL, 0, 0), 0));
					StaticData->Index2 = static_cast<INT>(::SendMessage(GetDlgItem(Window, DialogFaceComboIndex2), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogFaceComboIndex2), CB_GETCURSEL, 0, 0), 0));
					StaticData->Index3 = static_cast<INT>(::SendMessage(GetDlgItem(Window, DialogFaceComboIndex3), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogFaceComboIndex3), CB_GETCURSEL, 0, 0), 0));

					Model.Rebuild();

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogFaceButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogFaceButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the vertex ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_FACE_DIALOG::BuildVertexIdList(HWND Window, UINT ComboId, INT SelectedVertexId)
{
	INT i;
	INT Index;
	INT CurrentVertexId;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, ComboId);
	if(ComboBox == NULL) return;

	Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(NAME_NO_ID.c_str())));
	::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
	::SendMessage(ComboBox, CB_SETCURSEL, 0, 0);

	CurrentVertexId = 0;

	for(i = 0; i < StaticGeoset->Data().VertexContainer.GetTotalSize(); i++)
	{
		if(StaticGeoset->Data().VertexContainer.ValidIndex(i))
		{
			std::stringstream Stream;

			Stream << "Vertex " << StaticGeoset->Data().VertexContainer[i];
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Stream.str().c_str())));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, static_cast<LPARAM>(CurrentVertexId));

			if(CurrentVertexId == SelectedVertexId)
			{
				::SendMessage(ComboBox, CB_SETCURSEL, Index, 0);
			}
		}

		CurrentVertexId++;
	}
}

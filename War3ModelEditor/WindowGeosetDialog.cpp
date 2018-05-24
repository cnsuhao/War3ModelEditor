//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowGeosetDialog.h"
#include "WindowVerticesDialog.h"
#include "WindowFacesDialog.h"
#include "WindowGroupsDialog.h"
#include "WindowExtentsDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_GEOSET_DIALOG GeosetDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_GEOSET_DATA WINDOW_GEOSET_DIALOG::StaticData;
MODEL_GEOSET* WINDOW_GEOSET_DIALOG::StaticGeoset;
MODEL_MATERIAL* WINDOW_GEOSET_DIALOG::StaticMaterial;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_GEOSET_DIALOG::WINDOW_GEOSET_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_GEOSET_DIALOG::~WINDOW_GEOSET_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_GEOSET_DIALOG::Display(HWND ParentWindow, MODEL_GEOSET* Geoset, MODEL_GEOSET_DATA& Data, MODEL_MATERIAL*& Material) CONST
{
	StaticData = Data;
	StaticGeoset = Geoset;
	StaticMaterial = Material;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogGeoset), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		Material = StaticMaterial;
		StaticData = MODEL_GEOSET_DATA();
		StaticGeoset = NULL;
		return TRUE;
	}

	StaticData = MODEL_GEOSET_DATA();
	StaticGeoset = NULL;
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_GEOSET_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetInt(GetDlgItem(Window, DialogGeosetEditSelectionGroup), StaticData.SelectionGroup);
			SetFloat(GetDlgItem(Window, DialogGeosetEditBoundsRadius), StaticData.Extent.Radius);
			SetFloat(GetDlgItem(Window, DialogGeosetEditMinX), StaticData.Extent.Min.x);
			SetFloat(GetDlgItem(Window, DialogGeosetEditMinY), StaticData.Extent.Min.y);
			SetFloat(GetDlgItem(Window, DialogGeosetEditMinZ), StaticData.Extent.Min.z);
			SetFloat(GetDlgItem(Window, DialogGeosetEditMaxX), StaticData.Extent.Max.x);
			SetFloat(GetDlgItem(Window, DialogGeosetEditMaxY), StaticData.Extent.Max.y);
			SetFloat(GetDlgItem(Window, DialogGeosetEditMaxZ), StaticData.Extent.Max.z);
			CheckDlgButton(Window, DialogGeosetButtonUnselectable, BoolToCheckState(StaticData.Unselectable));

			BuildMaterialIdList(Window);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogGeosetButtonVertices:
				{
					if(!VerticesDialog.Display(Window, StaticGeoset, &StaticData.VertexContainer)) return TRUE;

					return TRUE;
				}

				case DialogGeosetButtonFaces:
				{
					if(!FacesDialog.Display(Window, StaticGeoset, &StaticData.FaceContainer)) return TRUE;

					return TRUE;
				}

				case DialogGeosetButtonGroups:
				{
					if(!GroupsDialog.Display(Window, &StaticData.GroupContainer)) return TRUE;

					return TRUE;
				}

				case DialogGeosetButtonExtents:
				{
					if(!ExtentsDialog.Display(Window, &StaticData.ExtentContainer)) return TRUE;

					return TRUE;
				}

				case DialogGeosetButtonOk:
				{
					StaticData.SelectionGroup = GetInt(GetDlgItem(Window, DialogGeosetEditSelectionGroup));
					StaticData.Extent.Radius = GetFloat(GetDlgItem(Window, DialogGeosetEditBoundsRadius));
					StaticData.Extent.Min.x = GetFloat(GetDlgItem(Window, DialogGeosetEditMinX));
					StaticData.Extent.Min.y = GetFloat(GetDlgItem(Window, DialogGeosetEditMinY));
					StaticData.Extent.Min.z = GetFloat(GetDlgItem(Window, DialogGeosetEditMinZ));
					StaticData.Extent.Max.x = GetFloat(GetDlgItem(Window, DialogGeosetEditMaxX));
					StaticData.Extent.Max.y = GetFloat(GetDlgItem(Window, DialogGeosetEditMaxY));
					StaticData.Extent.Max.z = GetFloat(GetDlgItem(Window, DialogGeosetEditMaxZ));
					StaticData.Unselectable = CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetButtonUnselectable));

					StaticMaterial = reinterpret_cast<MODEL_MATERIAL*>(::SendMessage(GetDlgItem(Window, DialogGeosetComboMaterialId), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogGeosetComboMaterialId), CB_GETCURSEL, 0, 0), 0));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogGeosetButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogGeosetButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the material ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_GEOSET_DIALOG::BuildMaterialIdList(HWND Window)
{
	INT i;
	INT Index;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogGeosetComboMaterialId);
	if(ComboBox == NULL) return;

	Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(NAME_NO_ID.c_str())));
	::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
	::SendMessage(ComboBox, CB_SETCURSEL, 0, 0);

	for(i = 0; i < Model.Data().MaterialContainer.GetTotalSize(); i++)
	{
		if(Model.Data().MaterialContainer.ValidIndex(i))
		{
			std::stringstream Stream;

			Stream << "Material " << Model.Data().MaterialContainer[i];
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Stream.str().c_str())));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Model.Data().MaterialContainer[i]));

			if(Model.Data().MaterialContainer[i] == StaticMaterial)
			{
				::SendMessage(ComboBox, CB_SETCURSEL, Index, 0);
			}
		}
	}
}

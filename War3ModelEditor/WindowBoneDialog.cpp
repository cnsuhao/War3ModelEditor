//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowBoneDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_BONE_DIALOG BoneDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_BONE_DATA WINDOW_BONE_DIALOG::StaticData;
MODEL_GEOSET* WINDOW_BONE_DIALOG::StaticGeoset;
MODEL_GEOSET_ANIMATION* WINDOW_BONE_DIALOG::StaticGeosetAnimation;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_BONE_DIALOG::WINDOW_BONE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_BONE_DIALOG::~WINDOW_BONE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_BONE_DIALOG::Display(HWND ParentWindow, MODEL_BONE_DATA& Data, MODEL_GEOSET*& Geoset, MODEL_GEOSET_ANIMATION*& GeosetAnimation) CONST
{
	StaticData = Data;
	StaticGeoset = Geoset;
	StaticGeosetAnimation = GeosetAnimation;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogBone), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		Geoset = StaticGeoset;
		GeosetAnimation = StaticGeosetAnimation;
		StaticData = MODEL_BONE_DATA();
		return TRUE;
	}

	StaticData = MODEL_BONE_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_BONE_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			BuildGeosetIdList(Window);
			BuildGeosetAnimationIdList(Window);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogBoneButtonOk:
				{
					StaticGeoset = reinterpret_cast<MODEL_GEOSET*>(::SendMessage(GetDlgItem(Window, DialogBoneComboGeosetId), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogBoneComboGeosetId), CB_GETCURSEL, 0, 0), 0));
					StaticGeosetAnimation = reinterpret_cast<MODEL_GEOSET_ANIMATION*>(::SendMessage(GetDlgItem(Window, DialogBoneComboGeosetAnimationId), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogBoneComboGeosetAnimationId), CB_GETCURSEL, 0, 0), 0));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogBoneButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogBoneButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the geoset ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_BONE_DIALOG::BuildGeosetIdList(HWND Window)
{
	INT i;
	INT Index;
	INT IndexToSelect;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogBoneComboGeosetId);
	if(ComboBox == NULL) return;

	Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(NAME_MULTIPLE_ID.c_str())));
	::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);

	IndexToSelect = 0;

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			std::stringstream Stream;

			Stream << "Geoset " << Model.Data().GeosetContainer[i];
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Stream.str().c_str())));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Model.Data().GeosetContainer[i]));

			if(Model.Data().GeosetContainer[i] == StaticGeoset)
			{
				IndexToSelect = Index;
			}
		}
	}

	::SendMessage(ComboBox, CB_SETCURSEL, IndexToSelect, 0);
}


//+-----------------------------------------------------------------------------
//| Builds the geoset animationID list
//+-----------------------------------------------------------------------------
VOID WINDOW_BONE_DIALOG::BuildGeosetAnimationIdList(HWND Window)
{
	INT i;
	INT Index;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogBoneComboGeosetAnimationId);
	if(ComboBox == NULL) return;

	Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(NAME_NO_ID.c_str())));
	::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
	::SendMessage(ComboBox, CB_SETCURSEL, 0, 0);

	for(i = 0; i < Model.Data().GeosetAnimationContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetAnimationContainer.ValidIndex(i))
		{
			std::stringstream Stream;

			Stream << "GeosetAnimation " << Model.Data().GeosetAnimationContainer[i];
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Stream.str().c_str())));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Model.Data().GeosetAnimationContainer[i]));

			if(Model.Data().GeosetAnimationContainer[i] == StaticGeosetAnimation)
			{
				::SendMessage(ComboBox, CB_SETCURSEL, Index, 0);
			}
		}
	}
}

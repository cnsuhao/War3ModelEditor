//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowGeosetAnimationDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_GEOSET_ANIMATION_DIALOG GeosetAnimationDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_GEOSET_ANIMATION_DATA WINDOW_GEOSET_ANIMATION_DIALOG::StaticData;
MODEL_GEOSET* WINDOW_GEOSET_ANIMATION_DIALOG::StaticGeoset;
D3DCOLOR WINDOW_GEOSET_ANIMATION_DIALOG::StaticColor;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_GEOSET_ANIMATION_DIALOG::WINDOW_GEOSET_ANIMATION_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_GEOSET_ANIMATION_DIALOG::~WINDOW_GEOSET_ANIMATION_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_GEOSET_ANIMATION_DIALOG::Display(HWND ParentWindow, MODEL_GEOSET_ANIMATION_DATA& Data, MODEL_GEOSET*& Geoset) CONST
{
	StaticData = Data;
	StaticGeoset = Geoset;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogGeosetAnimation), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		Geoset = StaticGeoset;
		StaticData = MODEL_GEOSET_ANIMATION_DATA();
		return TRUE;
	}

	StaticData = MODEL_GEOSET_ANIMATION_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_GEOSET_ANIMATION_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			StaticColor = Common.Vector3ToColor(StaticData.Color.GetVector3());
			SetFloat(GetDlgItem(Window, DialogGeosetAnimationEditAlpha), StaticData.Alpha.GetScalar());
			CheckDlgButton(Window, DialogGeosetAnimationButtonUseColor, BoolToCheckState(StaticData.UseColor));
			CheckDlgButton(Window, DialogGeosetAnimationButtonDropShadow, BoolToCheckState(StaticData.DropShadow));

			CheckDlgButton(Window, DialogGeosetAnimationButtonAnimatedColor, BoolToCheckState(!StaticData.Color.IsStatic()));
			CheckDlgButton(Window, DialogGeosetAnimationButtonAnimatedAlpha, BoolToCheckState(!StaticData.Alpha.IsStatic()));

			EnableWindow(GetDlgItem(Window, DialogGeosetAnimationButtonColor), CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedColor)));
			EnableWindow(GetDlgItem(Window, DialogGeosetAnimationButtonPick), !CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedColor)));
			EnableWindow(GetDlgItem(Window, DialogGeosetAnimationButtonAlpha), CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedAlpha)));
			EnableWindow(GetDlgItem(Window, DialogGeosetAnimationEditAlpha), !CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedAlpha)));

			BuildGeosetIdList(Window);

			return TRUE;
		}

		case WM_PAINT:
		{
			D3DCOLOR Color;

			Color = CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedColor)) ? PROPERTIES_NO_BACKGROUND : StaticColor;
			RenderColorBox(Window, GetDlgItem(Window, DialogGeosetAnimationColor), Color);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogGeosetAnimationButtonPick:
				{
					if(!SelectColor(Window, StaticColor))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogGeosetAnimationButtonColor:
				{
					InterpolatorDialog.Display(Window, StaticData.Color, "Color");
					return TRUE;
				}

				case DialogGeosetAnimationButtonAlpha:
				{
					InterpolatorDialog.Display(Window, StaticData.Alpha, "Alpha");
					return TRUE;
				}

				case DialogGeosetAnimationButtonAnimatedColor:
				{
					EnableWindow(GetDlgItem(Window, DialogGeosetAnimationButtonColor), CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedColor)));
					EnableWindow(GetDlgItem(Window, DialogGeosetAnimationButtonPick), !CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedColor)));
					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogGeosetAnimationButtonAnimatedAlpha:
				{
					EnableWindow(GetDlgItem(Window, DialogGeosetAnimationButtonAlpha), CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedAlpha)));
					EnableWindow(GetDlgItem(Window, DialogGeosetAnimationEditAlpha), !CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedAlpha)));
					return TRUE;
				}

				case DialogGeosetAnimationButtonOk:
				{
					StaticData.UseColor = CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonUseColor));
					StaticData.DropShadow = CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonDropShadow));

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedColor)))
					{
						StaticData.Color.SetStaticVector3(Common.ColorToVector3(StaticColor));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedAlpha)))
					{
						StaticData.Alpha.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogGeosetAnimationEditAlpha)));
					}

					StaticGeoset = reinterpret_cast<MODEL_GEOSET*>(::SendMessage(GetDlgItem(Window, DialogGeosetAnimationComboGeosetId), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogGeosetAnimationComboGeosetId), CB_GETCURSEL, 0, 0), 0));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogGeosetAnimationButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogGeosetAnimationButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the geoset ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_GEOSET_ANIMATION_DIALOG::BuildGeosetIdList(HWND Window)
{
	INT i;
	INT Index;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogGeosetAnimationComboGeosetId);
	if(ComboBox == NULL) return;

	Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(NAME_NO_ID.c_str())));
	::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
	::SendMessage(ComboBox, CB_SETCURSEL, 0, 0);

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
				::SendMessage(ComboBox, CB_SETCURSEL, Index, 0);
			}
		}
	}
}

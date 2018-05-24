//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowLightDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_LIGHT_DIALOG LightDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_LIGHT_DATA WINDOW_LIGHT_DIALOG::StaticData;
D3DCOLOR WINDOW_LIGHT_DIALOG::StaticColor;
D3DCOLOR WINDOW_LIGHT_DIALOG::StaticAmbientColor;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_LIGHT_DIALOG::WINDOW_LIGHT_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_LIGHT_DIALOG::~WINDOW_LIGHT_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_LIGHT_DIALOG::Display(HWND ParentWindow, MODEL_LIGHT_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogLight), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_LIGHT_DATA();
		return TRUE;
	}

	StaticData = MODEL_LIGHT_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_LIGHT_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT Index;
			HWND ComboBox;

			AddWindow(Window);
			CenterWindow(Window);

			ComboBox = GetDlgItem(Window, DialogLightComboType);

			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Omnidirectional")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Directional")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, 1);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Ambient")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, 2);

			if(StaticData.Type < 0) StaticData.Type = 0;
			if(StaticData.Type > 2) StaticData.Type = 2;
			::SendMessage(ComboBox, CB_SETCURSEL, StaticData.Type, 0);

			StaticColor = Common.Vector3ToColor(StaticData.Color.GetVector3());
			StaticAmbientColor = Common.Vector3ToColor(StaticData.AmbientColor.GetVector3());

			SetFloat(GetDlgItem(Window, DialogLightEditIntensity), StaticData.Intensity.GetScalar());
			SetFloat(GetDlgItem(Window, DialogLightEditAmbientIntensity), StaticData.AmbientIntensity.GetScalar());
			SetFloat(GetDlgItem(Window, DialogLightEditAttenuationStart), StaticData.AttenuationStart.GetScalar());
			SetFloat(GetDlgItem(Window, DialogLightEditAttenuationEnd), StaticData.AttenuationEnd.GetScalar());
			SetFloat(GetDlgItem(Window, DialogLightEditVisibility), StaticData.Visibility.GetScalar());

			CheckDlgButton(Window, DialogLightButtonAnimatedColor, BoolToCheckState(!StaticData.Color.IsStatic()));
			CheckDlgButton(Window, DialogLightButtonAnimatedIntensity, BoolToCheckState(!StaticData.Intensity.IsStatic()));
			CheckDlgButton(Window, DialogLightButtonAnimatedAmbientColor, BoolToCheckState(!StaticData.AmbientColor.IsStatic()));
			CheckDlgButton(Window, DialogLightButtonAnimatedAmbientIntensity, BoolToCheckState(!StaticData.AmbientIntensity.IsStatic()));
			CheckDlgButton(Window, DialogLightButtonAnimatedAttenuationStart, BoolToCheckState(!StaticData.AttenuationStart.IsStatic()));
			CheckDlgButton(Window, DialogLightButtonAnimatedAttenuationEnd, BoolToCheckState(!StaticData.AttenuationEnd.IsStatic()));
			CheckDlgButton(Window, DialogLightButtonAnimatedVisibility, BoolToCheckState(!StaticData.Visibility.IsStatic()));

			EnableWindow(GetDlgItem(Window, DialogLightButtonColor), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedColor)));
			EnableWindow(GetDlgItem(Window, DialogLightButtonPickColor), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedColor)));
			EnableWindow(GetDlgItem(Window, DialogLightButtonIntensity), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedIntensity)));
			EnableWindow(GetDlgItem(Window, DialogLightEditIntensity), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedIntensity)));
			EnableWindow(GetDlgItem(Window, DialogLightButtonAmbientColor), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientColor)));
			EnableWindow(GetDlgItem(Window, DialogLightButtonPickAmbientColor), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientColor)));
			EnableWindow(GetDlgItem(Window, DialogLightButtonAmbientIntensity), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientIntensity)));
			EnableWindow(GetDlgItem(Window, DialogLightEditAmbientIntensity), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientIntensity)));
			EnableWindow(GetDlgItem(Window, DialogLightButtonAttenuationStart), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAttenuationStart)));
			EnableWindow(GetDlgItem(Window, DialogLightEditAttenuationStart), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAttenuationStart)));
			EnableWindow(GetDlgItem(Window, DialogLightButtonAttenuationEnd), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAttenuationEnd)));
			EnableWindow(GetDlgItem(Window, DialogLightEditAttenuationEnd), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAttenuationEnd)));
			EnableWindow(GetDlgItem(Window, DialogLightButtonVisibility), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedVisibility)));
			EnableWindow(GetDlgItem(Window, DialogLightEditVisibility), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedVisibility)));

			return TRUE;
		}

		case WM_PAINT:
		{
			D3DCOLOR Color;

			Color = CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedColor)) ? PROPERTIES_NO_BACKGROUND : StaticColor;
			RenderColorBox(Window, GetDlgItem(Window, DialogLightColor), Color);

			Color = CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientColor)) ? PROPERTIES_NO_BACKGROUND : StaticAmbientColor;
			RenderColorBox(Window, GetDlgItem(Window, DialogLightAmbientColor), Color);

			EnableWindow(GetDlgItem(Window, DialogGeosetAnimationButtonColor), CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedColor)));
			EnableWindow(GetDlgItem(Window, DialogGeosetAnimationButtonPick), !CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedColor)));
			EnableWindow(GetDlgItem(Window, DialogGeosetAnimationButtonAlpha), CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedAlpha)));
			EnableWindow(GetDlgItem(Window, DialogGeosetAnimationEditAlpha), !CheckStateToBool(IsDlgButtonChecked(Window, DialogGeosetAnimationButtonAnimatedAlpha)));

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogLightButtonPickColor:
				{
					if(!SelectColor(Window, StaticColor))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogLightButtonPickAmbientColor:
				{
					if(!SelectColor(Window, StaticAmbientColor))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogLightButtonColor:
				{
					InterpolatorDialog.Display(Window, StaticData.Color, "Color");
					return TRUE;
				}

				case DialogLightButtonIntensity:
				{
					InterpolatorDialog.Display(Window, StaticData.Intensity, "Intensity");
					return TRUE;
				}

				case DialogLightButtonAmbientColor:
				{
					InterpolatorDialog.Display(Window, StaticData.AmbientColor, "Ambient Color");
					return TRUE;
				}

				case DialogLightButtonAmbientIntensity:
				{
					InterpolatorDialog.Display(Window, StaticData.AmbientIntensity, "Ambient Intensity");
					return TRUE;
				}

				case DialogLightButtonAttenuationStart:
				{
					InterpolatorDialog.Display(Window, StaticData.AttenuationStart, "Attenuation Start");
					return TRUE;
				}

				case DialogLightButtonAttenuationEnd:
				{
					InterpolatorDialog.Display(Window, StaticData.AttenuationEnd, "Attenuation End");
					return TRUE;
				}

				case DialogLightButtonVisibility:
				{
					InterpolatorDialog.Display(Window, StaticData.Visibility, "Visibility");
					return TRUE;
				}

				case DialogLightButtonAnimatedColor:
				{
					EnableWindow(GetDlgItem(Window, DialogLightButtonColor), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedColor)));
					EnableWindow(GetDlgItem(Window, DialogLightButtonPickColor), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedColor)));
					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogLightButtonAnimatedIntensity:
				{
					EnableWindow(GetDlgItem(Window, DialogLightButtonIntensity), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedIntensity)));
					EnableWindow(GetDlgItem(Window, DialogLightEditIntensity), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedIntensity)));
					return TRUE;
				}

				case DialogLightButtonAnimatedAmbientColor:
				{
					EnableWindow(GetDlgItem(Window, DialogLightButtonAmbientColor), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientColor)));
					EnableWindow(GetDlgItem(Window, DialogLightButtonPickAmbientColor), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientColor)));
					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogLightButtonAnimatedAmbientIntensity:
				{
					EnableWindow(GetDlgItem(Window, DialogLightButtonAmbientIntensity), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientIntensity)));
					EnableWindow(GetDlgItem(Window, DialogLightEditAmbientIntensity), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientIntensity)));
					return TRUE;
				}

				case DialogLightButtonAnimatedAttenuationStart:
				{
					EnableWindow(GetDlgItem(Window, DialogLightButtonAttenuationStart), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAttenuationStart)));
					EnableWindow(GetDlgItem(Window, DialogLightEditAttenuationStart), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAttenuationStart)));
					return TRUE;
				}

				case DialogLightButtonAnimatedAttenuationEnd:
				{
					EnableWindow(GetDlgItem(Window, DialogLightButtonAttenuationEnd), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAttenuationEnd)));
					EnableWindow(GetDlgItem(Window, DialogLightEditAttenuationEnd), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAttenuationEnd)));
					return TRUE;
				}

				case DialogLightButtonAnimatedVisibility:
				{
					EnableWindow(GetDlgItem(Window, DialogLightButtonVisibility), CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedVisibility)));
					EnableWindow(GetDlgItem(Window, DialogLightEditVisibility), !CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedVisibility)));
					return TRUE;
				}

				case DialogLightButtonOk:
				{
					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedColor)))
					{
						StaticData.Color.SetStaticVector3(Common.ColorToVector3(StaticColor));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedIntensity)))
					{
						StaticData.Intensity.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogLightEditIntensity)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientColor)))
					{
						StaticData.AmbientColor.SetStaticVector3(Common.ColorToVector3(StaticAmbientColor));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAmbientIntensity)))
					{
						StaticData.AmbientIntensity.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogLightEditAmbientIntensity)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAttenuationStart)))
					{
						StaticData.AttenuationStart.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogLightEditAttenuationStart)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedAttenuationEnd)))
					{
						StaticData.AttenuationEnd.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogLightEditAttenuationEnd)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogLightButtonAnimatedVisibility)))
					{
						StaticData.Visibility.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogLightEditVisibility)));
					}

					StaticData.Type = static_cast<INT>(::SendMessage(GetDlgItem(Window, DialogLightComboType), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogLightComboType), CB_GETCURSEL, 0, 0), 0));
					if(StaticData.Type < 0) StaticData.Type = 0;
					if(StaticData.Type > 2) StaticData.Type = 2;

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogLightButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogLightButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

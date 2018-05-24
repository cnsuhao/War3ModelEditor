//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowRibbonEmitterDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_RIBBON_EMITTER_DIALOG RibbonEmitterDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_RIBBON_EMITTER_DATA WINDOW_RIBBON_EMITTER_DIALOG::StaticData;
MODEL_MATERIAL* WINDOW_RIBBON_EMITTER_DIALOG::StaticMaterial;
D3DCOLOR WINDOW_RIBBON_EMITTER_DIALOG::StaticColor;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_RIBBON_EMITTER_DIALOG::WINDOW_RIBBON_EMITTER_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_RIBBON_EMITTER_DIALOG::~WINDOW_RIBBON_EMITTER_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_RIBBON_EMITTER_DIALOG::Display(HWND ParentWindow, MODEL_RIBBON_EMITTER_DATA& Data, MODEL_MATERIAL*& Material) CONST
{
	StaticData = Data;
	StaticMaterial = Material;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogRibbonEmitter), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		Material = StaticMaterial;
		StaticData = MODEL_RIBBON_EMITTER_DATA();
		return TRUE;
	}

	StaticData = MODEL_RIBBON_EMITTER_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_RIBBON_EMITTER_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			StaticColor = Common.Vector3ToColor(StaticData.Color.GetVector3());

			SetInt(GetDlgItem(Window, DialogRibbonEmitterEditRows), StaticData.Rows);
			SetInt(GetDlgItem(Window, DialogRibbonEmitterEditColumns), StaticData.Columns);
			SetFloat(GetDlgItem(Window, DialogRibbonEmitterEditEmissionRate), StaticData.EmissionRate);
			SetFloat(GetDlgItem(Window, DialogRibbonEmitterEditLifeSpan), StaticData.LifeSpan);
			SetFloat(GetDlgItem(Window, DialogRibbonEmitterEditGravity), StaticData.Gravity);

			SetFloat(GetDlgItem(Window, DialogRibbonEmitterEditAlpha), StaticData.Alpha.GetScalar());
			SetFloat(GetDlgItem(Window, DialogRibbonEmitterEditVisibility), StaticData.Visibility.GetScalar());
			SetFloat(GetDlgItem(Window, DialogRibbonEmitterEditHeightAbove), StaticData.HeightAbove.GetScalar());
			SetFloat(GetDlgItem(Window, DialogRibbonEmitterEditHeightBelow), StaticData.HeightBelow.GetScalar());
			SetFloat(GetDlgItem(Window, DialogRibbonEmitterEditTextureSlot), StaticData.TextureSlot.GetScalar());

			CheckDlgButton(Window, DialogRibbonEmitterButtonAnimatedColor, BoolToCheckState(!StaticData.Color.IsStatic()));
			CheckDlgButton(Window, DialogRibbonEmitterButtonAnimatedAlpha, BoolToCheckState(!StaticData.Alpha.IsStatic()));
			CheckDlgButton(Window, DialogRibbonEmitterButtonAnimatedVisibility, BoolToCheckState(!StaticData.Visibility.IsStatic()));
			CheckDlgButton(Window, DialogRibbonEmitterButtonAnimatedHeightAbove, BoolToCheckState(!StaticData.HeightAbove.IsStatic()));
			CheckDlgButton(Window, DialogRibbonEmitterButtonAnimatedHeightBelow, BoolToCheckState(!StaticData.HeightBelow.IsStatic()));
			CheckDlgButton(Window, DialogRibbonEmitterButtonAnimatedTextureSlot, BoolToCheckState(!StaticData.TextureSlot.IsStatic()));

			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonColor), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedColor)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonPickColor), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedColor)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonAlpha), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedAlpha)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterEditAlpha), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedAlpha)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonVisibility), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedVisibility)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterEditVisibility), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedVisibility)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonHeightAbove), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedHeightAbove)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterEditHeightAbove), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedHeightAbove)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonHeightBelow), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedHeightBelow)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterEditHeightBelow), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedHeightBelow)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonTextureSlot), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedTextureSlot)));
			EnableWindow(GetDlgItem(Window, DialogRibbonEmitterEditTextureSlot), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedTextureSlot)));

			BuildMaterialIdList(Window);

			return TRUE;
		}

		case WM_PAINT:
		{
			D3DCOLOR Color;

			Color = CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedColor)) ? PROPERTIES_NO_BACKGROUND : StaticColor;
			RenderColorBox(Window, GetDlgItem(Window, DialogRibbonEmitterColor), Color);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogRibbonEmitterButtonPickColor:
				{
					if(!SelectColor(Window, StaticColor))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogRibbonEmitterButtonColor:
				{
					InterpolatorDialog.Display(Window, StaticData.Color, "Color");
					return TRUE;
				}

				case DialogRibbonEmitterButtonAlpha:
				{
					InterpolatorDialog.Display(Window, StaticData.Alpha, "Alpha");
					return TRUE;
				}

				case DialogRibbonEmitterButtonVisibility:
				{
					InterpolatorDialog.Display(Window, StaticData.Visibility, "Visibility");
					return TRUE;
				}

				case DialogRibbonEmitterButtonHeightAbove:
				{
					InterpolatorDialog.Display(Window, StaticData.HeightAbove, "Height Above");
					return TRUE;
				}

				case DialogRibbonEmitterButtonHeightBelow:
				{
					InterpolatorDialog.Display(Window, StaticData.HeightBelow, "Height Below");
					return TRUE;
				}

				case DialogRibbonEmitterButtonTextureSlot:
				{
					InterpolatorDialog.Display(Window, StaticData.TextureSlot, "Texture Slot");
					return TRUE;
				}

				case DialogRibbonEmitterButtonAnimatedColor:
				{
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonColor), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedColor)));
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonPickColor), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedColor)));
					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogRibbonEmitterButtonAnimatedAlpha:
				{
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonAlpha), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedAlpha)));
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterEditAlpha), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedAlpha)));
					return TRUE;
				}

				case DialogRibbonEmitterButtonAnimatedVisibility:
				{
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonVisibility), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedVisibility)));
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterEditVisibility), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedVisibility)));
					return TRUE;
				}

				case DialogRibbonEmitterButtonAnimatedHeightAbove:
				{
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonHeightAbove), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedHeightAbove)));
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterEditHeightAbove), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedHeightAbove)));
					return TRUE;
				}

				case DialogRibbonEmitterButtonAnimatedHeightBelow:
				{
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonHeightBelow), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedHeightBelow)));
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterEditHeightBelow), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedHeightBelow)));
					return TRUE;
				}

				case DialogRibbonEmitterButtonAnimatedTextureSlot:
				{
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterButtonTextureSlot), CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedTextureSlot)));
					EnableWindow(GetDlgItem(Window, DialogRibbonEmitterEditTextureSlot), !CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedTextureSlot)));
					return TRUE;
				}

				case DialogRibbonEmitterButtonOk:
				{
					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedColor)))
					{
						StaticData.Color.SetStaticVector3(Common.ColorToVector3(StaticColor));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedAlpha)))
					{
						StaticData.Alpha.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogRibbonEmitterEditAlpha)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedVisibility)))
					{
						StaticData.Visibility.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogRibbonEmitterEditVisibility)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedHeightAbove)))
					{
						StaticData.HeightAbove.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogRibbonEmitterEditHeightAbove)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedHeightBelow)))
					{
						StaticData.HeightBelow.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogRibbonEmitterEditHeightBelow)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogRibbonEmitterButtonAnimatedTextureSlot)))
					{
						StaticData.TextureSlot.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogRibbonEmitterEditTextureSlot)));
					}

					StaticData.Rows = GetInt(GetDlgItem(Window, DialogRibbonEmitterEditRows));
					StaticData.Columns = GetInt(GetDlgItem(Window, DialogRibbonEmitterEditColumns));
					StaticData.EmissionRate = GetFloat(GetDlgItem(Window, DialogRibbonEmitterEditEmissionRate));
					StaticData.LifeSpan = GetFloat(GetDlgItem(Window, DialogRibbonEmitterEditLifeSpan));
					StaticData.Gravity = GetFloat(GetDlgItem(Window, DialogRibbonEmitterEditGravity));

					StaticMaterial = reinterpret_cast<MODEL_MATERIAL*>(::SendMessage(GetDlgItem(Window, DialogRibbonEmitterComboMaterialId), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogRibbonEmitterComboMaterialId), CB_GETCURSEL, 0, 0), 0));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogRibbonEmitterButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogRibbonEmitterButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the material ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_RIBBON_EMITTER_DIALOG::BuildMaterialIdList(HWND Window)
{
	INT i;
	INT Index;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogRibbonEmitterComboMaterialId);
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

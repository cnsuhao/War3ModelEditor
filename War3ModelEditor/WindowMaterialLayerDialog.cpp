//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowMaterialLayerDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_MATERIAL_LAYER_DIALOG MaterialLayerDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_LAYER_DATA WINDOW_MATERIAL_LAYER_DIALOG::StaticData;
MODEL_TEXTURE* WINDOW_MATERIAL_LAYER_DIALOG::StaticTexture;
MODEL_TEXTURE_ANIMATION* WINDOW_MATERIAL_LAYER_DIALOG::StaticTextureAnimation;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_MATERIAL_LAYER_DIALOG::WINDOW_MATERIAL_LAYER_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_MATERIAL_LAYER_DIALOG::~WINDOW_MATERIAL_LAYER_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_MATERIAL_LAYER_DIALOG::Display(HWND ParentWindow, MODEL_MATERIAL_LAYER_DATA& Data, MODEL_TEXTURE*& Texture, MODEL_TEXTURE_ANIMATION*& TextureAnimation) CONST
{
	StaticData = Data;
	StaticTexture = Texture;
	StaticTextureAnimation = TextureAnimation;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogMaterialLayer), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		Texture = StaticTexture;
		TextureAnimation = StaticTextureAnimation;
		StaticData = MODEL_MATERIAL_LAYER_DATA();
		return TRUE;
	}

	StaticData = MODEL_MATERIAL_LAYER_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_MATERIAL_LAYER_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT Index;
			HWND ComboBox;

			AddWindow(Window);
			CenterWindow(Window);

			BuildTextureIdList(Window);
			BuildTextureAnimationIdList(Window);

			ComboBox = GetDlgItem(Window, DialogMaterialLayerComboFilterMode);

			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("None")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, FILTER_MODE_NONE);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Transparent")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, FILTER_MODE_TRANSPARENT);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Blend")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, FILTER_MODE_BLEND);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Additive")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, FILTER_MODE_ADDITIVE);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Add Alpha")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, FILTER_MODE_ADD_ALPHA);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Modulate")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, FILTER_MODE_MODULATE);

			::SendMessage(ComboBox, CB_SETCURSEL, StaticData.FilterMode, 0);

			SetFloat(GetDlgItem(Window, DialogMaterialLayerEditAlpha), StaticData.Alpha.GetScalar());

			CheckDlgButton(Window, DialogMaterialLayerButtonUnshaded, BoolToCheckState(StaticData.Unshaded));
			CheckDlgButton(Window, DialogMaterialLayerButtonUnfogged, BoolToCheckState(StaticData.Unfogged));
			CheckDlgButton(Window, DialogMaterialLayerButtonTwoSided, BoolToCheckState(StaticData.TwoSided));
			CheckDlgButton(Window, DialogMaterialLayerButtonSphereEnvironmentMap, BoolToCheckState(StaticData.SphereEnvironmentMap));
			CheckDlgButton(Window, DialogMaterialLayerButtonNoDepthTest, BoolToCheckState(StaticData.NoDepthTest));
			CheckDlgButton(Window, DialogMaterialLayerButtonNoDepthSet, BoolToCheckState(StaticData.NoDepthSet));

			CheckDlgButton(Window, DialogMaterialLayerButtonAnimatedAlpha, BoolToCheckState(!StaticData.Alpha.IsStatic()));
			CheckDlgButton(Window, DialogMaterialLayerButtonAnimatedTextureId, BoolToCheckState(!StaticData.AnimatedTextureId.IsStatic()));

			EnableWindow(GetDlgItem(Window, DialogMaterialLayerButtonAlpha), CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonAnimatedAlpha)));
			EnableWindow(GetDlgItem(Window, DialogMaterialLayerEditAlpha), !CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonAnimatedAlpha)));
			EnableWindow(GetDlgItem(Window, DialogMaterialLayerButtonTextureId), CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonAnimatedTextureId)));
			EnableWindow(GetDlgItem(Window, DialogMaterialLayerComboTextureId), !CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonAnimatedTextureId)));

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogMaterialLayerButtonAlpha:
				{
					InterpolatorDialog.Display(Window, StaticData.Alpha, "Alpha");
					return TRUE;
				}

				case DialogMaterialLayerButtonTextureId:
				{
					InterpolatorDialog.Display(Window, StaticData.AnimatedTextureId, "Texture ID");
					return TRUE;
				}

				case DialogMaterialLayerButtonAnimatedAlpha:
				{
					EnableWindow(GetDlgItem(Window, DialogMaterialLayerButtonAlpha), CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonAnimatedAlpha)));
					EnableWindow(GetDlgItem(Window, DialogMaterialLayerEditAlpha), !CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonAnimatedAlpha)));
					return TRUE;
				}

				case DialogMaterialLayerButtonAnimatedTextureId:
				{
					EnableWindow(GetDlgItem(Window, DialogMaterialLayerButtonTextureId), CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonAnimatedTextureId)));
					EnableWindow(GetDlgItem(Window, DialogMaterialLayerComboTextureId), !CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonAnimatedTextureId)));
					return TRUE;
				}

				case DialogMaterialLayerButtonOk:
				{
					StaticTexture = reinterpret_cast<MODEL_TEXTURE*>(::SendMessage(GetDlgItem(Window, DialogMaterialLayerComboTextureId), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogMaterialLayerComboTextureId), CB_GETCURSEL, 0, 0), 0));
					StaticTextureAnimation = reinterpret_cast<MODEL_TEXTURE_ANIMATION*>(::SendMessage(GetDlgItem(Window, DialogMaterialLayerComboTextureAnimationId), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogMaterialLayerComboTextureAnimationId), CB_GETCURSEL, 0, 0), 0));

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonAnimatedAlpha)))
					{
						StaticData.Alpha.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogMaterialLayerEditAlpha)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonAnimatedTextureId)))
					{
						StaticData.AnimatedTextureId.SetStaticScalarInt(0);
					}
					else
					{
						StaticTexture = NULL;
					}

					StaticData.Unshaded = CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonUnshaded));
					StaticData.Unfogged = CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonUnfogged));
					StaticData.TwoSided = CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonTwoSided));
					StaticData.SphereEnvironmentMap = CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonSphereEnvironmentMap));
					StaticData.NoDepthTest = CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonNoDepthTest));
					StaticData.NoDepthSet = CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialLayerButtonNoDepthSet));

					StaticData.FilterMode = static_cast<FILTER_MODE>(::SendMessage(GetDlgItem(Window, DialogMaterialLayerComboFilterMode), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogMaterialLayerComboFilterMode), CB_GETCURSEL, 0, 0), 0));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogMaterialLayerButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogMaterialLayerButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the texture ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_MATERIAL_LAYER_DIALOG::BuildTextureIdList(HWND Window)
{
	INT i;
	INT Index;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogMaterialLayerComboTextureId);
	if(ComboBox == NULL) return;

	Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(NAME_NO_ID.c_str())));
	::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
	::SendMessage(ComboBox, CB_SETCURSEL, 0, 0);

	for(i = 0; i < Model.Data().TextureContainer.GetTotalSize(); i++)
	{
		if(Model.Data().TextureContainer.ValidIndex(i))
		{
			std::stringstream Stream;

			if(Model.Data().TextureContainer[i]->Data().FileName == "")
			{
				Stream << "Replaceable ID " << Model.Data().TextureContainer[i]->Data().ReplaceableId;
			}
			else
			{
				Stream << Model.Data().TextureContainer[i]->Data().FileName;
			}

			Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Stream.str().c_str())));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Model.Data().TextureContainer[i]));

			if(Model.Data().TextureContainer[i] == StaticTexture)
			{
				::SendMessage(ComboBox, CB_SETCURSEL, Index, 0);
			}
		}
	}
}


//+-----------------------------------------------------------------------------
//| Builds the texture animation ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_MATERIAL_LAYER_DIALOG::BuildTextureAnimationIdList(HWND Window)
{
	INT i;
	INT Index;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogMaterialLayerComboTextureAnimationId);
	if(ComboBox == NULL) return;

	Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(NAME_NO_ID.c_str())));
	::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
	::SendMessage(ComboBox, CB_SETCURSEL, 0, 0);

	for(i = 0; i < Model.Data().TextureAnimationContainer.GetTotalSize(); i++)
	{
		if(Model.Data().TextureAnimationContainer.ValidIndex(i))
		{
			std::stringstream Stream;

			Stream << "TextureAnimation " << Model.Data().TextureContainer[i];
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Stream.str().c_str())));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Model.Data().TextureAnimationContainer[i]));

			if(Model.Data().TextureAnimationContainer[i] == StaticTextureAnimation)
			{
				::SendMessage(ComboBox, CB_SETCURSEL, Index, 0);
			}
		}
	}
}

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowParticleEmitter2Dialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_PARTICLE_EMITTER_2_DIALOG ParticleEmitter2Dialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_PARTICLE_EMITTER_2_DATA WINDOW_PARTICLE_EMITTER_2_DIALOG::StaticData;
MODEL_TEXTURE* WINDOW_PARTICLE_EMITTER_2_DIALOG::StaticTexture;
D3DCOLOR WINDOW_PARTICLE_EMITTER_2_DIALOG::StaticColor1;
D3DCOLOR WINDOW_PARTICLE_EMITTER_2_DIALOG::StaticColor2;
D3DCOLOR WINDOW_PARTICLE_EMITTER_2_DIALOG::StaticColor3;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_PARTICLE_EMITTER_2_DIALOG::WINDOW_PARTICLE_EMITTER_2_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_PARTICLE_EMITTER_2_DIALOG::~WINDOW_PARTICLE_EMITTER_2_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_PARTICLE_EMITTER_2_DIALOG::Display(HWND ParentWindow, MODEL_PARTICLE_EMITTER_2_DATA& Data, MODEL_TEXTURE*& Texture) CONST
{
	StaticData = Data;
	StaticTexture = Texture;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogParticleEmitter2), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		Texture = StaticTexture;
		StaticData = MODEL_PARTICLE_EMITTER_2_DATA();
		return TRUE;
	}

	StaticData = MODEL_PARTICLE_EMITTER_2_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_PARTICLE_EMITTER_2_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT Index;
			HWND ComboBox;

			AddWindow(Window);
			CenterWindow(Window);

			ComboBox = GetDlgItem(Window, DialogParticleEmitter2ComboFilterMode);

			StaticColor1 = Common.Vector3ToColor(StaticData.SegmentColor1);
			StaticColor2 = Common.Vector3ToColor(StaticData.SegmentColor2);
			StaticColor3 = Common.Vector3ToColor(StaticData.SegmentColor3);

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

			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditAlpha1), StaticData.Alpha.x);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditAlpha2), StaticData.Alpha.y);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditAlpha3), StaticData.Alpha.z);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditScaling1), StaticData.ParticleScaling.x);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditScaling2), StaticData.ParticleScaling.y);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditScaling3), StaticData.ParticleScaling.z);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpan1), StaticData.HeadLifeSpan.x);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpan2), StaticData.HeadLifeSpan.y);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpan3), StaticData.HeadLifeSpan.z);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecay1), StaticData.HeadDecay.x);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecay2), StaticData.HeadDecay.y);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecay3), StaticData.HeadDecay.z);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpanTail1), StaticData.TailLifeSpan.x);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpanTail2), StaticData.TailLifeSpan.y);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpanTail3), StaticData.TailLifeSpan.z);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecayTail1), StaticData.TailDecay.x);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecayTail2), StaticData.TailDecay.y);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecayTail3), StaticData.TailDecay.z);

			SetInt(GetDlgItem(Window, DialogParticleEmitter2EditRows), StaticData.Rows);
			SetInt(GetDlgItem(Window, DialogParticleEmitter2EditColumns), StaticData.Columns);
			SetInt(GetDlgItem(Window, DialogParticleEmitter2EditPriorityPlane), StaticData.PriorityPlane);
			SetInt(GetDlgItem(Window, DialogParticleEmitter2EditReplaceableId), StaticData.ReplaceableId);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditTime), StaticData.Time);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpan), StaticData.LifeSpan);
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditTailLength), StaticData.TailLength);

			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditVisibility), StaticData.Visibility.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditEmissionRate), StaticData.EmissionRate.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditSpeed), StaticData.Speed.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditVariation), StaticData.Variation.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditWidth), StaticData.Width.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLength), StaticData.Length.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditGravity), StaticData.Gravity.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLatitude), StaticData.Latitude.GetScalar());

			CheckDlgButton(Window, DialogParticleEmitter2ButtonSortPrimitivesFarZ, BoolToCheckState(StaticData.SortPrimitivesFarZ));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonLineEmitter, BoolToCheckState(StaticData.LineEmitter));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonModelSpace, BoolToCheckState(StaticData.ModelSpace));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonAlphaKey, BoolToCheckState(StaticData.AlphaKey));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonUnshaded, BoolToCheckState(StaticData.Unshaded));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonUnfogged, BoolToCheckState(StaticData.Unfogged));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonXYQuad, BoolToCheckState(StaticData.XYQuad));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonSquirt, BoolToCheckState(StaticData.Squirt));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonHead, BoolToCheckState(StaticData.Head));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonTail, BoolToCheckState(StaticData.Tail));

			CheckDlgButton(Window, DialogParticleEmitter2ButtonAnimatedVisibility, BoolToCheckState(!StaticData.Visibility.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonAnimatedEmissionRate, BoolToCheckState(!StaticData.EmissionRate.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonAnimatedSpeed, BoolToCheckState(!StaticData.Speed.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonAnimatedVariation, BoolToCheckState(!StaticData.Variation.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonAnimatedWidth, BoolToCheckState(!StaticData.Width.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonAnimatedLength, BoolToCheckState(!StaticData.Length.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonAnimatedGravity, BoolToCheckState(!StaticData.Gravity.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitter2ButtonAnimatedLatitude, BoolToCheckState(!StaticData.Latitude.IsStatic()));

			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonVisibility), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedVisibility)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditVisibility), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedVisibility)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonEmissionRate), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedEmissionRate)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditEmissionRate), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedEmissionRate)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonSpeed), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedSpeed)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditSpeed), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedSpeed)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonVariation), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedVariation)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditVariation), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedVariation)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonWidth), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedWidth)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditWidth), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedWidth)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonLength), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedLength)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditLength), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedLength)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonGravity), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedGravity)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditGravity), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedGravity)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonLatitude), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedLatitude)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditLatitude), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedLatitude)));

			BuildTextureIdList(Window);

			return TRUE;
		}

		case WM_PAINT:
		{
			RenderColorBox(Window, GetDlgItem(Window, DialogParticleEmitter2Color1), StaticColor1);
			RenderColorBox(Window, GetDlgItem(Window, DialogParticleEmitter2Color2), StaticColor2);
			RenderColorBox(Window, GetDlgItem(Window, DialogParticleEmitter2Color3), StaticColor3);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogParticleEmitter2ButtonPickColor1:
				{
					if(!SelectColor(Window, StaticColor1))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogParticleEmitter2ButtonPickColor2:
				{
					if(!SelectColor(Window, StaticColor2))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogParticleEmitter2ButtonPickColor3:
				{
					if(!SelectColor(Window, StaticColor3))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogParticleEmitter2ButtonVisibility:
				{
					InterpolatorDialog.Display(Window, StaticData.Visibility, "Visibility");
					return TRUE;
				}

				case DialogParticleEmitter2ButtonEmissionRate:
				{
					InterpolatorDialog.Display(Window, StaticData.EmissionRate, "EmissionRate");
					return TRUE;
				}

				case DialogParticleEmitter2ButtonSpeed:
				{
					InterpolatorDialog.Display(Window, StaticData.Speed, "Speed");
					return TRUE;
				}

				case DialogParticleEmitter2ButtonVariation:
				{
					InterpolatorDialog.Display(Window, StaticData.Variation, "Variation");
					return TRUE;
				}

				case DialogParticleEmitter2ButtonWidth:
				{
					InterpolatorDialog.Display(Window, StaticData.Width, "Width");
					return TRUE;
				}

				case DialogParticleEmitter2ButtonLength:
				{
					InterpolatorDialog.Display(Window, StaticData.Length, "Length");
					return TRUE;
				}

				case DialogParticleEmitter2ButtonGravity:
				{
					InterpolatorDialog.Display(Window, StaticData.Gravity, "Gravity");
					return TRUE;
				}

				case DialogParticleEmitter2ButtonLatitude:
				{
					InterpolatorDialog.Display(Window, StaticData.Latitude, "Latitude");
					return TRUE;
				}

				case DialogParticleEmitter2ButtonAnimatedVisibility:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonVisibility), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedVisibility)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditVisibility), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedVisibility)));
					return TRUE;
				}

				case DialogParticleEmitter2ButtonAnimatedEmissionRate:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonEmissionRate), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedEmissionRate)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditEmissionRate), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedEmissionRate)));
					return TRUE;
				}

				case DialogParticleEmitter2ButtonAnimatedSpeed:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonSpeed), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedSpeed)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditSpeed), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedSpeed)));
					return TRUE;
				}

				case DialogParticleEmitter2ButtonAnimatedVariation:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonVariation), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedVariation)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditVariation), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedVariation)));
					return TRUE;
				}

				case DialogParticleEmitter2ButtonAnimatedWidth:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonWidth), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedWidth)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditWidth), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedWidth)));
					return TRUE;
				}

				case DialogParticleEmitter2ButtonAnimatedLength:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonLength), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedLength)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditLength), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedLength)));
					return TRUE;
				}

				case DialogParticleEmitter2ButtonAnimatedGravity:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonGravity), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedGravity)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditGravity), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedGravity)));
					return TRUE;
				}

				case DialogParticleEmitter2ButtonAnimatedLatitude:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2ButtonLatitude), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedLatitude)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitter2EditLatitude), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedLatitude)));
					return TRUE;
				}

				case DialogParticleEmitter2ButtonOk:
				{
					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedVisibility)))
					{
						StaticData.Visibility.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditVisibility)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedEmissionRate)))
					{
						StaticData.EmissionRate.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditEmissionRate)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedSpeed)))
					{
						StaticData.Speed.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditSpeed)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedVariation)))
					{
						StaticData.Variation.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditVariation)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedWidth)))
					{
						StaticData.Width.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditWidth)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedLength)))
					{
						StaticData.Length.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLength)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedGravity)))
					{
						StaticData.Gravity.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditGravity)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAnimatedLatitude)))
					{
						StaticData.Latitude.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLatitude)));
					}

					StaticData.SegmentColor1 = Common.ColorToVector3(StaticColor1);
					StaticData.SegmentColor2 = Common.ColorToVector3(StaticColor2);
					StaticData.SegmentColor3 = Common.ColorToVector3(StaticColor3);
					StaticData.Alpha.x = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditAlpha1));
					StaticData.Alpha.y = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditAlpha2));
					StaticData.Alpha.z = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditAlpha3));
					StaticData.ParticleScaling.x = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditScaling1));
					StaticData.ParticleScaling.y = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditScaling2));
					StaticData.ParticleScaling.z = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditScaling3));
					StaticData.HeadLifeSpan.x = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpan1));
					StaticData.HeadLifeSpan.y = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpan2));
					StaticData.HeadLifeSpan.z = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpan3));
					StaticData.HeadDecay.x = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecay1));
					StaticData.HeadDecay.y = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecay2));
					StaticData.HeadDecay.z = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecay3));
					StaticData.TailLifeSpan.x = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpanTail1));
					StaticData.TailLifeSpan.y = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpanTail2));
					StaticData.TailLifeSpan.z = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpanTail3));
					StaticData.TailDecay.x = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecayTail1));
					StaticData.TailDecay.y = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecayTail2));
					StaticData.TailDecay.z = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditDecayTail3));

					StaticData.Rows = GetInt(GetDlgItem(Window, DialogParticleEmitter2EditRows));
					StaticData.Columns = GetInt(GetDlgItem(Window, DialogParticleEmitter2EditColumns));
					StaticData.PriorityPlane = GetInt(GetDlgItem(Window, DialogParticleEmitter2EditPriorityPlane));
					StaticData.ReplaceableId = GetInt(GetDlgItem(Window, DialogParticleEmitter2EditReplaceableId));
					StaticData.Time = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditTime));
					StaticData.LifeSpan = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditLifeSpan));
					StaticData.TailLength = GetFloat(GetDlgItem(Window, DialogParticleEmitter2EditTailLength));

					StaticData.SortPrimitivesFarZ = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonSortPrimitivesFarZ));
					StaticData.LineEmitter = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonLineEmitter));
					StaticData.ModelSpace = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonModelSpace));
					StaticData.AlphaKey = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonAlphaKey));
					StaticData.Unshaded = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonUnshaded));
					StaticData.Unfogged = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonUnfogged));
					StaticData.XYQuad = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonXYQuad));
					StaticData.Squirt = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonSquirt));
					StaticData.Head = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonHead));
					StaticData.Tail = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitter2ButtonTail));

					StaticTexture = reinterpret_cast<MODEL_TEXTURE*>(::SendMessage(GetDlgItem(Window, DialogParticleEmitter2ComboTextureId), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogParticleEmitter2ComboTextureId), CB_GETCURSEL, 0, 0), 0));
					StaticData.FilterMode = static_cast<FILTER_MODE>(::SendMessage(GetDlgItem(Window, DialogParticleEmitter2ComboFilterMode), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogParticleEmitter2ComboFilterMode), CB_GETCURSEL, 0, 0), 0));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogParticleEmitter2ButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogParticleEmitter2ButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the texture ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_PARTICLE_EMITTER_2_DIALOG::BuildTextureIdList(HWND Window)
{
	INT i;
	INT Index;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogParticleEmitter2ComboTextureId);
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

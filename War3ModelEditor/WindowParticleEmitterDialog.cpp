//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowParticleEmitterDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_PARTICLE_EMITTER_DIALOG ParticleEmitterDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_PARTICLE_EMITTER_DATA WINDOW_PARTICLE_EMITTER_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_PARTICLE_EMITTER_DIALOG::WINDOW_PARTICLE_EMITTER_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_PARTICLE_EMITTER_DIALOG::~WINDOW_PARTICLE_EMITTER_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_PARTICLE_EMITTER_DIALOG::Display(HWND ParentWindow, MODEL_PARTICLE_EMITTER_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogParticleEmitter), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_PARTICLE_EMITTER_DATA();
		return TRUE;
	}

	StaticData = MODEL_PARTICLE_EMITTER_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_PARTICLE_EMITTER_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetString(GetDlgItem(Window, DialogParticleEmitterEditFileName), StaticData.ParticleFileName);

			SetFloat(GetDlgItem(Window, DialogParticleEmitterEditEmissionRate), StaticData.EmissionRate.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitterEditLifeSpan), StaticData.ParticleLifeSpan.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitterEditInitialVelocity), StaticData.ParticleInitialVelocity.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitterEditGravity), StaticData.Gravity.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitterEditLongitude), StaticData.Longitude.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitterEditLatitude), StaticData.Latitude.GetScalar());
			SetFloat(GetDlgItem(Window, DialogParticleEmitterEditVisibility), StaticData.Visibility.GetScalar());

			CheckDlgButton(Window, DialogParticleEmitterButtonUsesMdl, BoolToCheckState(StaticData.EmitterUsesMdl));
			CheckDlgButton(Window, DialogParticleEmitterButtonUsesTga, BoolToCheckState(StaticData.EmitterUsesTga));

			CheckDlgButton(Window, DialogParticleEmitterButtonAnimatedEmissionRate, BoolToCheckState(!StaticData.EmissionRate.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitterButtonAnimatedLifeSpan, BoolToCheckState(!StaticData.ParticleLifeSpan.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitterButtonAnimatedInitialVelocity, BoolToCheckState(!StaticData.ParticleInitialVelocity.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitterButtonAnimatedGravity, BoolToCheckState(!StaticData.Gravity.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitterButtonAnimatedLongitude, BoolToCheckState(!StaticData.Longitude.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitterButtonAnimatedLatitude, BoolToCheckState(!StaticData.Latitude.IsStatic()));
			CheckDlgButton(Window, DialogParticleEmitterButtonAnimatedVisibility, BoolToCheckState(!StaticData.Visibility.IsStatic()));

			EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonEmissionRate), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedEmissionRate)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditEmissionRate), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedEmissionRate)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonLifeSpan), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLifeSpan)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditLifeSpan), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLifeSpan)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonInitialVelocity), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedInitialVelocity)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditInitialVelocity), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedInitialVelocity)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonGravity), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedGravity)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditGravity), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedGravity)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonLongitude), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLongitude)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditLongitude), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLongitude)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonLatitude), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLatitude)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditLatitude), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLatitude)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonVisibility), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedVisibility)));
			EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditVisibility), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedVisibility)));

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogParticleEmitterButtonEmissionRate:
				{
					InterpolatorDialog.Display(Window, StaticData.EmissionRate, "Emission Rate");
					return TRUE;
				}

				case DialogParticleEmitterButtonLifeSpan:
				{
					InterpolatorDialog.Display(Window, StaticData.ParticleLifeSpan, "Life Span");
					return TRUE;
				}

				case DialogParticleEmitterButtonInitialVelocity:
				{
					InterpolatorDialog.Display(Window, StaticData.ParticleInitialVelocity, "Initial Velocity");
					return TRUE;
				}

				case DialogParticleEmitterButtonGravity:
				{
					InterpolatorDialog.Display(Window, StaticData.Gravity, "Gravity");
					return TRUE;
				}

				case DialogParticleEmitterButtonLongitude:
				{
					InterpolatorDialog.Display(Window, StaticData.Longitude, "Longitude");
					return TRUE;
				}

				case DialogParticleEmitterButtonLatitude:
				{
					InterpolatorDialog.Display(Window, StaticData.Latitude, "Latitude");
					return TRUE;
				}

				case DialogParticleEmitterButtonVisibility:
				{
					InterpolatorDialog.Display(Window, StaticData.Visibility, "Visibility");
					return TRUE;
				}

				case DialogParticleEmitterButtonAnimatedEmissionRate:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonEmissionRate), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedEmissionRate)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditEmissionRate), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedEmissionRate)));
					return TRUE;
				}

				case DialogParticleEmitterButtonAnimatedLifeSpan:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonLifeSpan), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLifeSpan)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditLifeSpan), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLifeSpan)));
					return TRUE;
				}

				case DialogParticleEmitterButtonAnimatedInitialVelocity:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonInitialVelocity), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedInitialVelocity)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditInitialVelocity), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedInitialVelocity)));
					return TRUE;
				}

				case DialogParticleEmitterButtonAnimatedGravity:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonGravity), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedGravity)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditGravity), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedGravity)));
					return TRUE;
				}

				case DialogParticleEmitterButtonAnimatedLongitude:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonLongitude), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLongitude)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditLongitude), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLongitude)));
					return TRUE;
				}

				case DialogParticleEmitterButtonAnimatedLatitude:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonLatitude), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLatitude)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditLatitude), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLatitude)));
					return TRUE;
				}

				case DialogParticleEmitterButtonAnimatedVisibility:
				{
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterButtonVisibility), CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedVisibility)));
					EnableWindow(GetDlgItem(Window, DialogParticleEmitterEditVisibility), !CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedVisibility)));
					return TRUE;
				}

				case DialogParticleEmitterButtonOk:
				{
					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedEmissionRate)))
					{
						StaticData.EmissionRate.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitterEditEmissionRate)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLifeSpan)))
					{
						StaticData.ParticleLifeSpan.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitterEditLifeSpan)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedInitialVelocity)))
					{
						StaticData.ParticleInitialVelocity.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitterEditInitialVelocity)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedGravity)))
					{
						StaticData.Gravity.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitterEditGravity)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLongitude)))
					{
						StaticData.Longitude.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitterEditLongitude)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedLatitude)))
					{
						StaticData.Latitude.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitterEditLatitude)));
					}

					if(!CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonAnimatedVisibility)))
					{
						StaticData.Visibility.SetStaticScalar(GetFloat(GetDlgItem(Window, DialogParticleEmitterEditVisibility)));
					}

					StaticData.EmitterUsesMdl = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonUsesMdl));
					StaticData.EmitterUsesTga = CheckStateToBool(IsDlgButtonChecked(Window, DialogParticleEmitterButtonUsesTga));

					StaticData.ParticleFileName = GetString(GetDlgItem(Window, DialogParticleEmitterEditFileName));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogParticleEmitterButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogParticleEmitterButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

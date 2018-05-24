//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowPropertiesDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_PROPERTIES_DIALOG PropertiesDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
PROPERTIES_INFO WINDOW_PROPERTIES_DIALOG::StaticPropertiesInfo;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_PROPERTIES_DIALOG::WINDOW_PROPERTIES_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_PROPERTIES_DIALOG::~WINDOW_PROPERTIES_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_PROPERTIES_DIALOG::Display(HWND ParentWindow, PROPERTIES_INFO& PropertiesInfo) CONST
{
	StaticPropertiesInfo = PropertiesInfo;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogProperties), ParentWindow, DialogMessageHandler, 0))
	{
		PropertiesInfo = StaticPropertiesInfo;
		return TRUE;
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_PROPERTIES_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			CenterWindow(Window);

			CheckDlgButton(Window, DialogPropertiesButtonUseLighting,      BoolToCheckState(StaticPropertiesInfo.UseLighting));
			CheckDlgButton(Window, DialogPropertiesButtonUseShaders,       BoolToCheckState(StaticPropertiesInfo.UseShaders));
			CheckDlgButton(Window, DialogPropertiesButtonZoom,             BoolToCheckState(StaticPropertiesInfo.RelativeZoom));
			CheckDlgButton(Window, DialogPropertiesButtonMove,             BoolToCheckState(StaticPropertiesInfo.RelativeMove));
			CheckDlgButton(Window, DialogPropertiesButtonClockwise,        BoolToCheckState(StaticPropertiesInfo.ClockwiseCulling));
			CheckDlgButton(Window, DialogPropertiesButtonUseGroundTexture, BoolToCheckState(StaticPropertiesInfo.UseGroundTexture));
			CheckDlgButton(Window, DialogPropertiesButtonDebug,            BoolToCheckState(StaticPropertiesInfo.ShowDebug));
			CheckDlgButton(Window, DialogPropertiesButtonAxises,           BoolToCheckState(StaticPropertiesInfo.ShowAxises));
			CheckDlgButton(Window, DialogPropertiesButtonLights,           BoolToCheckState(StaticPropertiesInfo.ShowLights));
			CheckDlgButton(Window, DialogPropertiesButtonBones,            BoolToCheckState(StaticPropertiesInfo.ShowBones));
			CheckDlgButton(Window, DialogPropertiesButtonCameras,          BoolToCheckState(StaticPropertiesInfo.ShowCameras));
			CheckDlgButton(Window, DialogPropertiesButtonCollisionShapes,  BoolToCheckState(StaticPropertiesInfo.ShowCollisionShapes));
			CheckDlgButton(Window, DialogPropertiesButtonCounterClockwise, !BoolToCheckState(StaticPropertiesInfo.ClockwiseCulling));
			CheckDlgButton(Window, DialogPropertiesButtonPoint,            BoolToCheckState(StaticPropertiesInfo.FillMode == D3DFILL_POINT));
			CheckDlgButton(Window, DialogPropertiesButtonWireframe,        BoolToCheckState(StaticPropertiesInfo.FillMode == D3DFILL_WIREFRAME));
			CheckDlgButton(Window, DialogPropertiesButtonSolid,            BoolToCheckState(StaticPropertiesInfo.FillMode == D3DFILL_SOLID));

			if(!Properties.ShadersAvailable())
			{
				CheckDlgButton(Window, DialogPropertiesButtonUseShaders, FALSE);
				EnableWindow(GetDlgItem(Window, DialogPropertiesButtonUseShaders), FALSE);
			}

			SetFloat(GetDlgItem(Window, DialogPropertiesEditPower), StaticPropertiesInfo.Power);
			SetInt(GetDlgItem(Window, DialogPropertiesEditQuality), StaticPropertiesInfo.BlpQuality);
			SetFloat(GetDlgItem(Window, DialogPropertiesEditNear), StaticPropertiesInfo.NearDistance);
			SetFloat(GetDlgItem(Window, DialogPropertiesEditFar), StaticPropertiesInfo.FarDistance);
			SetFloat(GetDlgItem(Window, DialogPropertiesEditGroundTextureScale), StaticPropertiesInfo.GroundTextureScale);
			SetString(GetDlgItem(Window, DialogPropertiesEditGroundTexture), StaticPropertiesInfo.GroundTexture);

			return TRUE;
		}

		case WM_PAINT:
		{
			RenderColorBox(Window, GetDlgItem(Window, DialogPropertiesColorBackground), StaticPropertiesInfo.Background);
			RenderColorBox(Window, GetDlgItem(Window, DialogPropertiesColorAmbient),    StaticPropertiesInfo.Ambient);
			RenderColorBox(Window, GetDlgItem(Window, DialogPropertiesColorDiffuse),    StaticPropertiesInfo.Diffuse);
			RenderColorBox(Window, GetDlgItem(Window, DialogPropertiesColorSpecular),   StaticPropertiesInfo.Specular);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogPropertiesButtonOk:
				{
					FLOAT TempPower;

					StaticPropertiesInfo.UseLighting         = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonUseLighting));
					StaticPropertiesInfo.UseShaders          = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonUseShaders));
					StaticPropertiesInfo.RelativeZoom        = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonZoom));
					StaticPropertiesInfo.RelativeMove        = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonMove));
					StaticPropertiesInfo.ClockwiseCulling    = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonClockwise));
					StaticPropertiesInfo.UseGroundTexture    = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonUseGroundTexture));
					StaticPropertiesInfo.ShowDebug           = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonDebug));
					StaticPropertiesInfo.ShowAxises          = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonAxises));
					StaticPropertiesInfo.ShowLights          = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonLights));
					StaticPropertiesInfo.ShowBones           = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonBones));
					StaticPropertiesInfo.ShowCameras         = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonCameras));
					StaticPropertiesInfo.ShowCollisionShapes = CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonCollisionShapes));

					if(CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonPoint)))
					{
						StaticPropertiesInfo.FillMode = D3DFILL_POINT;
					}
					else if(CheckStateToBool(IsDlgButtonChecked(Window, DialogPropertiesButtonWireframe)))
					{
						StaticPropertiesInfo.FillMode = D3DFILL_WIREFRAME;
					}
					else
					{
						StaticPropertiesInfo.FillMode = D3DFILL_SOLID;
					}

					TempPower = GetFloat(GetDlgItem(Window, DialogPropertiesEditPower), -1.0f);
					if(TempPower < 0.0f)
					{
						Error.SetMessage("The Specular Power must be at least 0.0!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogPropertiesEditPower));
						::SendMessage(GetDlgItem(Window, DialogPropertiesEditPower), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticPropertiesInfo.Power = TempPower;

					StaticPropertiesInfo.BlpQuality = GetInt(GetDlgItem(Window, DialogPropertiesEditQuality), -1);
					if((StaticPropertiesInfo.BlpQuality < 0) || (StaticPropertiesInfo.BlpQuality > 100))
					{
						Error.SetMessage("The BLP Quality must be between 0 and 100!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogPropertiesEditQuality));
						::SendMessage(GetDlgItem(Window, DialogPropertiesEditQuality), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticPropertiesInfo.NearDistance = GetFloat(GetDlgItem(Window, DialogPropertiesEditNear), -1.0f);
					if(StaticPropertiesInfo.NearDistance < 0.0)
					{
						Error.SetMessage("The Near Distance has to be greater than 0.0!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogPropertiesEditNear));
						::SendMessage(GetDlgItem(Window, DialogPropertiesEditNear), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticPropertiesInfo.FarDistance = GetFloat(GetDlgItem(Window, DialogPropertiesEditFar), -1.0f);
					if(StaticPropertiesInfo.FarDistance < 0.0)
					{
						Error.SetMessage("The Far Distance has to be greater than 0.0!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogPropertiesEditFar));
						::SendMessage(GetDlgItem(Window, DialogPropertiesEditFar), EM_SETSEL, 0, -1);

						return TRUE;
					}

					if(StaticPropertiesInfo.FarDistance <= StaticPropertiesInfo.NearDistance)
					{
						Error.SetMessage("The Far Distance has to be greater than the Near Distance!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogPropertiesEditFar));
						::SendMessage(GetDlgItem(Window, DialogPropertiesEditFar), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticPropertiesInfo.GroundTextureScale = GetFloat(GetDlgItem(Window, DialogPropertiesEditGroundTextureScale), 1.0f);
					StaticPropertiesInfo.GroundTexture = GetString(GetDlgItem(Window, DialogPropertiesEditGroundTexture));

					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogPropertiesButtonCancel:
				{
					EndDialog(Window, 0);
					return TRUE;
				}

				case DialogPropertiesButtonPickBackground:
				{
					if(!SelectColor(Window, StaticPropertiesInfo.Background))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogPropertiesButtonPickAmbient:
				{
					if(!SelectColor(Window, StaticPropertiesInfo.Ambient))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogPropertiesButtonPickDiffuse:
				{
					if(!SelectColor(Window, StaticPropertiesInfo.Diffuse))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogPropertiesButtonPickSpecular:
				{
					if(!SelectColor(Window, StaticPropertiesInfo.Specular))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogPropertiesButtonDefaultBackground:
				{
					StaticPropertiesInfo.Background = PROPERTIES_DEFAULT_BACKGROUND;
					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogPropertiesButtonDefaultAmbient:
				{
					StaticPropertiesInfo.Ambient = PROPERTIES_DEFAULT_AMBIENT;
					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogPropertiesButtonDefaultDiffuse:
				{
					StaticPropertiesInfo.Diffuse = PROPERTIES_DEFAULT_DIFFUSE;
					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogPropertiesButtonDefaultSpecular:
				{
					StaticPropertiesInfo.Specular = PROPERTIES_DEFAULT_SPECULAR;
					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogPropertiesButtonDefaultPower:
				{
					StaticPropertiesInfo.Power = PROPERTIES_DEFAULT_SPECULAR_POWER;
					SetFloat(GetDlgItem(Window, DialogPropertiesEditPower), StaticPropertiesInfo.Power);
					return TRUE;
				}

				case DialogPropertiesButtonDefaultQuality:
				{
					StaticPropertiesInfo.BlpQuality = DEFAULT_BLP_QUALITY;
					SetInt(GetDlgItem(Window, DialogPropertiesEditQuality), StaticPropertiesInfo.BlpQuality);
					return TRUE;
				}

				case DialogPropertiesButtonDefaultClippingPlane:
				{
					StaticPropertiesInfo.NearDistance = GRAPHICS_DEFAULT_NEAR_DISTANCE;
					StaticPropertiesInfo.FarDistance = GRAPHICS_DEFAULT_FAR_DISTANCE;
					SetFloat(GetDlgItem(Window, DialogPropertiesEditNear), StaticPropertiesInfo.NearDistance);
					SetFloat(GetDlgItem(Window, DialogPropertiesEditFar), StaticPropertiesInfo.FarDistance);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogPropertiesButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}

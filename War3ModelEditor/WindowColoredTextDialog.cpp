//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowColoredTextDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_COLORED_TEXT_DIALOG ColoredTextDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
BOOL WINDOW_COLORED_TEXT_DIALOG::Gradient = FALSE;
D3DCOLOR WINDOW_COLORED_TEXT_DIALOG::Color1 = 0xFFFFFFFF;
D3DCOLOR WINDOW_COLORED_TEXT_DIALOG::Color2 = 0xFFFFFFFF;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_COLORED_TEXT_DIALOG::WINDOW_COLORED_TEXT_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_COLORED_TEXT_DIALOG::~WINDOW_COLORED_TEXT_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_COLORED_TEXT_DIALOG::Display(HWND ParentWindow) CONST
{
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogColoredText), ParentWindow, DialogMessageHandler, 0))
	{
		return TRUE;
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Colorizes some text in a solid color
//+-----------------------------------------------------------------------------
BOOL WINDOW_COLORED_TEXT_DIALOG::ColorizeSolidText(std::string& Text)
{
	std::string OriginalText;

	OriginalText = Text;
	Text = "|c" + ColorToString(Color1) + OriginalText + "|r";

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Colorizes some text in a gradient color
//+-----------------------------------------------------------------------------
BOOL WINDOW_COLORED_TEXT_DIALOG::ColorizeGradientText(std::string& Text)
{
	INT i;
	INT Size;
	std::string OriginalText;
	std::string ColorString;
	std::string LastColorString;

	OriginalText = Text;

	Size = static_cast<INT>(OriginalText.size());
	if(Size == 0)
	{
		Text = "|c" + ColorToString(Color1) + "|r";
		return TRUE;
	}
	else if(Size == 1)
	{
		Text = "|c" + ColorToString(Color1) + OriginalText + "|r";
		return TRUE;
	}

	Text = "";

	for(i = 0; i < Size; i++)
	{
		LastColorString = ColorString;
		ColorString = ColorToString(InterpolateColor(i, Size));

		if(ColorString != LastColorString)
		{
			Text += "|c";
			Text += ColorToString(InterpolateColor(i, Size));
		}

		Text += OriginalText[i];
	}

	Text += "|r";

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Converts a color into  a string
//+-----------------------------------------------------------------------------
std::string WINDOW_COLORED_TEXT_DIALOG::ColorToString(D3DCOLOR Color)
{
	INT A;
	INT R;
	INT G;
	INT B;

	A = 0;
	R = GetBValue(Color);
	G = GetGValue(Color);
	B = GetRValue(Color);

	return NumberToString(A) + NumberToString(R) + NumberToString(G) + NumberToString(B);
}


//+-----------------------------------------------------------------------------
//| Converts a number to a string
//+-----------------------------------------------------------------------------
std::string WINDOW_COLORED_TEXT_DIALOG::NumberToString(INT Number)
{
	std::string String;

	String += DecToHex(Number / 16);
	String += DecToHex(Number % 16);

	return String;
}


//+-----------------------------------------------------------------------------
//| Converts a decimal number into a hexadecimal number
//+-----------------------------------------------------------------------------
CHAR WINDOW_COLORED_TEXT_DIALOG::DecToHex(INT Number)
{
	if(Number < 0) return '0';
	if(Number < 10) return '0' + Number;
	if(Number < 16) return 'A' + (Number - 10);

	return '0';
}


//+-----------------------------------------------------------------------------
//| Interpolates a color
//+-----------------------------------------------------------------------------
D3DCOLOR WINDOW_COLORED_TEXT_DIALOG::InterpolateColor(INT Index, INT Size)
{
	FLOAT Factor;
	FLOAT InverseFactor;
	D3DCOLOR Color;
	D3DXCOLOR TempColor;
	D3DXCOLOR TempColor1;
	D3DXCOLOR TempColor2;

	TempColor1 = Color1;
	TempColor2 = Color2;

	Factor = static_cast<FLOAT>(Index) / static_cast<FLOAT>(Size - 1);
	InverseFactor = 1.0f - Factor;

	TempColor.r = (InverseFactor * TempColor1.r) + (Factor * TempColor2.r);
	TempColor.g = (InverseFactor * TempColor1.g) + (Factor * TempColor2.g);
	TempColor.b = (InverseFactor * TempColor1.b) + (Factor * TempColor2.b);
	TempColor.a = 0.0f;

	Color = TempColor;

	return Color;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_COLORED_TEXT_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			CenterWindow(Window);

			CheckDlgButton(Window, DialogColoredTextButtonSolid, BoolToCheckState(!Gradient));
			CheckDlgButton(Window, DialogColoredTextButtonGradient, BoolToCheckState(Gradient));

			EnableWindow(GetDlgItem(Window, DialogColoredTextButtonPick2), Gradient);
			EnableWindow(GetDlgItem(Window, DialogColoredTextStaticText), Gradient);

			return TRUE;
		}

		case WM_PAINT:
		{
			D3DCOLOR RealColor2;

			RealColor2 = CheckStateToBool(IsDlgButtonChecked(Window, DialogColoredTextButtonGradient)) ? Color2 : PROPERTIES_NO_BACKGROUND;

			RenderColorBox(Window, GetDlgItem(Window, DialogColoredTextColor1), Color1);
			RenderColorBox(Window, GetDlgItem(Window, DialogColoredTextColor2), RealColor2);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogColoredTextButtonGenerate:
				{
					INT Size;
					std::string Text;
					std::vector<CHAR> Buffer;

					Size = ::GetWindowTextLength(GetDlgItem(Window, DialogColoredTextEditInput)) + 1;
					Buffer.resize(Size + 1);

					::GetWindowText(GetDlgItem(Window, DialogColoredTextEditInput), &Buffer[0], Size);
					Buffer[Size] = '\0';
					Text = &Buffer[0];

					if(CheckStateToBool(IsDlgButtonChecked(Window, DialogColoredTextButtonSolid)))
					{
						if(!ColorizeSolidText(Text))
						{
							Error.DisplayMessage(Window);
							Error.ClearMessage();
							return TRUE;
						}
					}
					else
					{
						if(!ColorizeGradientText(Text))
						{
							Error.DisplayMessage(Window);
							Error.ClearMessage();
							return TRUE;
						}
					}

					Common.SetClipboardData(Text);

					SetDlgItemText(Window, DialogColoredTextEditOutput, Text.c_str());

					return TRUE;
				}

				case DialogColoredTextButtonPick1:
				{
					if(!SelectColor(Window, Color1))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogColoredTextButtonPick2:
				{
					if(!SelectColor(Window, Color2))
					{
						SendMessage(Window, WM_PAINT, NULL, NULL);
						return TRUE;
					}

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogColoredTextButtonSolid:
				{
					Gradient = FALSE;

					EnableWindow(GetDlgItem(Window, DialogColoredTextButtonPick2), Gradient);
					EnableWindow(GetDlgItem(Window, DialogColoredTextStaticText), Gradient);

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}

				case DialogColoredTextButtonGradient:
				{
					Gradient = TRUE;

					EnableWindow(GetDlgItem(Window, DialogColoredTextButtonPick2), Gradient);
					EnableWindow(GetDlgItem(Window, DialogColoredTextStaticText), Gradient);

					SendMessage(Window, WM_PAINT, NULL, NULL);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			EndDialog(Window, 1);
			return TRUE;
		}
	}

	return FALSE;
}

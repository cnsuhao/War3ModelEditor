//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Font.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
FONT Font;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
FONT::FONT()
{
	Font = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
FONT::~FONT()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Returns the font
//+-----------------------------------------------------------------------------
LPD3DXFONT FONT::GetFont()
{
	return Font;
}


//+-----------------------------------------------------------------------------
//| Sets up the font
//+-----------------------------------------------------------------------------
BOOL FONT::Setup()
{
	D3DXFONT_DESC FontDescription;
	LPDIRECT3DDEVICE9 Direct3DDevice;

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL)
	{
		Error.SetMessage("Unable to create font, unable to retrieve the Direct3D device!");
		return FALSE;
	}

	FontDescription.Width = 0;
	FontDescription.Height = 20;
	FontDescription.Weight = FW_NORMAL;
	FontDescription.MipLevels = D3DX_DEFAULT;
	FontDescription.Italic = FALSE;
	FontDescription.CharSet = DEFAULT_CHARSET;
	FontDescription.OutputPrecision = OUT_DEFAULT_PRECIS;
	FontDescription.Quality = DEFAULT_QUALITY;
	FontDescription.PitchAndFamily = DEFAULT_PITCH;
	strcpy(FontDescription.FaceName, "Fixedsys");

	if(FAILED(D3DXCreateFontIndirect(Direct3DDevice, &FontDescription, &Font)))
	{
		Error.SetMessage("Unable to create font!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Shuts down the font
//+-----------------------------------------------------------------------------
VOID FONT::Shutdown()
{
	SAFE_RELEASE(Font);
}


//+-----------------------------------------------------------------------------
//| Action performed when the device is lost
//+-----------------------------------------------------------------------------
VOID FONT::OnLostDevice()
{
	if(Font) Font->OnLostDevice();
}


//+-----------------------------------------------------------------------------
//| Action performed when the device is reset
//+-----------------------------------------------------------------------------
VOID FONT::OnResetDevice()
{
	if(Font) Font->OnResetDevice();
}

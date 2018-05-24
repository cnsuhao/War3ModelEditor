//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Texture.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEXTURE::TEXTURE()
{
	Texture = NULL;
	FileName = "";

	Width = 0;
	Height = 0;
	RealWidth = 0;
	RealHeight = 0;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEXTURE::~TEXTURE()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the texture
//+-----------------------------------------------------------------------------
VOID TEXTURE::Clear()
{
	TextureReference.Detach();

	SAFE_RELEASE(Texture);
	FileName = "";

	Width = 0;
	Height = 0;
	RealWidth = 0;
	RealHeight = 0;
}


//+-----------------------------------------------------------------------------
//| Returns the texture
//+-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 TEXTURE::GetTexture()
{
	return Texture;
}


//+-----------------------------------------------------------------------------
//| Returns the filename
//+-----------------------------------------------------------------------------
std::string TEXTURE::GetFileName() CONST
{
	return FileName;
}


//+-----------------------------------------------------------------------------
//| Returns the width
//+-----------------------------------------------------------------------------
INT TEXTURE::GetWidth() CONST
{
	return Width;
}


//+-----------------------------------------------------------------------------
//| Returns the height
//+-----------------------------------------------------------------------------
INT TEXTURE::GetHeight() CONST
{
	return Height;
}


//+-----------------------------------------------------------------------------
//| Returns the internally stored width
//+-----------------------------------------------------------------------------
INT TEXTURE::GetRealWidth() CONST
{
	return RealWidth;
}


//+-----------------------------------------------------------------------------
//| Returns the internally stored height
//+-----------------------------------------------------------------------------
INT TEXTURE::GetRealHeight() CONST
{
	return RealHeight;
}


//+-----------------------------------------------------------------------------
//| Creates an empty single-colored texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE::CreateEmpty(INT NewWidth, INT NewHeight)
{
	D3DSURFACE_DESC SurfaceInfo;
	LPDIRECT3DDEVICE9 Direct3DDevice;

	Clear();

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL)
	{
		Error.SetMessage("Unable to create an empty texture, unable to retrieve the Direct3D device!");
		return FALSE;
	}

	if(FAILED(D3DXCreateTexture(Direct3DDevice, NewWidth, NewHeight, D3DX_DEFAULT, 0, Graphics.GetTextureFormat(), D3DPOOL_MANAGED, &Texture)))
	{
		Error.SetMessage("Unable to create an empty texture!");
		return FALSE;
	}

	if(FAILED(Texture->GetLevelDesc(0, &SurfaceInfo)))
	{
		Error.SetMessage("Unable to create an empty texture, unable to retrieve texture info!");
		return FALSE;
	}

	FileName = "EMPTY";
	Width = NewWidth;
	Height = NewHeight;
	RealWidth = SurfaceInfo.Width;
	RealHeight = SurfaceInfo.Height;

	Attach();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Attaches the texture to the graphics class
//+-----------------------------------------------------------------------------
VOID TEXTURE::Attach()
{
	TextureReference.SetData(this);
	TextureReference.Attach(Graphics.TextureReferenceObject);
}

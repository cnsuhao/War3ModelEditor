//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TextureLoaderBasic.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_BASIC TextureLoaderBasic;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_BASIC::TEXTURE_LOADER_BASIC()
{
	RegisterFileFormat("bmp", D3DXIFF_BMP);
	RegisterFileFormat("tga", D3DXIFF_TGA);
	RegisterFileFormat("png", D3DXIFF_PNG);
	RegisterFileFormat("jpg", D3DXIFF_JPG);
	RegisterFileFormat("jpeg", D3DXIFF_JPG);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_BASIC::~TEXTURE_LOADER_BASIC()
{
	FileFormatMap.clear();
}


//+-----------------------------------------------------------------------------
//| Saves a texture to a buffer
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_BASIC::Save(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer)
{
	INT BufferSize;
	std::string Extention;
	LPD3DXBUFFER TempBuffer = NULL;
	std::map<std::string, D3DXIMAGE_FILEFORMAT>::iterator i;

	CurrentFileName = FileName;

	Extention = Common.LowerCase(Common.GetExtention(FileName));
	i = FileFormatMap.find(Extention);
	if(i == FileFormatMap.end())
	{
		Error.SetMessage("Unable to save \"" + FileName + "\", unknown file format!");
		return FALSE;
	}

	if(FAILED(D3DXSaveTextureToFileInMemory(&TempBuffer, i->second, Texture.GetTexture(), NULL)))
	{
		Error.SetMessage("Unable to save \"" + FileName + "\"!");
		return FALSE;
	}
	

	BufferSize = TempBuffer->GetBufferSize();
	if(!Buffer.Resize(BufferSize))
	{
		Error.SetMessage("Unable to save \"" + FileName + "\", unable to create temporary buffer!");
		return FALSE;
	}

	memcpy(Buffer.GetData(), TempBuffer->GetBufferPointer(), BufferSize);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a texture from a buffer
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_BASIC::Load(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer)
{
	D3DXIMAGE_INFO ImageInfo;
	D3DSURFACE_DESC SurfaceInfo;
	LPDIRECT3DDEVICE9 Direct3DDevice;

	CurrentFileName = FileName;

	Texture.Clear();

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL)
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", unable to retrieve the Direct3D device!");
		return FALSE;
	}

	if(FAILED(D3DXCreateTextureFromFileInMemoryEx(Direct3DDevice, Buffer.GetData(), Buffer.GetSize(),
												  D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
												  Graphics.GetTextureFormat(),
												  D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
												  0, &ImageInfo, NULL, &Texture.Texture)))
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", texture creation failed!");
		return FALSE;
	}

	if(FAILED(Texture.Texture->GetLevelDesc(0, &SurfaceInfo)))
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", unable to retrieve texture info!");
		return FALSE;
	}

	Texture.FileName = FileName;
	Texture.Width = ImageInfo.Width;
	Texture.Height = ImageInfo.Height;
	Texture.RealWidth = SurfaceInfo.Width;
	Texture.RealHeight = SurfaceInfo.Height;

	Texture.Attach();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Registers a new file format
//+-----------------------------------------------------------------------------
VOID TEXTURE_LOADER_BASIC::RegisterFileFormat(CONST std::string& Extention, D3DXIMAGE_FILEFORMAT FileFormat)
{
	FileFormatMap.insert(std::make_pair(Extention, FileFormat));
}

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TextureLoaderBlp.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_BLP TextureLoaderBlp;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_BLP::TEXTURE_LOADER_BLP()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_BLP::~TEXTURE_LOADER_BLP()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Saves a texture to a buffer
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_BLP::Save(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer)
{
	INT i;
	INT X;
	INT Y;
	INT Size;
	INT Index;
	INT BufferIndex;
	INT TotalSize;
	INT NrOfMipMaps;
	INT TextureSize;
	INT CurrentWidth;
	INT CurrentHeight;
	INT CurrentOffset;
	BUFFER TempBuffer;
	BLP_HEADER Header;
	CONST UCHAR* Pointer;
	DWORD JpegHeaderSize;
	std::stringstream Stream;
	D3DLOCKED_RECT LockedRect;
	LPDIRECT3DSURFACE9 Surface;
	std::vector<BUFFER> MipMapBufferList;

	CurrentFileName = FileName;

	JpegHeaderSize = 4;
	MipMapBufferList.resize(MAX_NR_OF_BLP_MIP_MAPS);

	Header.Compression = 0;
	Header.Flags = 8;
	Header.Width = Texture.GetWidth();
	Header.Height = Texture.GetHeight();
	Header.PictureType = 4;
	Header.PictureSubType = 1;

	NrOfMipMaps = 0;

	Size = max(Header.Width, Header.Height);
	while(Size >= 1)
	{
		Size /= 2;
		NrOfMipMaps++;
	}

	if(NrOfMipMaps > MAX_NR_OF_BLP_MIP_MAPS)
	{
		NrOfMipMaps = MAX_NR_OF_BLP_MIP_MAPS;
	}

	if(NrOfMipMaps < 1)
	{
		Error.SetMessage("Unable to save \"" + FileName + "\", texture size is too small!");
		return FALSE;
	}

	CurrentWidth = Header.Width;
	CurrentHeight = Header.Height;
	CurrentOffset = sizeof(BLP_HEADER) + sizeof(DWORD) + JpegHeaderSize;
	for(i = 0; i < NrOfMipMaps; i++)
	{
		if(!TempBuffer.Resize(CurrentWidth * CurrentHeight * 4)) return FALSE;

		if(FAILED(Texture.GetTexture()->GetSurfaceLevel(i, &Surface)))
		{
			Stream << "Unable to save \"" + FileName + "\", unable to retrieve mipmap (mipmap level " << i << ")!";
			Error.SetMessage(Stream.str());
			return FALSE;
		}

		if(FAILED(Surface->LockRect(&LockedRect, NULL, D3DLOCK_READONLY)))
		{
			Error.SetMessage("Unable to save \"" + FileName + "\", surface locking failed!");
			SAFE_RELEASE(Surface);
			return FALSE;
		}

		Index = 0;
		BufferIndex = 0;
		Pointer = reinterpret_cast<CONST UCHAR*>(LockedRect.pBits);

		for(Y = 0; Y < static_cast<INT>(CurrentHeight); Y++)
		{
			for(X = 0; X < static_cast<INT>(CurrentWidth); X++)
			{
				TempBuffer[BufferIndex++] = Pointer[Index++];
				TempBuffer[BufferIndex++] = Pointer[Index++];
				TempBuffer[BufferIndex++] = Pointer[Index++];
				TempBuffer[BufferIndex++] = Pointer[Index++];
			}

			Index += LockedRect.Pitch - (CurrentWidth * 4);
		}

		Surface->UnlockRect();
		SAFE_RELEASE(Surface);

		/*
		if(!Jpeg.Write(TempBuffer, MipMapBufferList[i], CurrentWidth, CurrentHeight, Properties().BlpQuality))
		{
			Stream << "Unable to save \"" + FileName + "\", BLP writing failed (mipmap level " << i << ")!";
			Error.SetMessage(Stream.str());
			return FALSE;
		}
		*/

		TextureSize = MipMapBufferList[i].GetSize();

		Header.Offset[i] = CurrentOffset;
		Header.Size[i] = TextureSize - JpegHeaderSize;

		CurrentWidth /= 2;
		CurrentHeight /= 2;
		CurrentOffset += Header.Size[i];

		if(CurrentWidth < 1) CurrentWidth = 1;
		if(CurrentHeight < 1) CurrentHeight = 1;
	}

	TotalSize = sizeof(BLP_HEADER) + sizeof(DWORD) + JpegHeaderSize;
	for(i = 0; i < NrOfMipMaps; i++)
	{
		if(MipMapBufferList[i].GetSize() <= 0) break;
		TotalSize += Header.Size[i];
	}

	if(!Buffer.Resize(TotalSize)) return FALSE;

	CurrentOffset = 0;

	memcpy(&Buffer[CurrentOffset], &Header, sizeof(BLP_HEADER));
	CurrentOffset += sizeof(BLP_HEADER);

	memcpy(&Buffer[CurrentOffset], &JpegHeaderSize, sizeof(DWORD));
	CurrentOffset += sizeof(DWORD);

	Size = Header.Size[0] + JpegHeaderSize;
	memcpy(&Buffer[CurrentOffset], &((MipMapBufferList[0])[0]), Size);
	CurrentOffset += Size;

	for(i = 1; i < NrOfMipMaps; i++)
	{
		if(MipMapBufferList[i].GetSize() <= 0) break;

		memcpy(&Buffer[CurrentOffset], &((MipMapBufferList[i])[JpegHeaderSize]), Header.Size[i]);
		CurrentOffset += Header.Size[i];
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a texture from a buffer
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_BLP::Load(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer)
{
	BLP_HEADER Header;
	D3DSURFACE_DESC SurfaceInfo;

	CurrentFileName = FileName;

	memcpy(reinterpret_cast<CHAR*>(&Header), &Buffer[0], sizeof(BLP_HEADER));
	if(Header.MagicNumber != '1PLB')
	{
		Error.SetMessage("The file is not a BLP texture!");
		return FALSE;
	}

	switch(Header.Compression)
	{
		case 0:
		{
			if(!LoadCompressed(Texture, Header, Buffer)) return FALSE;
			break;
		}

		case 1:
		{
			if(!LoadUncompressed(Texture, Header, Buffer)) return FALSE;
			break;
		}

		default:
		{
			Error.SetMessage("Unable to load \"" + FileName + "\", unknown compression method!");
			return FALSE;
		}
	}

	if(FAILED(D3DXFilterTexture(Texture.Texture, NULL, D3DX_DEFAULT, D3DX_DEFAULT)))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", texture filtering failed!");
		return FALSE;
	}

	if(FAILED(Texture.Texture->GetLevelDesc(0, &SurfaceInfo)))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unable to retrieve texture info!");
		return FALSE;
	}

	Texture.FileName = FileName;
	Texture.Width = Header.Width;
	Texture.Height = Header.Height;
	Texture.RealWidth = SurfaceInfo.Width;
	Texture.RealHeight = SurfaceInfo.Height;

	Texture.Attach();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a compressed blp texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_BLP::LoadCompressed(TEXTURE& Texture, BLP_HEADER& Header, BUFFER& Buffer)
{
	INT X;
	INT Y;
	INT Index;
	INT BufferIndex;
	UCHAR* Pointer;
	BUFFER TempBuffer;
	BUFFER TempBuffer2;
	D3DLOCKED_RECT LockedRect;
	LPDIRECT3DDEVICE9 Direct3DDevice;
	DWORD JpegHeaderSize;

	std::memcpy(reinterpret_cast<CHAR*>(&JpegHeaderSize), &Buffer[sizeof(BLP_HEADER)], sizeof(DWORD));

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unable to retrieve the Direct3D device!");
		return FALSE;
	}

	if(FAILED(D3DXCreateTexture(Direct3DDevice, Header.Width, Header.Height, D3DX_DEFAULT, 0,
		Graphics.GetTextureFormat(), D3DPOOL_MANAGED, &Texture.Texture)))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", texture creation failed!");
		return FALSE;
	}

	if(!TempBuffer2.Resize(Header.Size[0] + JpegHeaderSize))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", buffer resizing failed!");
		return FALSE;
	}

	std::memcpy(&TempBuffer2[0], &Buffer[sizeof(BLP_HEADER) + sizeof(DWORD)], JpegHeaderSize);
	std::memcpy(&TempBuffer2[JpegHeaderSize], &Buffer[Header.Offset[0]], Header.Size[0]);

	if(!Jpeg.Read(TempBuffer2, TempBuffer))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", BLP reading failed!");
		return FALSE;
	}

	if(FAILED(Texture.Texture->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD)))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", texture locking failed!");
		return FALSE;
	}

	Index = 0;
	BufferIndex = 0;
	Pointer = reinterpret_cast<UCHAR*>(LockedRect.pBits);

	for(Y = 0; Y < static_cast<INT>(Header.Height); Y++)
	{
		for(X = 0; X < static_cast<INT>(Header.Width); X++)
		{
			Pointer[Index++] = TempBuffer[BufferIndex++];
			Pointer[Index++] = TempBuffer[BufferIndex++];
			Pointer[Index++] = TempBuffer[BufferIndex++];
			Pointer[Index++] = TempBuffer[BufferIndex++];
		}

		Index += LockedRect.Pitch - (Header.Width * 4);
	}

	Texture.Texture->UnlockRect(0);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads an uncompressed blp texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_BLP::LoadUncompressed(TEXTURE& Texture, BLP_HEADER& Header, BUFFER& Buffer)
{
	INT i;
	INT X;
	INT Y;
	INT Size;
	INT Index;
	INT BufferIndex;
	UCHAR* Pointer;
	BUFFER TempBuffer;
	D3DLOCKED_RECT LockedRect;
	LPDIRECT3DDEVICE9 Direct3DDevice;
	CONST INT PALETTE_SIZE = 256;
	BLP_RGBA Palette[PALETTE_SIZE];
	BLP_RGBA* TargetPixel;

	memcpy(reinterpret_cast<CHAR*>(Palette), &Buffer[sizeof(BLP_HEADER)], (PALETTE_SIZE * 4));

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unable to retrieve the Direct3D device!");
		return FALSE;
	}

	if(FAILED(D3DXCreateTexture(Direct3DDevice, Header.Width, Header.Height, D3DX_DEFAULT, 0,
								Graphics.GetTextureFormat(), D3DPOOL_MANAGED, &Texture.Texture)))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", texture creation failed!");
		return FALSE;
	}

	Size = Header.Width * Header.Height;
	if(!TempBuffer.Resize(Size * 4))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", buffer resizing failed!");
		return FALSE;
	}

	switch(Header.PictureType)
	{
		case 3:
		case 4:
		{
			BLP_PIXEL* SourcePixel;

			SourcePixel = reinterpret_cast<BLP_PIXEL*>(&Buffer[sizeof(BLP_HEADER) + (PALETTE_SIZE * 4)]);
			TargetPixel = reinterpret_cast<BLP_RGBA*>(&TempBuffer[0]);

			for(i = 0; i < Size; i++)
			{
				TargetPixel[i].Red = Palette[SourcePixel[i].Index].Red;
				TargetPixel[i].Green = Palette[SourcePixel[i].Index].Green;
				TargetPixel[i].Blue = Palette[SourcePixel[i].Index].Blue;
			}

			for(i = 0; i < Size; i++)
			{
				TargetPixel[i].Alpha = SourcePixel[Size + i].Index;
			}

			break;
		}

		case 5:
		{
			BLP_PIXEL* SourcePixel;

			SourcePixel = reinterpret_cast<BLP_PIXEL*>(&Buffer[sizeof(BLP_HEADER) + (PALETTE_SIZE * 4)]);
			TargetPixel = reinterpret_cast<BLP_RGBA*>(&TempBuffer[0]);

			for(i = 0; i < Size; i++)
			{
				TargetPixel[i].Red = Palette[SourcePixel[i].Index].Red;
				TargetPixel[i].Green = Palette[SourcePixel[i].Index].Green;
				TargetPixel[i].Blue = Palette[SourcePixel[i].Index].Blue;
				TargetPixel[i].Alpha = 255 - Palette[SourcePixel[i].Index].Alpha;
			}

			break;
		}

		default:
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown picture type!");
			return FALSE;
		}
	}

	if(FAILED(Texture.Texture->LockRect(0, &LockedRect, NULL, 0)))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", texture locking failed!");
		return FALSE;
	}

	Index = 0;
	BufferIndex = 0;
	Pointer = reinterpret_cast<UCHAR*>(LockedRect.pBits);

	for(Y = 0; Y < static_cast<INT>(Header.Height); Y++)
	{
		for(X = 0; X < static_cast<INT>(Header.Width); X++)
		{
			Pointer[Index++] = TempBuffer[BufferIndex++];
			Pointer[Index++] = TempBuffer[BufferIndex++];
			Pointer[Index++] = TempBuffer[BufferIndex++];
			Pointer[Index++] = TempBuffer[BufferIndex++];
		}

		Index += LockedRect.Pitch - (Header.Width * 4);
	}

	Texture.Texture->UnlockRect(0);

	return TRUE;
}

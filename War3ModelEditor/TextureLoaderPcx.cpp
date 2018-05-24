//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TextureLoaderPcx.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_PCX TextureLoaderPcx;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_PCX::TEXTURE_LOADER_PCX()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_PCX::~TEXTURE_LOADER_PCX()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Saves a texture to a buffer
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_PCX::Save(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer)
{
	Error.SetMessage("Saving *.pcx files are currently unsupported!");
	return FALSE;

	/*
	PCX_HEADER Header;
	DATA_OUT_STREAM DataStream;

	CurrentFileName = FileName;
	DataStream.SetFileName(FileName);

	//XXXXXXXXXXXX

	DataStream.Write(reinterpret_cast<CHAR*>(&Header), sizeof(PCX_HEADER));

	//XXXXXXXXXXXX

	if(!DataStream.Save(Buffer)) return FALSE;

	return TRUE;
	*/
}


//+-----------------------------------------------------------------------------
//| Loads a texture from a buffer
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_PCX::Load(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer)
{
	INT i;
	INT X;
	INT Y;
	INT P;
	INT Index;
	INT Width;
	INT Height;
	INT Repeat;
	INT TotalBytes;
	BYTE TempByte;
	UCHAR* Pointer;
	PCX_LINE Line;
	PCX_HEADER Header;
	DATA_IN_STREAM DataStream;
	D3DLOCKED_RECT LockedRect;
	D3DSURFACE_DESC SurfaceInfo;
	LPDIRECT3DDEVICE9 Direct3DDevice;
	std::vector<std::vector<BYTE> > ScanLine;
	std::list<PCX_LINE> LineList;
	std::list<PCX_LINE>::iterator LineListIterator;
	std::vector<BYTE> Palette;

	CurrentFileName = FileName;
	DataStream.SetFileName(FileName);

	if(!DataStream.Load(Buffer)) return FALSE;

	if(!DataStream.Read(reinterpret_cast<CHAR*>(&Header), sizeof(PCX_HEADER))) return FALSE;

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unable to retrieve the Direct3D device!");
		return FALSE;
	}

	if(Header.BitsPerPixel != 8)
	{
		std::stringstream Stream;

		Stream << "The image \"" << FileName << "\" has " << static_cast<INT>(Header.BitsPerPixel) << " bits per pixel, this loader only supports 8!";

		Error.SetMessage(Stream.str());
		return FALSE;
	}

	if((Header.NrOfPlanes != 1) && (Header.NrOfPlanes != 3))
	{
		std::stringstream Stream;

		Stream << "The image \"" << FileName << "\" contains " << static_cast<INT>(Header.NrOfPlanes) << " planes, this loader only supports 1 or 3!";

		Error.SetMessage(Stream.str());
		return FALSE;
	}

	Width = Header.MaxX - Header.MinX + 1;
	Height = Header.MaxY - Header.MinY + 1;
	TotalBytes = Header.NrOfPlanes * Header.BytesPerLine;

	Line.I.reserve(Width);
	Line.R.reserve(Width);
	Line.G.reserve(Width);
	Line.B.reserve(Width);
	Line.A.reserve(Width);

	ScanLine.resize(Header.NrOfPlanes);
	for(i = 0; i < Header.NrOfPlanes; i++)
	{
		ScanLine[i].reserve(Width * 2);
	}

	for(Y = 0; Y < Height; Y++)
	{
		Line.I.clear();
		Line.R.clear();
		Line.G.clear();
		Line.B.clear();
		Line.A.clear();

		for(P = 0; P < Header.NrOfPlanes; P++)
		{
			ScanLine[P].clear();

			X = 0;
			while(X < Header.BytesPerLine)
			{
				TempByte = DataStream.ReadByte();
				if((TempByte & 0xC0) == 0xC0)
				{
					Repeat = static_cast<INT>(TempByte & 0x3F);
					TempByte = DataStream.ReadByte();
				}
				else
				{
					Repeat = 1;
				}

				for(i = 0; i < Repeat; i++)
				{
					ScanLine[P].push_back(TempByte);
					X++;
				}
			}
		}

		for(i = 0; i < Width; i++)
		{
			if(Header.NrOfPlanes == 1)
			{
				Line.I.push_back((ScanLine[0])[i]);
				Line.R.push_back(255);
				Line.G.push_back(255);
				Line.B.push_back(255);
				Line.A.push_back(255);
			}
			else
			{
				Line.R.push_back((ScanLine[0])[i]);
				Line.G.push_back((ScanLine[1])[i]);
				Line.B.push_back((ScanLine[2])[i]);
				Line.A.push_back(255);
			}
		}

		LineList.push_back(Line);
	}

	if(Header.NrOfPlanes == 1)
	{
		if(Header.Version >= 5)
		{
			DataStream.SetPosition(-768, TRUE);

			Palette.resize(768);
			DataStream.Read(reinterpret_cast<CHAR*>(&Palette[0]), 768);
		}
		else
		{
			Palette.resize(48);
			memcpy(reinterpret_cast<CHAR*>(&Palette[0]), reinterpret_cast<CHAR*>(&Header.Palette[0]), 48);
		}
	}

	if(FAILED(D3DXCreateTexture(Direct3DDevice, Width, Height, D3DX_DEFAULT, 0,
								Graphics.GetTextureFormat(), D3DPOOL_MANAGED, &Texture.Texture)))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", texture creation failed!");
		return FALSE;
	}

	if(FAILED(Texture.Texture->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD)))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", texture locking failed!");
		return FALSE;
	}

	Index = 0;
	LineListIterator = LineList.begin();
	Pointer = reinterpret_cast<UCHAR*>(LockedRect.pBits);

	for(Y = 0; Y < Height; Y++)
	{
		if(LineListIterator == LineList.end()) break;

		for(X = 0; X < Width; X++)
		{
			if(Header.NrOfPlanes == 1)
			{
				Pointer[Index++] = Palette[(3 * LineListIterator->I[X]) + 2];
				Pointer[Index++] = Palette[(3 * LineListIterator->I[X]) + 1];
				Pointer[Index++] = Palette[(3 * LineListIterator->I[X]) + 0];
				Pointer[Index++] = 255;
			}
			else
			{
				Pointer[Index++] = LineListIterator->B[X];
				Pointer[Index++] = LineListIterator->G[X];
				Pointer[Index++] = LineListIterator->R[X];
				Pointer[Index++] = LineListIterator->A[X];
			}
		}

		Index += LockedRect.Pitch - (Width * 4);
		LineListIterator++;
	}

	Texture.Texture->UnlockRect(0);

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
	Texture.Width = Width;
	Texture.Height = Height;
	Texture.RealWidth = SurfaceInfo.Width;
	Texture.RealHeight = SurfaceInfo.Height;

	Texture.Attach();

	return TRUE;
}

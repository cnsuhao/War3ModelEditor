//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TEXTURE_LOADER_PCX_H
#define MAGOS_TEXTURE_LOADER_PCX_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "TextureLoader.h"


//+-----------------------------------------------------------------------------
//| Pcx header structure
//+-----------------------------------------------------------------------------
struct PCX_HEADER
{
	PCX_HEADER()
	{
		Manufacturer = 10;
		Version = 5;
		Encoding = 1;
		BitsPerPixel = 8;

		MinX = 0;
		MinY = 0;
		MaxX = 0;
		MaxY = 0;
		ResolutionX = 300;
		ResolutionY = 300;

		ZeroMemory(reinterpret_cast<CHAR*>(&Palette[0]), 48);

		Reserved1 = 0;
		NrOfPlanes = 0;
		BytesPerLine = 0;
		PaletteInfo = 1;
		Width = 0;
		Height = 0;

		ZeroMemory(reinterpret_cast<CHAR*>(&Reserved2[0]), 54);
	}

	BYTE Manufacturer;
	BYTE Version;
	BYTE Encoding;
	BYTE BitsPerPixel;

	WORD MinX;
	WORD MinY;
	WORD MaxX;
	WORD MaxY;
	WORD ResolutionX;
	WORD ResolutionY;

	BYTE Palette[48];

	BYTE Reserved1;
	BYTE NrOfPlanes;
	WORD BytesPerLine;
	WORD PaletteInfo;
	WORD Width;
	WORD Height;

	BYTE Reserved2[54];
};


//+-----------------------------------------------------------------------------
//| Pcx line structure
//+-----------------------------------------------------------------------------
struct PCX_LINE
{
	PCX_LINE()
	{
		//Empty
	}

	std::vector<BYTE> I;
	std::vector<BYTE> R;
	std::vector<BYTE> G;
	std::vector<BYTE> B;
	std::vector<BYTE> A;
};


//+-----------------------------------------------------------------------------
//| Texture loader pcx class
//+-----------------------------------------------------------------------------
class TEXTURE_LOADER_PCX : public TEXTURE_LOADER
{
	public:
		CONSTRUCTOR TEXTURE_LOADER_PCX();
		DESTRUCTOR ~TEXTURE_LOADER_PCX();

		virtual BOOL Save(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer);
		virtual BOOL Load(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer);

	protected:
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern TEXTURE_LOADER_PCX TextureLoaderPcx;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TEXTURE_LOADER_BLP_H
#define MAGOS_TEXTURE_LOADER_BLP_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "TextureLoader.h"


//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST INT MAX_NR_OF_BLP_MIP_MAPS = 16;


//+-----------------------------------------------------------------------------
//| Blp header structure
//+-----------------------------------------------------------------------------
struct BLP_HEADER
{
	BLP_HEADER()
	{
		MagicNumber = '1PLB';
		Compression = 0;
		Flags = 0;
		Width = 0;
		Height = 0;
		PictureType = 0;
		PictureSubType = 0;
		ZeroMemory(Offset, MAX_NR_OF_BLP_MIP_MAPS * sizeof(DWORD));
		ZeroMemory(Size, MAX_NR_OF_BLP_MIP_MAPS * sizeof(DWORD));
	}

	DWORD MagicNumber;
	DWORD Compression;
	DWORD Flags;
	DWORD Width;
	DWORD Height;
	DWORD PictureType;
	DWORD PictureSubType;
	DWORD Offset[MAX_NR_OF_BLP_MIP_MAPS];
	DWORD Size[MAX_NR_OF_BLP_MIP_MAPS];
};


//+-----------------------------------------------------------------------------
//| Blp RGBA structure
//+-----------------------------------------------------------------------------
struct BLP_RGBA
{
	UCHAR Red;
	UCHAR Green;
	UCHAR Blue;
	UCHAR Alpha;
};


//+-----------------------------------------------------------------------------
//| Blp pixel structure
//+-----------------------------------------------------------------------------
struct BLP_PIXEL
{
	UCHAR Index;
};


//+-----------------------------------------------------------------------------
//| Texture loader blp class
//+-----------------------------------------------------------------------------
class TEXTURE_LOADER_BLP : public TEXTURE_LOADER
{
	public:
		CONSTRUCTOR TEXTURE_LOADER_BLP();
		DESTRUCTOR ~TEXTURE_LOADER_BLP();

		virtual BOOL Save(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer);
		virtual BOOL Load(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer);

	protected:
		static BOOL LoadCompressed(TEXTURE& Texture, BLP_HEADER& Header, BUFFER& Buffer);
		static BOOL LoadUncompressed(TEXTURE& Texture, BLP_HEADER& Header, BUFFER& Buffer);
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern TEXTURE_LOADER_BLP TextureLoaderBlp;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

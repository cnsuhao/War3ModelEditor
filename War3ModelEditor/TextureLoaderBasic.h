//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TEXTURE_LOADER_BASIC_H
#define MAGOS_TEXTURE_LOADER_BASIC_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "TextureLoader.h"


//+-----------------------------------------------------------------------------
//| Texture loader basic class
//+-----------------------------------------------------------------------------
class TEXTURE_LOADER_BASIC : public TEXTURE_LOADER
{
	public:
		CONSTRUCTOR TEXTURE_LOADER_BASIC();
		DESTRUCTOR ~TEXTURE_LOADER_BASIC();

		virtual BOOL Save(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer);
		virtual BOOL Load(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer);

	protected:
		VOID RegisterFileFormat(CONST std::string& Extention, D3DXIMAGE_FILEFORMAT FileFormat);

		std::map<std::string, D3DXIMAGE_FILEFORMAT> FileFormatMap;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern TEXTURE_LOADER_BASIC TextureLoaderBasic;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

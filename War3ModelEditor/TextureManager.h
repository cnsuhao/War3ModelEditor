//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TEXTURE_MANAGER_H
#define MAGOS_TEXTURE_MANAGER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class TEXTURE;


//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST INT NR_OF_REPLACEABLE_TEXTURES = 13;


//+-----------------------------------------------------------------------------
//| Texture manager class
//+-----------------------------------------------------------------------------
class TEXTURE_MANAGER
{
	public:
		CONSTRUCTOR TEXTURE_MANAGER();
		DESTRUCTOR ~TEXTURE_MANAGER();

		VOID Clear();

		BOOL LoadAllReplaceableTextures();
		VOID UnloadAllReplaceableTextures();

		BOOL Load(CONST std::string& FileName);
		BOOL Unload(CONST std::string& FileName);
		BOOL Import(CONST std::string& FileName, CONST std::string& RealFileName);
		BOOL Export(CONST std::string& FileName, CONST std::string& RealFileName);

		BOOL ResizeTexture(CONST std::string& FileName, CONST std::string& NewFileName, INT NewWidth, INT NewHeight);

		TEXTURE* GetTexture(CONST std::string& FileName);
		TEXTURE* GetReplaceableTexture(INT ReplaceableId);

	protected:
		TEXTURE* GetTeamColorTexture();
		TEXTURE* GetTeamGlowTexture();

		TEXTURE* InternalLoad(CONST std::string& FileName);
		std::string MakeTwoDigitNumber(INT Number);

		TEXTURE* InvalidTexture;
		TEXTURE* ReplaceableTexture11;
		TEXTURE* ReplaceableTexture31;
		TEXTURE* ReplaceableTexture32;
		TEXTURE* ReplaceableTexture33;
		TEXTURE* ReplaceableTexture34;
		TEXTURE* ReplaceableTexture35;
		TEXTURE* TeamColorTexture[NR_OF_REPLACEABLE_TEXTURES];
		TEXTURE* TeamGlowTexture[NR_OF_REPLACEABLE_TEXTURES];

		CONTAINER<TEXTURE*> TextureContainer;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern TEXTURE_MANAGER TextureManager;


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "ResourceLoader.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

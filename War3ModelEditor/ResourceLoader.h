//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_RESOURCE_LOADER_H
#define MAGOS_RESOURCE_LOADER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class MODEL;
class MODEL_LOADER;
class TEXTURE;
class TEXTURE_LOADER;


//+-----------------------------------------------------------------------------
//| Resource loader class
//+-----------------------------------------------------------------------------
class RESOURCE_LOADER
{
	public:
		CONSTRUCTOR RESOURCE_LOADER();
		DESTRUCTOR ~RESOURCE_LOADER();

		VOID RegisterAllLoaders();
		VOID ClearAllLoaders();

		BOOL IsModelExtention(CONST std::string& Extention) CONST;
		BOOL IsTextureExtention(CONST std::string& Extention) CONST;

		BOOL SaveModel(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer) CONST;
		BOOL LoadModel(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer) CONST;

		BOOL SaveTexture(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer) CONST;
		BOOL LoadTexture(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer) CONST;

	protected:
		VOID RegisterModelLoader(CONST std::string& Extention, MODEL_LOADER* ModelLoader);
		VOID RegisterTextureLoader(CONST std::string& Extention, TEXTURE_LOADER* TextureLoader);

		std::map<std::string, MODEL_LOADER*> ModelLoaderMap;
		std::map<std::string, TEXTURE_LOADER*> TextureLoaderMap;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern RESOURCE_LOADER ResourceLoader;


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "ModelLoader.h"
#include "TextureLoader.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

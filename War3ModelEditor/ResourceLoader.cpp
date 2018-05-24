//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ResourceLoader.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
RESOURCE_LOADER ResourceLoader;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
RESOURCE_LOADER::RESOURCE_LOADER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
RESOURCE_LOADER::~RESOURCE_LOADER()
{
	ClearAllLoaders();
}


//+-----------------------------------------------------------------------------
//| Registers all loaders
//+-----------------------------------------------------------------------------
VOID RESOURCE_LOADER::RegisterAllLoaders()
{
	ClearAllLoaders();

	RegisterModelLoader("mdl", &ModelLoaderMdl);
	RegisterModelLoader("mdx", &ModelLoaderMdx);

	RegisterTextureLoader("bmp", &TextureLoaderBasic);
	RegisterTextureLoader("tga", &TextureLoaderBasic);
	RegisterTextureLoader("png", &TextureLoaderBasic);
	RegisterTextureLoader("jpg", &TextureLoaderBasic);
	RegisterTextureLoader("jpeg", &TextureLoaderBasic);
	RegisterTextureLoader("pcx", &TextureLoaderPcx);
	RegisterTextureLoader("blp", &TextureLoaderBlp);
}


//+-----------------------------------------------------------------------------
//| Clears all loaders
//+-----------------------------------------------------------------------------
VOID RESOURCE_LOADER::ClearAllLoaders()
{
	ModelLoaderMap.clear();
	TextureLoaderMap.clear();
}


//+-----------------------------------------------------------------------------
//| Checks if an extention is a model extention
//+-----------------------------------------------------------------------------
BOOL RESOURCE_LOADER::IsModelExtention(CONST std::string& Extention) CONST
{
	std::map<std::string, MODEL_LOADER*>::const_iterator i;

	i = ModelLoaderMap.find(Common.LowerCase(Extention));
	if(i == ModelLoaderMap.end()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Checks if an extention is a texture extention
//+-----------------------------------------------------------------------------
BOOL RESOURCE_LOADER::IsTextureExtention(CONST std::string& Extention) CONST
{
	std::map<std::string, TEXTURE_LOADER*>::const_iterator i;

	i = TextureLoaderMap.find(Common.LowerCase(Extention));
	if(i == TextureLoaderMap.end()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model
//+-----------------------------------------------------------------------------
BOOL RESOURCE_LOADER::SaveModel(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer) CONST
{
	std::string Extention;
	std::map<std::string, MODEL_LOADER*>::const_iterator i;

	Extention = Common.LowerCase(Common.GetExtention(FileName));
	i = ModelLoaderMap.find(Extention);
	if(i == ModelLoaderMap.end())
	{
		Error.SetMessage("Unable to save \"" + FileName + "\", unknown extention \"" + Extention + "\"!");
		return FALSE;
	}

	return i->second->Save(Model, FileName, Buffer);
}


//+-----------------------------------------------------------------------------
//| Loads a model
//+-----------------------------------------------------------------------------
BOOL RESOURCE_LOADER::LoadModel(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer) CONST
{
	std::string Extention;
	std::map<std::string, MODEL_LOADER*>::const_iterator i;

	Model.Clear();

	Extention = Common.LowerCase(Common.GetExtention(FileName));
	i = ModelLoaderMap.find(Extention);
	if(i == ModelLoaderMap.end())
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", unknown extention \"" + Extention + "\"!");
		return FALSE;
	}

	return i->second->Load(Model, FileName, Buffer);
}


//+-----------------------------------------------------------------------------
//| Saves a texture
//+-----------------------------------------------------------------------------
BOOL RESOURCE_LOADER::SaveTexture(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer) CONST
{
	std::string Extention;
	std::map<std::string, TEXTURE_LOADER*>::const_iterator i;

	Extention = Common.LowerCase(Common.GetExtention(FileName));
	i = TextureLoaderMap.find(Extention);
	if(i == TextureLoaderMap.end())
	{
		Error.SetMessage("Unable to save \"" + FileName + "\", unknown extention \"" + Extention + "\"!");
		return FALSE;
	}

	return i->second->Save(Texture, FileName, Buffer);
}


//+-----------------------------------------------------------------------------
//| Loads a texture
//+-----------------------------------------------------------------------------
BOOL RESOURCE_LOADER::LoadTexture(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer) CONST
{
	std::string Extention;
	std::map<std::string, TEXTURE_LOADER*>::const_iterator i;

	Texture.Clear();

	Extention = Common.LowerCase(Common.GetExtention(FileName));
	i = TextureLoaderMap.find(Extention);
	if(i == TextureLoaderMap.end())
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", unknown extention \"" + Extention + "\"!");
		return FALSE;
	}

	return i->second->Load(Texture, FileName, Buffer);
}


//+-----------------------------------------------------------------------------
//| Registers a model loader
//+-----------------------------------------------------------------------------
VOID RESOURCE_LOADER::RegisterModelLoader(CONST std::string& Extention, MODEL_LOADER* ModelLoader)
{
	ModelLoaderMap.insert(std::make_pair(Common.LowerCase(Extention), ModelLoader));
}


//+-----------------------------------------------------------------------------
//| Registers a texture loader
//+-----------------------------------------------------------------------------
VOID RESOURCE_LOADER::RegisterTextureLoader(CONST std::string& Extention, TEXTURE_LOADER* TextureLoader)
{
	TextureLoaderMap.insert(std::make_pair(Common.LowerCase(Extention), TextureLoader));
}

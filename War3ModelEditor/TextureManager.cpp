//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TextureManager.h"
#include "TextureManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEXTURE_MANAGER TextureManager;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEXTURE_MANAGER::TEXTURE_MANAGER()
{
	INT i;

	InvalidTexture = NULL;
	ReplaceableTexture11 = NULL;
	ReplaceableTexture31 = NULL;
	ReplaceableTexture32 = NULL;
	ReplaceableTexture33 = NULL;
	ReplaceableTexture34 = NULL;
	ReplaceableTexture35 = NULL;

	for(i = 0; i < NR_OF_REPLACEABLE_TEXTURES; i++)
	{
		TeamColorTexture[i] = NULL;
		TeamGlowTexture[i] = NULL;
	}
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEXTURE_MANAGER::~TEXTURE_MANAGER()
{
	Clear();
	UnloadAllReplaceableTextures();
}


//+-----------------------------------------------------------------------------
//| Clears all textures
//+-----------------------------------------------------------------------------
VOID TEXTURE_MANAGER::Clear()
{
	INT i;

	for(i = 0; i < TextureContainer.GetTotalSize(); i++)
	{
		if(TextureContainer.ValidIndex(i))
		{
			delete TextureContainer[i];
		}
	}

	TextureContainer.Clear();
	TextureManagerWindow.ClearAllNames();
	TextureManagerWindow.HideTextureWindow();
}


//+-----------------------------------------------------------------------------
//| Clears all textures includign the replaceable textures
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER::LoadAllReplaceableTextures()
{
	INT i;
	std::string Digit;

	InvalidTexture = InternalLoad("Textures\\BTNtempW.blp");
	if(InvalidTexture == NULL) return FALSE;

	ReplaceableTexture11 = InternalLoad(Properties().ReplaceableTexture11);
	if(ReplaceableTexture11 == NULL) return FALSE;

	ReplaceableTexture31 = InternalLoad(Properties().ReplaceableTexture31);
	if(ReplaceableTexture31 == NULL) return FALSE;

	ReplaceableTexture32 = InternalLoad(Properties().ReplaceableTexture32);
	if(ReplaceableTexture32 == NULL) return FALSE;

	ReplaceableTexture33 = InternalLoad(Properties().ReplaceableTexture33);
	if(ReplaceableTexture33 == NULL) return FALSE;

	ReplaceableTexture34 = InternalLoad(Properties().ReplaceableTexture34);
	if(ReplaceableTexture34 == NULL) return FALSE;

	ReplaceableTexture35 = InternalLoad(Properties().ReplaceableTexture35);
	if(ReplaceableTexture35 == NULL) return FALSE;

	for(i = 0; i < NR_OF_REPLACEABLE_TEXTURES; i++)
	{
		Digit = MakeTwoDigitNumber(i);

		TeamColorTexture[i] = InternalLoad(PATH_TEAM_COLOR + "TeamColor" + Digit + ".blp");
		if(TeamColorTexture[i] == NULL) return FALSE;

		TeamGlowTexture[i] = InternalLoad(PATH_TEAM_GLOW + "TeamGlow" + Digit + ".blp");
		if(TeamGlowTexture[i] == NULL) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Clears all textures except for the replaceable textures
//+-----------------------------------------------------------------------------
VOID TEXTURE_MANAGER::UnloadAllReplaceableTextures()
{
	INT i;

	SAFE_DELETE(InvalidTexture);
	SAFE_DELETE(ReplaceableTexture11);
	SAFE_DELETE(ReplaceableTexture31);
	SAFE_DELETE(ReplaceableTexture32);
	SAFE_DELETE(ReplaceableTexture33);
	SAFE_DELETE(ReplaceableTexture34);
	SAFE_DELETE(ReplaceableTexture35);

	for(i = 0; i < NR_OF_REPLACEABLE_TEXTURES; i++)
	{
		SAFE_DELETE(TeamColorTexture[i]);
		SAFE_DELETE(TeamGlowTexture[i]);
	}
}


//+-----------------------------------------------------------------------------
//| Loads a texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER::Load(CONST std::string& FileName)
{
	INT Index;
	BUFFER Buffer;
	TEXTURE* Texture;

	Index = TextureContainer.GetIndex(FileName);
	if(Index != INVALID_INDEX) return TRUE;

	Texture = InternalLoad(FileName);
	if(Texture == NULL) return FALSE;

	if(!TextureContainer.Add(FileName, Texture))
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", unable to add texture!");
		delete Texture;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Unloads a texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER::Unload(CONST std::string& FileName)
{
	INT Index;

	Index = TextureContainer.GetIndex(FileName);
	if(Index == INVALID_INDEX) return TRUE;

	TextureContainer.Remove(Index);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Imports a texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER::Import(CONST std::string& FileName, CONST std::string& RealFileName)
{
	INT Index;
	BUFFER Buffer;
	TEXTURE* Texture;

	Index = TextureContainer.GetIndex(FileName);
	if(Index != INVALID_INDEX)
	{
		Error.SetMessage("Unable to import \"" + RealFileName + "\", file already exists!");
		return FALSE;
	}

	Texture = new TEXTURE();
	if(Texture == NULL)
	{
		Error.SetMessage("Unable to import \"" + RealFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!FileLoader.LoadFromFile(RealFileName, Buffer))
	{
		delete Texture;
		return FALSE;
	}

	if(!ResourceLoader.LoadTexture((*Texture), RealFileName, Buffer))
	{
		delete Texture;
		return FALSE;
	}

	if(!TextureContainer.Add(FileName, Texture))
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", unable to add texture!");
		delete Texture;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Exports a texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER::Export(CONST std::string& FileName, CONST std::string& RealFileName)
{
	INT Index;
	BUFFER Buffer;
	TEXTURE* Texture;

	Index = TextureContainer.GetIndex(FileName);
	if(Index == INVALID_INDEX)
	{
		Error.SetMessage("Unable to export \"" + FileName + "\", file does not exist!");
		return FALSE;
	}

	Texture = TextureContainer[Index];

	if(!ResourceLoader.SaveTexture((*Texture), RealFileName, Buffer)) return FALSE;
	if(!FileLoader.SaveToFile(RealFileName, Buffer)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Resizes the texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER::ResizeTexture(CONST std::string& FileName, CONST std::string& NewFileName, INT NewWidth, INT NewHeight)
{
	BUFFER Buffer;
	TEXTURE Source;
	TEXTURE Target;
	RECT SourceRect;
	RECT TargetRect;
	LPDIRECT3DTEXTURE9 SourceTexture;
	LPDIRECT3DTEXTURE9 TargetTexture;
	LPDIRECT3DSURFACE9 SourceSurface;
	LPDIRECT3DSURFACE9 TargetSurface;

	if(FileName == NewFileName)
	{
		Error.SetMessage("Unable to save \"" + NewFileName + "\", target file is the same as source file!");
		return FALSE;
	}

	if(!FileLoader.LoadFromFile(FileName, Buffer)) return FALSE;
	if(!ResourceLoader.LoadTexture(Source, FileName, Buffer)) return FALSE;
	if(!Target.CreateEmpty(NewWidth, NewHeight)) return FALSE;

	SourceTexture = Source.GetTexture();
	if(SourceTexture == NULL)
	{
		Error.SetMessage("Unable to resize \"" + NewFileName + "\", no source texture!");
		return FALSE;
	}

	TargetTexture = Target.GetTexture();
	if(TargetTexture == NULL)
	{
		Error.SetMessage("Unable to resize \"" + NewFileName + "\", no target texture!");
		return FALSE;
	}

	if(FAILED(SourceTexture->GetSurfaceLevel(0, &SourceSurface)))
	{
		Error.SetMessage("Unable to resize \"" + NewFileName + "\", no source surface!");
		return FALSE;
	}

	if(FAILED(TargetTexture->GetSurfaceLevel(0, &TargetSurface)))
	{
		SAFE_RELEASE(SourceSurface);
		Error.SetMessage("Unable to resize \"" + NewFileName + "\", no target surface!");
		return FALSE;
	}

	SourceRect.left = 0;
	SourceRect.top = 0;
	SourceRect.right = Source.GetWidth();
	SourceRect.bottom = Source.GetHeight();

	TargetRect.left = 0;
	TargetRect.top = 0;
	TargetRect.right = NewWidth;
	TargetRect.bottom = NewHeight;

	if(FAILED(D3DXLoadSurfaceFromSurface(TargetSurface, NULL, &TargetRect, SourceSurface, NULL, &SourceRect, D3DX_DEFAULT, 0)))
	{
		SAFE_RELEASE(SourceSurface);
		SAFE_RELEASE(TargetSurface);
		Error.SetMessage("Unable to resize \"" + NewFileName + "\", resizing failed!");
		return FALSE;
	}

	SAFE_RELEASE(SourceSurface);
	SAFE_RELEASE(TargetSurface);

	if(FAILED(D3DXFilterTexture(TargetTexture, NULL, D3DX_DEFAULT, D3DX_DEFAULT)))
	{
		Error.SetMessage("Unable to resize \"" + NewFileName + "\", texture filtering failed!");
		return FALSE;
	}

	if(!ResourceLoader.SaveTexture(Target, NewFileName, Buffer)) return FALSE;
	if(!FileLoader.SaveToFile(NewFileName, Buffer)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns a texture
//+-----------------------------------------------------------------------------
TEXTURE* TEXTURE_MANAGER::GetTexture(CONST std::string& FileName)
{
	INT Index;

	Index = TextureContainer.GetIndex(FileName);
	if(Index == INVALID_INDEX) return InvalidTexture;

	return TextureContainer[Index];
}


//+-----------------------------------------------------------------------------
//| Returns a replaceable texture
//+-----------------------------------------------------------------------------
TEXTURE* TEXTURE_MANAGER::GetReplaceableTexture(INT ReplaceableId)
{
	TEXTURE* Texture;

	switch(ReplaceableId)
	{
	case 1:
		{
			Texture = GetTeamColorTexture();
			if(Texture == NULL) break;

			return Texture;
		}

	case 2:
		{
			Texture = GetTeamGlowTexture();
			if(Texture == NULL) break;

			return Texture;
		}

	case 11:
		{
			return ReplaceableTexture11;
		}

	case 31:
		{
			return ReplaceableTexture31;
		}

	case 32:
		{
			return ReplaceableTexture32;
		}

	case 33:
		{
			return ReplaceableTexture33;
		}

	case 34:
		{
			return ReplaceableTexture34;
		}

	case 35:
		{
			return ReplaceableTexture35;
		}
	}

	return InvalidTexture;
}


//+-----------------------------------------------------------------------------
//| Returns a team color texture
//+-----------------------------------------------------------------------------
TEXTURE* TEXTURE_MANAGER::GetTeamColorTexture()
{
	INT Index;

	Index = TeamColor.GetCurrentTeamColor();

	if(Index < 0) return NULL;
	if(Index >= NR_OF_REPLACEABLE_TEXTURES) return NULL;

	return TeamColorTexture[Index];
}


//+-----------------------------------------------------------------------------
//| Returns a team glow texture
//+-----------------------------------------------------------------------------
TEXTURE* TEXTURE_MANAGER::GetTeamGlowTexture()
{
	INT Index;

	Index = TeamColor.GetCurrentTeamColor();

	if(Index < 0) return NULL;
	if(Index >= NR_OF_REPLACEABLE_TEXTURES) return NULL;

	return TeamGlowTexture[Index];
}


//+-----------------------------------------------------------------------------
//| Loads a texture
//+-----------------------------------------------------------------------------
TEXTURE* TEXTURE_MANAGER::InternalLoad(CONST std::string& FileName)
{
	BUFFER Buffer;
	TEXTURE* Texture;

	Texture = new TEXTURE();
	if(Texture == NULL)
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", memory allocation failed!");
		return NULL;
	}

	if(!FileLoader.Load(FileName, Buffer))
	{
		delete Texture;
		return NULL;
	}

	if(!ResourceLoader.LoadTexture((*Texture), FileName, Buffer))
	{
		delete Texture;
		return NULL;
	}

	return Texture;
}


//+-----------------------------------------------------------------------------
//| Builds a 2-digit number as a string
//+-----------------------------------------------------------------------------
std::string TEXTURE_MANAGER::MakeTwoDigitNumber(INT Number)
{
	std::stringstream Stream;

	Stream << (Number / 10);
	Stream << (Number % 10);

	return Stream.str();
}

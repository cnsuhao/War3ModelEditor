//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_PROPERTIES_H
#define MAGOS_PROPERTIES_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Minor properties info structure
//+-----------------------------------------------------------------------------
struct MINOR_PROPERTIES_INFO
{
	MINOR_PROPERTIES_INFO()
	{
		AlwaysLoop = FALSE;
		NeverLoop = FALSE;
	}

	BOOL AlwaysLoop;
	BOOL NeverLoop;
};


//+-----------------------------------------------------------------------------
//| Properties info structure
//+-----------------------------------------------------------------------------
struct PROPERTIES_INFO
{
	PROPERTIES_INFO()
	{
		Background = PROPERTIES_DEFAULT_BACKGROUND;

		Ambient = PROPERTIES_DEFAULT_AMBIENT;
		Diffuse = PROPERTIES_DEFAULT_DIFFUSE;
		Specular = PROPERTIES_DEFAULT_SPECULAR;
		Power = PROPERTIES_DEFAULT_SPECULAR_POWER;

		FillMode = D3DFILL_SOLID;

		NearDistance = GRAPHICS_DEFAULT_NEAR_DISTANCE;
		FarDistance = GRAPHICS_DEFAULT_FAR_DISTANCE;

		UseLighting = TRUE;
		UseShaders = TRUE;
		RelativeZoom = TRUE;
		RelativeMove = TRUE;
		ClockwiseCulling = TRUE;
		UseGroundTexture = FALSE;

		GroundTextureScale = DEFAULT_GROUND_TEXTURE_SCALE;

		ShowDebug = FALSE;
		ShowAxises = TRUE;
		ShowLights = FALSE;
		ShowBones = FALSE;
		ShowCameras = FALSE;
		ShowAttachments = FALSE;
		ShowCollisionShapes = FALSE;

		BlpQuality = DEFAULT_BLP_QUALITY;

		GroundTexture = PATH_GROUND_TEXTURE;
		ReplaceableTexture11 = PATH_REPLACEABLE_TEXTURE_11;
		ReplaceableTexture31 = PATH_REPLACEABLE_TEXTURE_31;
		ReplaceableTexture32 = PATH_REPLACEABLE_TEXTURE_32;
		ReplaceableTexture33 = PATH_REPLACEABLE_TEXTURE_33;
		ReplaceableTexture34 = PATH_REPLACEABLE_TEXTURE_34;
		ReplaceableTexture35 = PATH_REPLACEABLE_TEXTURE_35;
	}

	D3DCOLOR Background;

	D3DCOLOR Ambient;
	D3DCOLOR Diffuse;
	D3DCOLOR Specular;
	FLOAT Power;

	DWORD FillMode;

	FLOAT NearDistance;
	FLOAT FarDistance;

	BOOL UseLighting;
	BOOL UseShaders;
	BOOL RelativeZoom;
	BOOL RelativeMove;
	BOOL ClockwiseCulling;
	BOOL UseGroundTexture;

	FLOAT GroundTextureScale;

	BOOL ShowDebug;
	BOOL ShowAxises;
	BOOL ShowLights;
	BOOL ShowBones;
	BOOL ShowCameras;
	BOOL ShowAttachments;
	BOOL ShowCollisionShapes;

	INT BlpQuality;

	std::string GroundTexture;
	std::string ReplaceableTexture11;
	std::string ReplaceableTexture31;
	std::string ReplaceableTexture32;
	std::string ReplaceableTexture33;
	std::string ReplaceableTexture34;
	std::string ReplaceableTexture35;

	MINOR_PROPERTIES_INFO Minor;
};


//+-----------------------------------------------------------------------------
//| Properties class
//+-----------------------------------------------------------------------------
class PROPERTIES
{
	public:
		CONSTRUCTOR PROPERTIES();
		DESTRUCTOR ~PROPERTIES();

		BOOL SavePropertiesInfo(CONST std::string& FileName);
		BOOL LoadPropertiesInfo(CONST std::string& FileName);

		VOID SetPropertiesInfo(CONST PROPERTIES_INFO& NewPropertiesInfo);
		PROPERTIES_INFO GetPropertiesInfo() CONST;

		CONST PROPERTIES_INFO& operator ()();

		BOOL ShadersAvailable();
		VOID MakeShadersAvailable(BOOL Available);

	protected:
		std::string BoolToString(BOOL Bool) CONST;
		std::string ColorToString(CONST D3DCOLOR& Color) CONST;
		std::string FillModeToString(DWORD FillMode) CONST;

		VOID LoadBool(std::ifstream& File, BOOL& Bool) CONST;
		VOID LoadColor(std::ifstream& File, D3DCOLOR& Color) CONST;
		VOID LoadFillMode(std::ifstream& File, DWORD& FillMode) CONST;

		PROPERTIES_INFO PropertiesInfo;

		BOOL ShadersIsAvailable;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern PROPERTIES Properties;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

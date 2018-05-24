//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Properties.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
PROPERTIES Properties;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
PROPERTIES::PROPERTIES()
{
	ShadersIsAvailable = FALSE;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
PROPERTIES::~PROPERTIES()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Saves the properties to a file
//+-----------------------------------------------------------------------------
BOOL PROPERTIES::SavePropertiesInfo(CONST std::string& FileName)
{
	std::ofstream File;
	std::string PropertiesFileName;

	PropertiesFileName = Common.GetProgramDirectory() + "\\" + FileName;

	File.open(PropertiesFileName.c_str(), std::ios::out);
	if(File.fail())
	{
		Error.SetMessage("Unable to save \"" + PropertiesFileName + "\"!");
		return FALSE;
	}

	File << "Background " << ColorToString(PropertiesInfo.Background) << "\n";
	File << "Ambient " << ColorToString(PropertiesInfo.Ambient) << "\n";
	File << "Diffuse " << ColorToString(PropertiesInfo.Diffuse) << "\n";
	File << "Specular " << ColorToString(PropertiesInfo.Specular) << "\n";
	File << "Power " << PropertiesInfo.Power << "\n";

	File << "FillMode " << FillModeToString(PropertiesInfo.FillMode) << "\n";

	File << "NearDistance " << PropertiesInfo.NearDistance << "\n";
	File << "FarDistance " << PropertiesInfo.FarDistance << "\n";

	File << "UseLighting " << BoolToString(PropertiesInfo.UseLighting) << "\n";
	File << "UseShaders " << BoolToString(PropertiesInfo.UseShaders) << "\n";
	File << "RelativeZoom " << BoolToString(PropertiesInfo.RelativeZoom) << "\n";
	File << "RelativeMove " << BoolToString(PropertiesInfo.RelativeMove) << "\n";
	File << "ClockwiseCulling " << BoolToString(PropertiesInfo.ClockwiseCulling) << "\n";
	File << "UseGroundTexture " << BoolToString(PropertiesInfo.UseGroundTexture) << "\n";

	File << "GroundTextureScale " << PropertiesInfo.GroundTextureScale << "\n";

	File << "ShowDebug " << BoolToString(PropertiesInfo.ShowDebug) << "\n";
	File << "ShowAxises " << BoolToString(PropertiesInfo.ShowAxises) << "\n";
	File << "ShowLights " << BoolToString(PropertiesInfo.ShowLights) << "\n";
	File << "ShowBones " << BoolToString(PropertiesInfo.ShowBones) << "\n";
	File << "ShowCameras " << BoolToString(PropertiesInfo.ShowCameras) << "\n";
	File << "ShowAttachments " << BoolToString(PropertiesInfo.ShowAttachments) << "\n";
	File << "ShowCollisionShapes " << BoolToString(PropertiesInfo.ShowCollisionShapes) << "\n";

	File << "BlpQuality " << PropertiesInfo.BlpQuality << "\n";

	File << "GroundTexture " << PropertiesInfo.GroundTexture << "\n";
	File << "ReplaceableTexture11 " << PropertiesInfo.ReplaceableTexture11 << "\n";
	File << "ReplaceableTexture31 " << PropertiesInfo.ReplaceableTexture31 << "\n";
	File << "ReplaceableTexture32 " << PropertiesInfo.ReplaceableTexture32 << "\n";
	File << "ReplaceableTexture33 " << PropertiesInfo.ReplaceableTexture33 << "\n";
	File << "ReplaceableTexture34 " << PropertiesInfo.ReplaceableTexture34 << "\n";
	File << "ReplaceableTexture35 " << PropertiesInfo.ReplaceableTexture35 << "\n";

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the properties from a file
//+-----------------------------------------------------------------------------
BOOL PROPERTIES::LoadPropertiesInfo(CONST std::string& FileName)
{
	std::ifstream File;
	std::string Token;
	std::string PropertiesFileName;

	PropertiesInfo = PROPERTIES_INFO();
	PropertiesFileName = Common.GetProgramDirectory() + "\\" + FileName;

	File.open(PropertiesFileName.c_str(), std::ios::in);
	if(File.fail()) return TRUE;

	while(!File.eof())
	{
		Token = "";
		File >> Token;
		if(Token == "") break;

		if(Token == "Background")
		{
			LoadColor(File, PropertiesInfo.Background);
		}
		else if(Token == "Ambient")
		{
			LoadColor(File, PropertiesInfo.Ambient);
		}
		else if(Token == "Diffuse")
		{
			LoadColor(File, PropertiesInfo.Diffuse);
		}
		else if(Token == "Specular")
		{
			LoadColor(File, PropertiesInfo.Specular);
		}
		else if(Token == "Power")
		{
			File >> PropertiesInfo.Power;
		}
		else if(Token == "FillMode")
		{
			LoadFillMode(File, PropertiesInfo.FillMode);
		}
		else if(Token == "NearDistance")
		{
			File >> PropertiesInfo.NearDistance;
		}
		else if(Token == "FarDistance")
		{
			File >> PropertiesInfo.FarDistance;
		}
		else if(Token == "UseLighting")
		{
			LoadBool(File, PropertiesInfo.UseLighting);
		}
		else if(Token == "UseShaders")
		{
			LoadBool(File, PropertiesInfo.UseShaders);
		}
		else if(Token == "RelativeZoom")
		{
			LoadBool(File, PropertiesInfo.RelativeZoom);
		}
		else if(Token == "RelativeMove")
		{
			LoadBool(File, PropertiesInfo.RelativeMove);
		}
		else if(Token == "ClockwiseCulling")
		{
			LoadBool(File, PropertiesInfo.ClockwiseCulling);
		}
		else if(Token == "UseGroundTexture")
		{
			LoadBool(File, PropertiesInfo.UseGroundTexture);
		}
		else if(Token == "GroundTextureScale")
		{
			File >> PropertiesInfo.GroundTextureScale;
		}
		else if(Token == "ShowDebug")
		{
			LoadBool(File, PropertiesInfo.ShowDebug);
		}
		else if(Token == "ShowAxises")
		{
			LoadBool(File, PropertiesInfo.ShowAxises);
		}
		else if(Token == "ShowLights")
		{
			LoadBool(File, PropertiesInfo.ShowLights);
		}
		else if(Token == "ShowBones")
		{
			LoadBool(File, PropertiesInfo.ShowBones);
		}
		else if(Token == "ShowCameras")
		{
			LoadBool(File, PropertiesInfo.ShowCameras);
		}
		else if(Token == "ShowAttachments")
		{
			LoadBool(File, PropertiesInfo.ShowAttachments);
		}
		else if(Token == "ShowCollisionShapes")
		{
			LoadBool(File, PropertiesInfo.ShowCollisionShapes);
		}
		else if(Token == "BlpQuality")
		{
			File >> PropertiesInfo.BlpQuality;
		}
		else if(Token == "GroundTexture")
		{
			std::getline(File, PropertiesInfo.GroundTexture);
			Common.RemoveWhiteSpace(PropertiesInfo.GroundTexture);
		}
		else if(Token == "ReplaceableTexture11")
		{
			std::getline(File, PropertiesInfo.ReplaceableTexture11);
			Common.RemoveWhiteSpace(PropertiesInfo.ReplaceableTexture11);
		}
		else if(Token == "ReplaceableTexture31")
		{
			std::getline(File, PropertiesInfo.ReplaceableTexture31);
			Common.RemoveWhiteSpace(PropertiesInfo.ReplaceableTexture31);
		}
		else if(Token == "ReplaceableTexture32")
		{
			std::getline(File, PropertiesInfo.ReplaceableTexture32);
			Common.RemoveWhiteSpace(PropertiesInfo.ReplaceableTexture32);
		}
		else if(Token == "ReplaceableTexture33")
		{
			std::getline(File, PropertiesInfo.ReplaceableTexture33);
			Common.RemoveWhiteSpace(PropertiesInfo.ReplaceableTexture33);
		}
		else if(Token == "ReplaceableTexture34")
		{
			std::getline(File, PropertiesInfo.ReplaceableTexture34);
			Common.RemoveWhiteSpace(PropertiesInfo.ReplaceableTexture34);
		}
		else if(Token == "ReplaceableTexture35")
		{
			std::getline(File, PropertiesInfo.ReplaceableTexture35);
			Common.RemoveWhiteSpace(PropertiesInfo.ReplaceableTexture35);
		}
		else
		{
			Error.SetMessage("Unknown token \"" + Token + "\" in \"" + PropertiesFileName + "\"!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Sets a new properties info
//+-----------------------------------------------------------------------------
VOID PROPERTIES::SetPropertiesInfo(CONST PROPERTIES_INFO& NewPropertiesInfo)
{
	PropertiesInfo = NewPropertiesInfo;
}


//+-----------------------------------------------------------------------------
//| Returns the current properties info
//+-----------------------------------------------------------------------------
PROPERTIES_INFO PROPERTIES::GetPropertiesInfo() CONST
{
	return PropertiesInfo;
}


//+-----------------------------------------------------------------------------
//| Returns the current properties info as a reference
//+-----------------------------------------------------------------------------
CONST PROPERTIES_INFO& PROPERTIES::operator ()()
{
	return PropertiesInfo;
}


//+-----------------------------------------------------------------------------
//| Checks if shaders are available
//+-----------------------------------------------------------------------------
BOOL PROPERTIES::ShadersAvailable()
{
	return ShadersIsAvailable;
}


//+-----------------------------------------------------------------------------
//| Makes the shaders available or not
//+-----------------------------------------------------------------------------
VOID PROPERTIES::MakeShadersAvailable(BOOL Available)
{
	ShadersIsAvailable = Available;

	if(!ShadersIsAvailable) PropertiesInfo.UseShaders = FALSE;
}


//+-----------------------------------------------------------------------------
//| Converts a boolean to a string
//+-----------------------------------------------------------------------------
std::string PROPERTIES::BoolToString(BOOL Bool) CONST
{
	return Bool ? "True" : "False";
}


//+-----------------------------------------------------------------------------
//| Converts a color to a string
//+-----------------------------------------------------------------------------
std::string PROPERTIES::ColorToString(CONST D3DCOLOR& Color) CONST
{
	std::stringstream Stream;

	Stream << static_cast<INT>(GetBValue(Color));
	Stream << " ";
	Stream << static_cast<INT>(GetGValue(Color));
	Stream << " ";
	Stream << static_cast<INT>(GetRValue(Color));

	return Stream.str();
}


//+-----------------------------------------------------------------------------
//| Converts a fill mode to a string
//+-----------------------------------------------------------------------------
std::string PROPERTIES::FillModeToString(DWORD FillMode) CONST
{
	switch(FillMode)
	{
		case D3DFILL_POINT:
		{
			return "Point";
		}

		case D3DFILL_WIREFRAME:
		{
			return "Wireframe";
		}

		default:
		{
			return "Solid";
		}
	}
}


//+-----------------------------------------------------------------------------
//| Loads a boolean
//+-----------------------------------------------------------------------------
VOID PROPERTIES::LoadBool(std::ifstream& File, BOOL& Bool) CONST
{
	std::string String;

	File >> String;

	Bool = (Common.LowerCase(String) == "true");
}


//+-----------------------------------------------------------------------------
//| Loads a color
//+-----------------------------------------------------------------------------
VOID PROPERTIES::LoadColor(std::ifstream& File, D3DCOLOR& Color) CONST
{
	INT R = 0;
	INT G = 0;
	INT B = 0;

	File >> R >> G >> B;

	Color = D3DCOLOR_ARGB(0, R, G, B);
}


//+-----------------------------------------------------------------------------
//| Loads a fill mode
//+-----------------------------------------------------------------------------
VOID PROPERTIES::LoadFillMode(std::ifstream& File, DWORD& FillMode) CONST
{
	std::string String;

	File >> String;

	if(String == "Point")
	{
		FillMode = D3DFILL_POINT;
	}
	else if(String == "Wireframe")
	{
		FillMode = D3DFILL_WIREFRAME;
	}
	else
	{
		FillMode = D3DFILL_SOLID;
	}
}

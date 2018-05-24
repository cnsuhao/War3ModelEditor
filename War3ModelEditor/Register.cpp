//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Register.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
REGISTER Register;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
REGISTER::REGISTER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
REGISTER::~REGISTER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Flushes a key
//+-----------------------------------------------------------------------------
VOID REGISTER::FlushKey(HKEY Key)
{
	if(Key != NULL) RegFlushKey(Key);
}


//+-----------------------------------------------------------------------------
//| Closes a key
//+-----------------------------------------------------------------------------
VOID REGISTER::CloseKey(HKEY Key)
{
	if(Key != NULL) RegCloseKey(Key);
}


//+-----------------------------------------------------------------------------
//| Creates a new key
//+-----------------------------------------------------------------------------
BOOL REGISTER::CreateKey(HKEY& Key, HKEY Parent, CONST std::string& Name)
{
	DWORD Mode;

	if(RegCreateKeyEx(Parent, Name.c_str(), 0, "", 0, KEY_ALL_ACCESS, NULL, &Key, &Mode) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to create the registry key \"" + Name + "\"!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Opens an existing key
//+-----------------------------------------------------------------------------
BOOL REGISTER::OpenKey(HKEY& Key, HKEY Parent, CONST std::string& Name)
{
	if(RegOpenKeyEx(Parent, Name.c_str(), 0, KEY_READ, &Key) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to open the registry key \"" + Name + "\"!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Sets a dword value
//+-----------------------------------------------------------------------------
BOOL REGISTER::SetDwordValue(HKEY Key, CONST std::string& Name, DWORD Dword)
{
	if(RegSetValueEx(Key, NULL, 0, REG_DWORD, reinterpret_cast<CONST BYTE*>(&Dword), sizeof(DWORD)) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to set a registry key string value!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Sets a binary value
//+-----------------------------------------------------------------------------
BOOL REGISTER::SetBinaryValue(HKEY Key, CONST std::string& Name, BUFFER& Buffer)
{
	if(RegSetValueEx(Key, NULL, 0, REG_BINARY, reinterpret_cast<CONST BYTE*>(Buffer.GetData()), static_cast<INT>(Buffer.GetSize()) + 1) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to set a registry key string value!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Sets a string value
//+-----------------------------------------------------------------------------
BOOL REGISTER::SetStringValue(HKEY Key, CONST std::string& Name, CONST std::string& String)
{
	if(RegSetValueEx(Key, (Name == "" ? NULL : Name.c_str()), 0, REG_SZ, reinterpret_cast<CONST BYTE*>(String.c_str()), static_cast<INT>(String.size()) + 1) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to set a registry key string value!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns a dword value
//+-----------------------------------------------------------------------------
BOOL REGISTER::GetDwordValue(HKEY Key, CONST std::string& Name, DWORD& Dword)
{
	DWORD Type;
	DWORD Size;

	if(RegQueryValueEx(Key, Name.c_str(), 0, &Type, NULL, &Size) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to get a registry key dword value information!");
		return FALSE;
	}

	if(Type != REG_DWORD)
	{
		Error.SetMessage("The registry key is not a dword!");
		return FALSE;
	}

	if(RegQueryValueEx(Key, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(&Dword), &Size) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to get a registry key dword value!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns a binary value
//+-----------------------------------------------------------------------------
BOOL REGISTER::GetBinaryValue(HKEY Key, CONST std::string& Name, BUFFER& Buffer)
{
	DWORD Type;
	DWORD Size;

	if(RegQueryValueEx(Key, Name.c_str(), 0, &Type, NULL, &Size) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to get a registry key binary value information!");
		return FALSE;
	}

	if(Type != REG_BINARY)
	{
		Error.SetMessage("The registry key is not binary!");
		return FALSE;
	}

	if(!Buffer.Resize(Size)) return FALSE;

	if(RegQueryValueEx(Key, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(Buffer.GetData()), &Size) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to get a registry key binary value!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns a string value
//+-----------------------------------------------------------------------------
BOOL REGISTER::GetStringValue(HKEY Key, CONST std::string& Name, std::string& String)
{
	DWORD Type;
	DWORD Size;
	std::vector<CHAR> Buffer;

	if(RegQueryValueEx(Key, Name.c_str(), 0, &Type, NULL, &Size) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to get a registry key string value information!");
		return FALSE;
	}

	if(Type != REG_SZ)
	{
		Error.SetMessage("The registry key is not a string!");
		return FALSE;
	}

	Buffer.resize(Size + 1);

	if(RegQueryValueEx(Key, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(&Buffer[0]), &Size) != ERROR_SUCCESS)
	{
		Error.SetMessage("Unable to get a registry key string value!");
		return FALSE;
	}

	Buffer[Size] = '\0';
	String = &Buffer[0];

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Registers an extention
//+-----------------------------------------------------------------------------
BOOL REGISTER::RegisterExtention(CONST std::string& Extention, CONST std::string& Description, CONST std::string& Icon)
{
	BOOL Result = FALSE;
	HKEY Key = NULL;
	HKEY MpqKey = NULL;
	HKEY MpqIconKey = NULL;
	HKEY MpqShellKey = NULL;
	HKEY MpqShellOpenKey = NULL;
	HKEY MpqShellOpenCommandKey = NULL;
	std::string StringName;
	std::string StringExtention;
	std::string StringIconPath;
	std::string StringProgramPath;

	while(TRUE)
	{
		StringName = "Magos.War3ModelEditor." + Common.LowerCase(Extention);
		StringExtention = "." + Common.LowerCase(Extention);
		StringIconPath = Common.GetProgramDirectory() + "\\" + Icon;
		StringProgramPath = Common.GetProgramDirectory() + "\\" + Common.GetProgramFileName() + " %1";

		if(!CreateKey(Key, HKEY_CLASSES_ROOT, StringExtention)) break;
		if(!CreateKey(MpqKey, HKEY_CLASSES_ROOT, StringName)) break;
		if(!CreateKey(MpqIconKey, MpqKey, "DefaultIcon")) break;
		if(!CreateKey(MpqShellKey, MpqKey, "Shell")) break;
		if(!CreateKey(MpqShellOpenKey, MpqShellKey, "Open")) break;
		if(!CreateKey(MpqShellOpenCommandKey, MpqShellOpenKey, "Command")) break;

		if(!SetStringValue(Key, "", StringName)) break;
		if(!SetStringValue(MpqKey, "", Description)) break;
		if(!SetStringValue(MpqIconKey, "", StringIconPath)) break;
		if(!SetStringValue(MpqShellOpenCommandKey, "", StringProgramPath)) break;

		FlushKey(Key);
		FlushKey(MpqKey);
		FlushKey(MpqIconKey);
		FlushKey(MpqShellKey);
		FlushKey(MpqShellOpenKey);
		FlushKey(MpqShellOpenCommandKey);

		Result = TRUE;
		break;
	}

	CloseKey(MpqShellOpenCommandKey);
	CloseKey(MpqShellOpenKey);
	CloseKey(MpqShellKey);
	CloseKey(MpqIconKey);
	CloseKey(MpqKey);
	CloseKey(Key);

	return Result;
}


//+-----------------------------------------------------------------------------
//| Registers an open-with command
//+-----------------------------------------------------------------------------
BOOL REGISTER::RegisterOpenWith(CONST std::string& Description)
{
	BOOL Result = FALSE;
	HKEY AllKey = NULL;
	HKEY AllShellKey = NULL;
	HKEY AllShellOpenKey = NULL;
	HKEY AllShellOpenCommandKey = NULL;
	std::string StringProgramPath;

	while(TRUE)
	{
		StringProgramPath = Common.GetProgramDirectory() + "\\" + Common.GetProgramFileName() + " %1";

		if(!CreateKey(AllKey, HKEY_CLASSES_ROOT, "AllFilesystemObjects")) break;
		if(!CreateKey(AllShellKey, AllKey, "Shell")) break;
		if(!CreateKey(AllShellOpenKey, AllShellKey, Description)) break;
		if(!CreateKey(AllShellOpenCommandKey, AllShellOpenKey, "Command")) break;

		if(!SetStringValue(AllShellOpenCommandKey, "", StringProgramPath)) break;

		FlushKey(AllKey);
		FlushKey(AllShellKey);
		FlushKey(AllShellOpenKey);
		FlushKey(AllShellOpenCommandKey);

		Result = TRUE;
		break;
	}

	CloseKey(AllShellOpenCommandKey);
	CloseKey(AllShellOpenKey);
	CloseKey(AllShellKey);
	CloseKey(AllKey);

	return Result;
}


//+-----------------------------------------------------------------------------
//| Retyrieves the warcraft directory from the registry
//+-----------------------------------------------------------------------------
BOOL REGISTER::FindWarcraftDirectory()
{
	BOOL Result = FALSE;
	HKEY SoftwareKey = NULL;
	HKEY SoftwareBlizzardKey = NULL;
	HKEY SoftwareBlizzardWarcraftKey = NULL;

	while(TRUE)
	{
		if(!OpenKey(SoftwareKey, HKEY_CURRENT_USER, "Software")) break;
		if(!OpenKey(SoftwareBlizzardKey, SoftwareKey, "Blizzard Entertainment")) break;
		if(!OpenKey(SoftwareBlizzardWarcraftKey, SoftwareBlizzardKey, "Warcraft III")) break;

		if(!GetStringValue(SoftwareBlizzardWarcraftKey, "InstallPath", WarcraftDirectory)) break;

		Result = TRUE;
		break;
	}

	CloseKey(SoftwareBlizzardWarcraftKey);
	CloseKey(SoftwareBlizzardKey);
	CloseKey(SoftwareKey);

	if(!Result) WarcraftDirectory = Common.GetProgramDirectory();
	WarcraftDirectory ="F:\\Warcraft";
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the warcraft directory
//+-----------------------------------------------------------------------------
std::string REGISTER::GetWarcraftDirectory() CONST
{
	return WarcraftDirectory;
}

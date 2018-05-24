//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Common.h"

//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
COMMON Common;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
COMMON::COMMON()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
COMMON::~COMMON()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Copies a string into the clipboard
//+-----------------------------------------------------------------------------
BOOL COMMON::SetClipboardData(CONST std::string& String)
{
	HGLOBAL GlobalString;
	CHAR* GlobalStringPointer;

	if(!::OpenClipboard(NULL))
	{
		Error.SetMessage("Unable to open the clipboard!");
		return FALSE;
	}

	GlobalString = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, static_cast<INT>(String.size()) + 1);
	if(GlobalString == NULL)
	{
		::CloseClipboard();
		Error.SetMessage("Unable to allocate memory for the clipboard data!");
		return FALSE;
	}

	EmptyClipboard();

	GlobalStringPointer = reinterpret_cast<CHAR*>(::GlobalLock(GlobalString));
	if(GlobalStringPointer == NULL)
	{
		::CloseClipboard();
		Error.SetMessage("Unable to lock the global memory!");
		return FALSE;
	}

	memcpy(GlobalStringPointer, String.c_str(), static_cast<INT>(String.size()) + 1);
	::GlobalUnlock(GlobalString);

	::SetClipboardData(CF_TEXT, GlobalString);
	::CloseClipboard();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Retrieves a string from the clipboard
//+-----------------------------------------------------------------------------
std::string COMMON::GetClipboardData()
{
	INT Size;
	HGLOBAL GlobalString;
	CHAR* GlobalStringPointer;
	std::vector<CHAR> Buffer;

	if(!::OpenClipboard(NULL)) return "";

	GlobalString = ::GetClipboardData(CF_TEXT);
	if(GlobalString == NULL)
	{
		::CloseClipboard();
		return "";
	}

	GlobalStringPointer = reinterpret_cast<CHAR*>(::GlobalLock(GlobalString));
	if(GlobalStringPointer == NULL)
	{
		::CloseClipboard();
		return "";
	}

	Size = static_cast<INT>(strlen(GlobalStringPointer));
	Buffer.resize(Size + 1);
	memcpy(&Buffer[0], GlobalStringPointer, Size);
	Buffer[Size] = '\0';

	::GlobalUnlock(GlobalString);
	::CloseClipboard();

	return &Buffer[0];
}


//+-----------------------------------------------------------------------------
//| Checks if a point is inside a rect
//+-----------------------------------------------------------------------------
BOOL COMMON::PointInsideRect(CONST POINT& Point, CONST RECT& Rect) CONST
{
	if(Point.x < Rect.left) return FALSE;
	if(Point.y < Rect.top) return FALSE;
	if(Point.x >= Rect.right) return FALSE;
	if(Point.y >= Rect.bottom) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Turns the string into all lowercase characters
//+-----------------------------------------------------------------------------
std::string COMMON::LowerCase(CONST std::string& String) CONST
{
	INT i;
	std::string Result = String;

	for(i = 0; i < static_cast<INT>(Result.size()); i++)
	{
		Result[i] = tolower(Result[i]);
	}

	return Result;
}


//+-----------------------------------------------------------------------------
//| Turns the string into all uppercase characters
//+-----------------------------------------------------------------------------
std::string COMMON::UpperCase(CONST std::string& String) CONST
{
	INT i;
	std::string Result = String;

	for(i = 0; i < static_cast<INT>(Result.size()); i++)
	{
		Result[i] = toupper(Result[i]);
	}

	return Result;
}


//+-----------------------------------------------------------------------------
//| Returns the path of a filename
//+-----------------------------------------------------------------------------
std::string COMMON::GetPath(CONST std::string& FileName) CONST
{
	INT i;

	i = static_cast<INT>(FileName.rfind("\\"));
	if(i == std::string::npos)
	{
		i = static_cast<INT>(FileName.rfind("/"));
		if(i == std::string::npos)
		{
			return "";
		}
	}

	return FileName.substr(0, i + 1);
}


//+-----------------------------------------------------------------------------
//| Returns the name of a filename
//+-----------------------------------------------------------------------------
std::string COMMON::GetName(CONST std::string& FileName) CONST
{
	INT i;
	INT j;

	i = static_cast<INT>(FileName.rfind("\\"));
	if(i == std::string::npos)
	{
		i = static_cast<INT>(FileName.rfind("/"));
		if(i == std::string::npos)
		{
			i = -1;
		}
	}

	j = static_cast<INT>(FileName.rfind("."));
	if(j == std::string::npos) j = static_cast<INT>(FileName.size());

	return FileName.substr(i + 1, j - i - 1);
}


//+-----------------------------------------------------------------------------
//| Returns the filename of a filename (excluding the path)
//+-----------------------------------------------------------------------------
std::string COMMON::GetFileName(CONST std::string& FileName) CONST
{
	INT i;

	i = static_cast<INT>(FileName.rfind("\\"));
	if(i == std::string::npos)
	{
		i = static_cast<INT>(FileName.rfind("/"));
		if(i == std::string::npos)
		{
			return FileName;
		}
	}

	return FileName.substr(i + 1, FileName.size() - i - 1);
}


//+-----------------------------------------------------------------------------
//| Returns the extention of a filename
//+-----------------------------------------------------------------------------
std::string COMMON::GetExtention(CONST std::string& FileName) CONST
{
	INT i;

	i = static_cast<INT>(FileName.rfind("."));
	if(i == std::string::npos) return "";

	return FileName.substr(i + 1, FileName.size() - i - 1);
}


//+-----------------------------------------------------------------------------
//| Splits a filename into path, name and extention
//+-----------------------------------------------------------------------------
VOID COMMON::SplitFileName(std::string& FullFileName, std::string& Path, std::string& Name, std::string& FileName,std::string& Extention) CONST
{
	INT i;
	INT j;

	i = static_cast<INT>(FullFileName.rfind("\\"));
	if(i == std::string::npos)
	{
		i = static_cast<INT>(FullFileName.rfind("/"));
		if(i == std::string::npos)
		{
			i = -1;
		}
	}

	j = static_cast<INT>(FullFileName.rfind("."));
	if(j == std::string::npos) j = static_cast<INT>(FullFileName.size());

	Path = (i >= 0) ? FullFileName.substr(0, i + 1) : "";
	Name = FullFileName.substr(i + 1, j - i - 1);
	FileName = FullFileName.substr(i + 1, FullFileName.size() - i - 1);
	Extention = (j < static_cast<INT>(FullFileName.size())) ? FullFileName.substr(i + 1, FullFileName.size() - i - 1) : "";
}


//+-----------------------------------------------------------------------------
//| Returns the program filename
//+-----------------------------------------------------------------------------
std::string COMMON::GetProgramFileName() CONST
{
	INT i;
	std::string FileName;
	CHAR Buffer[BUFFER_SIZE];

	::GetModuleFileName(NULL, Buffer, BUFFER_SIZE - 1);
	FileName = Buffer;

	i = static_cast<INT>(FileName.rfind("\\"));
	if(i != std::string::npos)
	{
		FileName = FileName.substr((i + 1), (static_cast<INT>(FileName.size()) - (i + 1)));
	}

	return FileName;
}


//+-----------------------------------------------------------------------------
//| Returns the program directory
//+-----------------------------------------------------------------------------
std::string COMMON::GetProgramDirectory() CONST
{
	INT i;
	std::string Directory;
	CHAR Buffer[BUFFER_SIZE];

	::GetModuleFileName(NULL, Buffer, BUFFER_SIZE - 1);
	Directory = Buffer;

	i = static_cast<INT>(Directory.rfind("\\"));
	if(i != std::string::npos)
	{
		Directory = Directory.substr(0, i);
	}
	Directory = "F:\\Warcraft";
	return Directory;
}


//+-----------------------------------------------------------------------------
//| Returns the current directory
//+-----------------------------------------------------------------------------
std::string COMMON::GetCurrentDirectory() CONST
{
	CHAR Buffer[BUFFER_SIZE];

	if(::GetCurrentDirectory(BUFFER_SIZE - 1, Buffer) == 0) return "";

	return Buffer;
}


//+-----------------------------------------------------------------------------
//| Sets a new current directory
//+-----------------------------------------------------------------------------
VOID COMMON::SetCurrentDirectory(CONST std::string& NewDirectory) CONST
{
	::SetCurrentDirectory(NewDirectory.c_str());
}


//+-----------------------------------------------------------------------------
//| Checks if a character is a whitespace
//+-----------------------------------------------------------------------------
BOOL COMMON::IsWhiteSpace(CHAR Char) CONST
{
	if(Char == ' ')  return TRUE;
	if(Char == '\t') return TRUE;
	if(Char == '\n') return TRUE;
	if(Char == '\r') return TRUE;

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Checks if a character is a linebreak
//+-----------------------------------------------------------------------------
BOOL COMMON::IsLineBreak(CHAR Char) CONST
{
	if(Char == '\n') return TRUE;
	if(Char == '\r') return TRUE;

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Checks if a character is a digit
//+-----------------------------------------------------------------------------
BOOL COMMON::IsDigit(CHAR Char) CONST
{
	return (Char >= '0') && (Char <= '9');
}


//+-----------------------------------------------------------------------------
//| Checks if a character is a letter
//+-----------------------------------------------------------------------------
BOOL COMMON::IsLetter(CHAR Char) CONST
{
	if((Char >= 'a') && (Char <= 'z')) return TRUE;
	if((Char >= 'A') && (Char <= 'Z')) return TRUE;

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Checks if a character is an uppercase letter
//+-----------------------------------------------------------------------------
BOOL COMMON::IsLowerCaseLetter(CHAR Char) CONST
{
	return (Char >= 'a') && (Char <= 'z');
}


//+-----------------------------------------------------------------------------
//| Checks if a character is a lowercase letter
//+-----------------------------------------------------------------------------
BOOL COMMON::IsUpperCaseLetter(CHAR Char) CONST
{
	return (Char >= 'A') && (Char <= 'Z');
}


//+-----------------------------------------------------------------------------
//| Removes all preceeding and postceeding whitespace characters
//+-----------------------------------------------------------------------------
VOID COMMON::RemoveWhiteSpace(std::string& String) CONST
{
	while(String.size() > 0)
	{
		if(!IsWhiteSpace(String[0])) break;
		String.erase(0, 1);
	}

	while(String.size() > 0)
	{
		if(!IsWhiteSpace(String[String.size() - 1])) break;
		String.erase(String.size() - 1, 1);
	}
}


//+-----------------------------------------------------------------------------
//| Converts a string to a bool
//+-----------------------------------------------------------------------------
BOOL COMMON::StringToBool(CONST std::string& String) CONST
{
	return (LowerCase(String) == "true");
}


//+-----------------------------------------------------------------------------
//| Converts a string to an int
//+-----------------------------------------------------------------------------
INT COMMON::StringToInt(CONST std::string& String) CONST
{
	INT Int = 0;
	std::stringstream Stream(String);

	Stream >> Int;

	return Int;
}


//+-----------------------------------------------------------------------------
//| Converts a string to a float
//+-----------------------------------------------------------------------------
FLOAT COMMON::StringToFloat(CONST std::string& String) CONST
{
	FLOAT Float = 0.0f;
	std::stringstream Stream(String);

	Stream >> Float;

	return Float;
}


//+-----------------------------------------------------------------------------
//| Converts a string to a double
//+-----------------------------------------------------------------------------
DOUBLE COMMON::StringToDouble(CONST std::string& String) CONST
{
	DOUBLE Double = 0.0;
	std::stringstream Stream(String);

	Stream >> Double;

	return Double;
}


//+-----------------------------------------------------------------------------
//| Converts a bool to a string
//+-----------------------------------------------------------------------------
std::string COMMON::BoolToString(BOOL Bool) CONST
{
	return Bool ? "true" : "false";
}


//+-----------------------------------------------------------------------------
//| Converts an int to a string
//+-----------------------------------------------------------------------------
std::string COMMON::IntToString(INT Int) CONST
{
	std::stringstream Stream;

	Stream << Int;

	return Stream.str();
}


//+-----------------------------------------------------------------------------
//| Converts a float to a string
//+-----------------------------------------------------------------------------
std::string COMMON::FloatToString(FLOAT Float) CONST
{
	std::stringstream Stream;

	Stream << Float;

	return Stream.str();
}


//+-----------------------------------------------------------------------------
//| Converts a double to a string
//+-----------------------------------------------------------------------------
std::string COMMON::DoubleToString(DOUBLE Double) CONST
{
	std::stringstream Stream;

	Stream << Double;

	return Stream.str();
}


//+-----------------------------------------------------------------------------
//| Converts a color to a 3-dimensional vector
//+-----------------------------------------------------------------------------
D3DXVECTOR3 COMMON::ColorToVector3(D3DCOLOR Color)
{
	D3DXVECTOR3 Vector;
	D3DXCOLOR TempColor = Color;

	Vector.z = TempColor.r;
	Vector.y = TempColor.g;
	Vector.x = TempColor.b;

	return Vector;
}


//+-----------------------------------------------------------------------------
//| Converts a color to a 4-dimensional vector
//+-----------------------------------------------------------------------------
D3DXVECTOR4 COMMON::ColorToVector4(D3DCOLOR Color)
{
	D3DXVECTOR4 Vector;
	D3DXCOLOR TempColor = Color;

	Vector.z = TempColor.r;
	Vector.y = TempColor.g;
	Vector.x = TempColor.b;
	Vector.w = TempColor.a;

	return Vector;
}


//+-----------------------------------------------------------------------------
//| Converts a 3-dimensional vector toa  color
//+-----------------------------------------------------------------------------
D3DCOLOR COMMON::Vector3ToColor(CONST D3DXVECTOR3& Vector)
{
	return D3DCOLOR_COLORVALUE(Vector.z, Vector.y, Vector.x, 1.0f);
}


//+-----------------------------------------------------------------------------
//| Converts a 4-dimensional vector toa  color
//+-----------------------------------------------------------------------------
D3DCOLOR COMMON::Vector4ToColor(CONST D3DXVECTOR4& Vector)
{
	return D3DCOLOR_COLORVALUE(Vector.z, Vector.y, Vector.x, Vector.w);
}


//+-----------------------------------------------------------------------------
//| Returns a string version of the current date
//+-----------------------------------------------------------------------------
std::string COMMON::GetCurrentDate()
{
	INT Size;
	DWORD Locale;
	std::string Date;
	std::string Time;
	std::vector<CHAR> Buffer;

	Locale = MAKELCID(CURRENT_LANGUAGE, SORT_DEFAULT);

	Size = GetDateFormat(Locale, 0, NULL, STRING_DATE_FORMAT.c_str(), NULL, 0);
	Buffer.resize(Size + 1);

	GetDateFormat(Locale, 0, NULL, STRING_DATE_FORMAT.c_str(), &Buffer[0], Size);
	Buffer[Size] = '\0';
	Date = &Buffer[0];

	Size = GetTimeFormat(Locale, 0, NULL, STRING_TIME_FORMAT.c_str(), NULL, 0);
	Buffer.resize(Size + 1);

	GetTimeFormat(Locale, 0, NULL, STRING_TIME_FORMAT.c_str(), &Buffer[0], Size);
	Buffer[Size] = '\0';
	Time = &Buffer[0];

	return Date + " " + Time;
}

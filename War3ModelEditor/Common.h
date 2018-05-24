//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_COMMON_H
#define MAGOS_COMMON_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Error.h"

//+-----------------------------------------------------------------------------
//| Common class
//+-----------------------------------------------------------------------------
class COMMON
{
	public:
		CONSTRUCTOR COMMON();
		DESTRUCTOR ~COMMON();

		BOOL SetClipboardData(CONST std::string& String);
		std::string GetClipboardData();

		BOOL PointInsideRect(CONST POINT& Point, CONST RECT& Rect) CONST;

		std::string LowerCase(CONST std::string& String) CONST;
		std::string UpperCase(CONST std::string& String) CONST;

		std::string GetPath(CONST std::string& FileName) CONST;
		std::string GetName(CONST std::string& FileName) CONST;
		std::string GetFileName(CONST std::string& FileName) CONST;
		std::string GetExtention(CONST std::string& FileName) CONST;
		
		VOID SplitFileName(std::string& FullFileName, std::string& Path, std::string& Name, std::string& FileName, std::string& Extention) CONST;

		std::string GetProgramFileName() CONST;
		std::string GetProgramDirectory() CONST;
		std::string GetCurrentDirectory() CONST;
		VOID SetCurrentDirectory(CONST std::string& NewDirectory) CONST;

		BOOL IsWhiteSpace(CHAR Char) CONST;
		BOOL IsLineBreak(CHAR Char) CONST;
		BOOL IsDigit(CHAR Char) CONST;
		BOOL IsLetter(CHAR Char) CONST;
		BOOL IsLowerCaseLetter(CHAR Char) CONST;
		BOOL IsUpperCaseLetter(CHAR Char) CONST;

		VOID RemoveWhiteSpace(std::string& String) CONST;

		BOOL StringToBool(CONST std::string& String) CONST;
		INT StringToInt(CONST std::string& String) CONST;
		FLOAT StringToFloat(CONST std::string& String) CONST;
		DOUBLE StringToDouble(CONST std::string& String) CONST;

		std::string BoolToString(BOOL Bool) CONST;
		std::string IntToString(INT Int) CONST;
		std::string FloatToString(FLOAT Float) CONST;
		std::string DoubleToString(DOUBLE Double) CONST;

		D3DXVECTOR3 ColorToVector3(D3DCOLOR Color);
		D3DXVECTOR4 ColorToVector4(D3DCOLOR Color);

		D3DCOLOR Vector3ToColor(CONST D3DXVECTOR3& Vector);
		D3DCOLOR Vector4ToColor(CONST D3DXVECTOR4& Vector);

		std::string GetCurrentDate();

	protected:
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern COMMON Common;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

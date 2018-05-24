//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TokenInStream.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TOKEN_IN_STREAM::TOKEN_IN_STREAM()
{
	FileName = "";
	Group = "";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TOKEN_IN_STREAM::~TOKEN_IN_STREAM()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Sets a new filename
//+-----------------------------------------------------------------------------
VOID TOKEN_IN_STREAM::SetFileName(CONST std::string& NewFileName)
{
	FileName = NewFileName;
}


//+-----------------------------------------------------------------------------
//| Returns the filename
//+-----------------------------------------------------------------------------
std::string TOKEN_IN_STREAM::GetFileName()
{
	return FileName;
}


//+-----------------------------------------------------------------------------
//| Sets a new group
//+-----------------------------------------------------------------------------
VOID TOKEN_IN_STREAM::SetGroup(CONST std::string& NewGroup)
{
	Group = NewGroup;
}


//+-----------------------------------------------------------------------------
//| Returns the group
//+-----------------------------------------------------------------------------
std::string TOKEN_IN_STREAM::GetGroup()
{
	return Group;
}


//+-----------------------------------------------------------------------------
//| Clears the token stream
//+-----------------------------------------------------------------------------
VOID TOKEN_IN_STREAM::Clear()
{
	Stream.str("");
	Stream.clear();
}


//+-----------------------------------------------------------------------------
//| Loads a file
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::Load(BUFFER& Buffer)
{
	Clear();

	Stream.str(reinterpret_cast<CHAR*>(Buffer.GetData()));

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a string
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::Load(CONST std::string& String)
{
	Clear();

	Stream.str(String);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Checks if end-of-file has been reached
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::Eof() CONST
{
	return Stream.eof();
}


//+-----------------------------------------------------------------------------
//| Reads a boolean
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::ReadBool()
{
	std::string String;

	Stream >> String;

	return (Common.LowerCase(String) == "true");
}


//+-----------------------------------------------------------------------------
//| Reads a character
//+-----------------------------------------------------------------------------
CHAR TOKEN_IN_STREAM::ReadChar()
{
	CHAR Char;
	std::string TempString;

	while(!Stream.eof())
	{
		Char = ' ';
		Stream >> Char;

		if(Char != '/') return Char;
		if(Stream.peek() != '/') return Char;

		std::getline(Stream, TempString);
	}

	return ' ';
}


//+-----------------------------------------------------------------------------
//| Reads an integer
//+-----------------------------------------------------------------------------
INT TOKEN_IN_STREAM::ReadInt()
{
	INT Int = 0;

	Stream >> Int;

	return Int;
}


//+-----------------------------------------------------------------------------
//| Reads a floating point number
//+-----------------------------------------------------------------------------
FLOAT TOKEN_IN_STREAM::ReadFloat()
{
	FLOAT Float = 0.0f;

	Stream >> Float;

	return Float;
}


//+-----------------------------------------------------------------------------
//| Reads a double precision floating point number
//+-----------------------------------------------------------------------------
DOUBLE TOKEN_IN_STREAM::ReadDouble()
{
	DOUBLE Double = 0.0;

	Stream >> Double;

	return Double;
}


//+-----------------------------------------------------------------------------
//| Reads a single word
//+-----------------------------------------------------------------------------
std::string TOKEN_IN_STREAM::ReadWord()
{
	std::string Word;

	while(!Stream.eof())
	{
		Word = "";
		Stream >> Word;

		if(!IsComment(Word)) return Word;

		std::getline(Stream, Word);
	}

	return "";
}


//+-----------------------------------------------------------------------------
//| Reads a whole line
//+-----------------------------------------------------------------------------
std::string TOKEN_IN_STREAM::ReadLine()
{
	std::string Line;

	while(!Stream.eof())
	{
		Line = "";
		std::getline(Stream, Line);
		Common.RemoveWhiteSpace(Line);

		if(!IsComment(Line)) return Line;
	}

	return "";
}


//+-----------------------------------------------------------------------------
//| Reads a string
//+-----------------------------------------------------------------------------
std::string TOKEN_IN_STREAM::ReadString()
{
	CHAR TempChar;
	std::string TempString = "";

	Stream >> TempChar;
	if(TempChar != '\"') return "";

	TempChar = Stream.get();
	while(!Stream.eof())
	{
		if(TempChar == '\"') return TempString;

		TempString += TempChar;
		TempChar = Stream.get();
	}

	return "";
}


//+-----------------------------------------------------------------------------
//| Reads a 2-dimensional vector
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::ReadVector2(D3DXVECTOR2& Vector)
{
	if(!ExpectChar('{')) return FALSE;
	Stream >> Vector.x;
	if(!ExpectChar(',')) return FALSE;
	Stream >> Vector.y;
	if(!ExpectChar('}')) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Reads a 3-dimensional vector
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::ReadVector3(D3DXVECTOR3& Vector)
{
	if(!ExpectChar('{')) return FALSE;
	Stream >> Vector.x;
	if(!ExpectChar(',')) return FALSE;
	Stream >> Vector.y;
	if(!ExpectChar(',')) return FALSE;
	Stream >> Vector.z;
	if(!ExpectChar('}')) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Reads a 4-dimensional vector
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::ReadVector4(D3DXVECTOR4& Vector)
{
	if(!ExpectChar('{')) return FALSE;
	Stream >> Vector.x;
	if(!ExpectChar(',')) return FALSE;
	Stream >> Vector.y;
	if(!ExpectChar(',')) return FALSE;
	Stream >> Vector.z;
	if(!ExpectChar(',')) return FALSE;
	Stream >> Vector.w;
	if(!ExpectChar('}')) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Reads a 2-dimensional vector
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::ReadVector2(D3DXVECTOR4& Vector)
{
	if(!ExpectChar('{')) return FALSE;
	Stream >> Vector.x;
	if(!ExpectChar(',')) return FALSE;
	Stream >> Vector.y;
	if(!ExpectChar('}')) return FALSE;
	Vector.z = 0.0f;
	Vector.w = 0.0f;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Reads a 3-dimensional vector
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::ReadVector3(D3DXVECTOR4& Vector)
{
	if(!ExpectChar('{')) return FALSE;
	Stream >> Vector.x;
	if(!ExpectChar(',')) return FALSE;
	Stream >> Vector.y;
	if(!ExpectChar(',')) return FALSE;
	Stream >> Vector.z;
	if(!ExpectChar('}')) return FALSE;
	Vector.w = 0.0f;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Expects a certain character
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::ExpectChar(CHAR Char)
{
	CHAR TempChar;

	TempChar = ReadChar();
	if(TempChar == Char) return TRUE;

	Error.SetMessage(std::string("Expected \"") + Char + "\", got \"" + TempChar + "\" in group \"" + Group + "\" in \"" + FileName + "\"!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Expects a certain word
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::ExpectWord(CONST std::string& Word)
{
	std::string TempWord;

	TempWord = ReadWord();
	if(TempWord == Word) return TRUE;

	Error.SetMessage("Expected \"" + Word + "\", got \"" + TempWord + "\" in group \"" + Group + "\" in \"" + FileName + "\"!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Checks if a string is a comment
//+-----------------------------------------------------------------------------
BOOL TOKEN_IN_STREAM::IsComment(std::string String)
{
	Common.RemoveWhiteSpace(String);

	if(String.size() < 2) return FALSE;
	if(String[0] != '/')  return FALSE;
	if(String[1] != '/')  return FALSE;

	return TRUE;
}

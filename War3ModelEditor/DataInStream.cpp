//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "DataInStream.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
DATA_IN_STREAM::DATA_IN_STREAM()
{
	Position = INVALID_INDEX;
	FileName = "";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
DATA_IN_STREAM::~DATA_IN_STREAM()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Sets a new filename
//+-----------------------------------------------------------------------------
VOID DATA_IN_STREAM::SetFileName(CONST std::string& NewFileName)
{
	FileName = NewFileName;
}


//+-----------------------------------------------------------------------------
//| Returns the filename
//+-----------------------------------------------------------------------------
std::string DATA_IN_STREAM::GetFileName()
{
	return FileName;
}


//+-----------------------------------------------------------------------------
//| Clears the data stream
//+-----------------------------------------------------------------------------
VOID DATA_IN_STREAM::Clear()
{
	Position = INVALID_INDEX;
	DataBuffer.Clear();
}


//+-----------------------------------------------------------------------------
//| Loads a file
//+-----------------------------------------------------------------------------
BOOL DATA_IN_STREAM::Load(BUFFER& Buffer)
{
	Clear();

	DataBuffer = Buffer;
	Position = 0;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Checks if end-of-file has been reached
//+-----------------------------------------------------------------------------
BOOL DATA_IN_STREAM::Eof() CONST
{
	return (Position == INVALID_INDEX);
}


//+-----------------------------------------------------------------------------
//| Returns the position of the stream pointer
//+-----------------------------------------------------------------------------
INT DATA_IN_STREAM::GetPosition() CONST
{
	return Position;
}


//+-----------------------------------------------------------------------------
//| Sets a new stream pointer position
//+-----------------------------------------------------------------------------
VOID DATA_IN_STREAM::SetPosition(INT NewPosition, BOOL FromEnd)
{
	Position = FromEnd ? (DataBuffer.GetSize() + NewPosition) : NewPosition;

	if(Position < 0) Position = INVALID_INDEX;
	if(Position >= DataBuffer.GetSize()) Position = INVALID_INDEX;
}


//+-----------------------------------------------------------------------------
//| Reads data from the stream
//+-----------------------------------------------------------------------------
BOOL DATA_IN_STREAM::Read(CHAR* Buffer, INT NrOfBytes)
{
	if(NrOfBytes <= 0)
	{
		Error.SetMessage("Unable to read from the stream, must read at least 1 byte!");
		return FALSE;
	}

	if(Position == INVALID_INDEX)
	{
		Error.SetMessage("Unable to read from the stream, EOF has been reached!");
		return FALSE;
	}

	if((Position + NrOfBytes) > DataBuffer.GetSize())
	{
		Error.SetMessage("Unable to read more bytes from the stream than there exists!");
		return FALSE;
	}

	memcpy(Buffer, &DataBuffer[Position], NrOfBytes);

	Position += NrOfBytes;
	if(Position >= DataBuffer.GetSize()) Position = INVALID_INDEX;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Reads a single character from the stream
//+-----------------------------------------------------------------------------
CHAR DATA_IN_STREAM::ReadChar()
{
	CHAR Char = ' ';

	Read(reinterpret_cast<CHAR*>(&Char), sizeof(CHAR));

	return Char;
}


//+-----------------------------------------------------------------------------
//| Reads an integer from the stream
//+-----------------------------------------------------------------------------
INT DATA_IN_STREAM::ReadInt()
{
	INT Int = 0;

	Read(reinterpret_cast<CHAR*>(&Int), sizeof(INT));

	return Int;
}


//+-----------------------------------------------------------------------------
//| Reads a floating point number from the stream
//+-----------------------------------------------------------------------------
FLOAT DATA_IN_STREAM::ReadFloat()
{
	FLOAT Float = 0.0f;

	Read(reinterpret_cast<CHAR*>(&Float), sizeof(FLOAT));

	return Float;
}


//+-----------------------------------------------------------------------------
//| Reads a double precision floating point number from the stream
//+-----------------------------------------------------------------------------
DOUBLE DATA_IN_STREAM::ReadDouble()
{
	DOUBLE Double = 0.0;

	Read(reinterpret_cast<CHAR*>(&Double), sizeof(DOUBLE));

	return Double;
}


//+-----------------------------------------------------------------------------
//| Reads a byte from the stream
//+-----------------------------------------------------------------------------
BYTE DATA_IN_STREAM::ReadByte()
{
	BYTE Byte = 0;

	Read(reinterpret_cast<CHAR*>(&Byte), sizeof(BYTE));

	return Byte;
}


//+-----------------------------------------------------------------------------
//| Reads a word from the stream
//+-----------------------------------------------------------------------------
WORD DATA_IN_STREAM::ReadWord()
{
	WORD Word = 0;

	Read(reinterpret_cast<CHAR*>(&Word), sizeof(WORD));

	return Word;
}


//+-----------------------------------------------------------------------------
//| Reads a dword from the stream
//+-----------------------------------------------------------------------------
DWORD DATA_IN_STREAM::ReadDWord()
{
	DWORD DWord = 0;

	Read(reinterpret_cast<CHAR*>(&DWord), sizeof(DWORD));

	return DWord;
}

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "DataOutStream.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
DATA_OUT_STREAM::DATA_OUT_STREAM()
{
	Position = 0;
	FileName = "";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
DATA_OUT_STREAM::~DATA_OUT_STREAM()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Sets a new filename
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::SetFileName(CONST std::string& NewFileName)
{
	FileName = NewFileName;
}


//+-----------------------------------------------------------------------------
//| Returns the filename
//+-----------------------------------------------------------------------------
std::string DATA_OUT_STREAM::GetFileName()
{
	return FileName;
}


//+-----------------------------------------------------------------------------
//| Clears the data stream
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::Clear()
{
	Position = 0;
	DataBuffer.clear();
}


//+-----------------------------------------------------------------------------
//| Saves a file
//+-----------------------------------------------------------------------------
BOOL DATA_OUT_STREAM::Save(BUFFER& Buffer)
{
	if(!Buffer.Resize(Position))
	{
		Error.SetMessage("Unable to save \"" + FileName + "\", buffer resize failed!");
		return FALSE;
	}

	memcpy(Buffer.GetData(), &DataBuffer[0], Position);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the position of the stream pointer
//+-----------------------------------------------------------------------------
INT DATA_OUT_STREAM::GetPosition() CONST
{
	return Position;
}


//+-----------------------------------------------------------------------------
//| Writes data to the stream
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::Write(CONST CHAR* Buffer, INT NrOfBytes)
{
	if(NrOfBytes <= 0) return;

	DataBuffer.resize(static_cast<INT>(DataBuffer.size()) + NrOfBytes, 0);

	memcpy(&DataBuffer[Position], Buffer, NrOfBytes);
	Position += NrOfBytes;
}


//+-----------------------------------------------------------------------------
//| Writes a single character to the stream
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::WriteChar(CHAR Char)
{
	Write(reinterpret_cast<CHAR*>(&Char), sizeof(CHAR));
}


//+-----------------------------------------------------------------------------
//| Writes an integer to the stream
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::WriteInt(INT Int)
{
	Write(reinterpret_cast<CHAR*>(&Int), sizeof(INT));
}


//+-----------------------------------------------------------------------------
//| Writes a floating point number to the stream
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::WriteFloat(FLOAT Float)
{
	Write(reinterpret_cast<CHAR*>(&Float), sizeof(FLOAT));
}


//+-----------------------------------------------------------------------------
//| Writes a double precision floating point number to the stream
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::WriteDouble(DOUBLE Double)
{
	Write(reinterpret_cast<CHAR*>(&Double), sizeof(DOUBLE));
}


//+-----------------------------------------------------------------------------
//| Writes a byte to the stream
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::WriteByte(BYTE Byte)
{
	Write(reinterpret_cast<CHAR*>(&Byte), sizeof(BYTE));
}


//+-----------------------------------------------------------------------------
//| Writes a word to the stream
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::WriteWord(WORD Word)
{
	Write(reinterpret_cast<CHAR*>(&Word), sizeof(WORD));
}


//+-----------------------------------------------------------------------------
//| Writes a dword to the stream
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::WriteDWord(DWORD DWord)
{
	Write(reinterpret_cast<CHAR*>(&DWord), sizeof(DWORD));
}


//+-----------------------------------------------------------------------------
//| Writes a fixed length string to the stream
//+-----------------------------------------------------------------------------
VOID DATA_OUT_STREAM::WriteString(CONST std::string& String, INT Size)
{
	std::vector<CHAR> Buffer;

	Buffer.resize(Size + 1, 0);
	Buffer[Size] = '\0';

	//changed min to MIN
	memcpy(&Buffer[0], String.c_str(), min(Size, static_cast<INT>(String.size())));
	Buffer[static_cast<INT>(String.size())] = '\0';

	Write(&Buffer[0], Size);
}

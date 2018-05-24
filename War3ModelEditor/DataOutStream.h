//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_DATA_OUT_STREAM_H
#define MAGOS_DATA_OUT_STREAM_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Buffer.h"


//+-----------------------------------------------------------------------------
//| Data out stream class
//+-----------------------------------------------------------------------------
class DATA_OUT_STREAM
{
	public:
		CONSTRUCTOR DATA_OUT_STREAM();
		DESTRUCTOR ~DATA_OUT_STREAM();

		VOID SetFileName(CONST std::string& NewFileName);
		std::string GetFileName();

		VOID Clear();

		BOOL Save(BUFFER& Buffer);

		INT GetPosition() CONST;

		VOID Write(CONST CHAR* Buffer, INT NrOfBytes);

		VOID WriteChar(CHAR Char);
		VOID WriteInt(INT Int);
		VOID WriteFloat(FLOAT Float);
		VOID WriteDouble(DOUBLE Double);
		VOID WriteByte(BYTE Byte);
		VOID WriteWord(WORD Word);
		VOID WriteDWord(DWORD DWord);
		VOID WriteString(CONST std::string& String, INT Size);

	protected:
		INT Position;
		std::vector<CHAR> DataBuffer;
		std::string FileName;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

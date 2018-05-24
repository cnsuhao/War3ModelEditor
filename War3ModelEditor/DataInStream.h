//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_DATA_IN_STREAM_H
#define MAGOS_DATA_IN_STREAM_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Buffer.h"


//+-----------------------------------------------------------------------------
//| Data in stream class
//+-----------------------------------------------------------------------------
class DATA_IN_STREAM
{
	public:
		CONSTRUCTOR DATA_IN_STREAM();
		DESTRUCTOR ~DATA_IN_STREAM();

		VOID SetFileName(CONST std::string& NewFileName);
		std::string GetFileName();

		VOID Clear();

		BOOL Load(BUFFER& Buffer);

		BOOL Eof() CONST;

		INT GetPosition() CONST;
		VOID SetPosition(INT NewPosition, BOOL FromEnd = FALSE);

		BOOL Read(CHAR* Buffer, INT NrOfBytes);

		CHAR ReadChar();
		INT ReadInt();
		FLOAT ReadFloat();
		DOUBLE ReadDouble();
		BYTE ReadByte();
		WORD ReadWord();
		DWORD ReadDWord();

	protected:
		INT Position;
		BUFFER DataBuffer;
		std::string FileName;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

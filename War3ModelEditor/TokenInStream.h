//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TOKEN_IN_STREAM_H
#define MAGOS_TOKEN_IN_STREAM_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Buffer.h"


//+-----------------------------------------------------------------------------
//| Token in stream class
//+-----------------------------------------------------------------------------
class TOKEN_IN_STREAM
{
	public:
		CONSTRUCTOR TOKEN_IN_STREAM();
		DESTRUCTOR ~TOKEN_IN_STREAM();

		VOID SetFileName(CONST std::string& NewFileName);
		std::string GetFileName();

		VOID SetGroup(CONST std::string& NewGroup);
		std::string GetGroup();

		VOID Clear();

		BOOL Load(BUFFER& Buffer);
		BOOL Load(CONST std::string& String);

		BOOL Eof() CONST;

		BOOL ReadBool();
		CHAR ReadChar();
		INT ReadInt();
		FLOAT ReadFloat();
		DOUBLE ReadDouble();
		std::string ReadWord();
		std::string ReadLine();
		std::string ReadString();

		BOOL ReadVector2(D3DXVECTOR2& Vector);
		BOOL ReadVector3(D3DXVECTOR3& Vector);
		BOOL ReadVector4(D3DXVECTOR4& Vector);

		BOOL ReadVector2(D3DXVECTOR4& Vector);
		BOOL ReadVector3(D3DXVECTOR4& Vector);

		BOOL ExpectChar(CHAR Char);
		BOOL ExpectWord(CONST std::string& Word);

	protected:
		BOOL IsComment(std::string String);

		std::stringstream Stream;
		std::string FileName;
		std::string Group;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

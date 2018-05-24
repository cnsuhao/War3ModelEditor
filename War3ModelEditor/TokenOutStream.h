//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TOKEN_OUT_STREAM_H
#define MAGOS_TOKEN_OUT_STREAM_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Buffer.h"


//+-----------------------------------------------------------------------------
//| Token out stream class
//+-----------------------------------------------------------------------------
class TOKEN_OUT_STREAM
{
	public:
		CONSTRUCTOR TOKEN_OUT_STREAM();
		DESTRUCTOR ~TOKEN_OUT_STREAM();

		VOID SetFileName(CONST std::string& NewFileName);
		std::string GetFileName();

		VOID Clear();

		BOOL Save(BUFFER& Buffer);

		VOID WriteBool(BOOL Bool);
		VOID WriteChar(CHAR Char);
		VOID WriteInt(INT Int);
		VOID WriteFloat(FLOAT Float);
		VOID WriteDouble(DOUBLE Double);
		VOID WriteWord(CONST std::string& Word);
		VOID WriteLine(CONST std::string& Line);
		VOID WriteString(CONST std::string& String);

		VOID WriteVector2(CONST D3DXVECTOR2& Vector);
		VOID WriteVector3(CONST D3DXVECTOR3& Vector);
		VOID WriteVector4(CONST D3DXVECTOR4& Vector);

		VOID WriteVector2(CONST D3DXVECTOR4& Vector);
		VOID WriteVector3(CONST D3DXVECTOR4& Vector);

		VOID WriteHeader(CONST std::string& Title);

		VOID WriteBreak(INT NrOfBreaks = 1);
		VOID WriteTab(INT NrOfTabs = 1);

	protected:
		std::stringstream Stream;
		std::string FileName;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

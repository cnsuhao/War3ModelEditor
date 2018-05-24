//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_BUFFER_H
#define MAGOS_BUFFER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Buffer class
//+-----------------------------------------------------------------------------
class BUFFER
{
	public:
		CONSTRUCTOR BUFFER();
		DESTRUCTOR ~BUFFER();

		VOID Clear();
		BOOL Resize(INT NewSize);

		CHAR* GetData() CONST;
		INT GetSize() CONST;

		BUFFER& operator =(CONST BUFFER& CopyObject);
		BUFFER& operator =(CONST std::string& CopyString);

		CHAR& operator [](INT Index) CONST;

	protected:
		CHAR* Data;
		INT Size;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Buffer.h"

//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
BUFFER::BUFFER()
{
	Data = NULL;
	Size = 0;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
BUFFER::~BUFFER()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the buffer
//+-----------------------------------------------------------------------------
VOID BUFFER::Clear()
{
	SAFE_ARRAY_DELETE(Data);
	Size = 0;
}


//+-----------------------------------------------------------------------------
//| Resizes the buffer
//+-----------------------------------------------------------------------------
BOOL BUFFER::Resize(INT NewSize)
{
	Clear();

	if(NewSize < 0)
	{
		Error.SetMessage("Unable to resize buffer, size is too small!");
		return FALSE;
	}

	Data = new CHAR[NewSize + 1];
	if(Data == NULL)
	{
		Error.SetMessage("Unable to resize buffer, memory allocation failed!");
		return FALSE;
	}

	Size = NewSize;
	Data[Size] = '\0';

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the data from the buffer
//+-----------------------------------------------------------------------------
CHAR* BUFFER::GetData() CONST
{
	return Data;
}


//+-----------------------------------------------------------------------------
//| Returns the buffer size
//+-----------------------------------------------------------------------------
INT BUFFER::GetSize() CONST
{
	return Size;
}


//+-----------------------------------------------------------------------------
//| Assigns the contents from another buffer
//+-----------------------------------------------------------------------------
BUFFER& BUFFER::operator =(CONST BUFFER& CopyObject)
{
	Resize(CopyObject.Size);
	memcpy(Data, CopyObject.GetData(), Size);

	return (*this);
}


//+-----------------------------------------------------------------------------
//| Assigns the contents from a string
//+-----------------------------------------------------------------------------
BUFFER& BUFFER::operator =(CONST std::string& CopyString)
{
	Resize(static_cast<INT>(CopyString.size()));
	memcpy(Data, CopyString.c_str(), Size);

	return (*this);
}


//+-----------------------------------------------------------------------------
//| References a byte in the buffer
//+-----------------------------------------------------------------------------
CHAR& BUFFER::operator [](INT Index) CONST
{
	return Data[Index];
}

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_SIMPLE_CONTAINER_H
#define MAGOS_SIMPLE_CONTAINER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Simple container node structure
//+-----------------------------------------------------------------------------
template <class TYPE>
struct SIMPLE_CONTAINER_NODE
{
	SIMPLE_CONTAINER_NODE()
	{
		//Empty
	}

	TYPE Data;
};


//+-----------------------------------------------------------------------------
//| Simple container class
//+-----------------------------------------------------------------------------
template <class TYPE>
class SIMPLE_CONTAINER
{
	public:
		CONSTRUCTOR SIMPLE_CONTAINER();
		CONSTRUCTOR SIMPLE_CONTAINER(CONST SIMPLE_CONTAINER<TYPE>& CopyObject);
		DESTRUCTOR ~SIMPLE_CONTAINER();

		VOID Clear();

		VOID Reserve(INT Size);

		BOOL Add(CONST TYPE& NewObject);
		BOOL Remove(INT Index);
		BOOL Replace(INT Index, CONST TYPE& NewObject);

		CONST SIMPLE_CONTAINER<TYPE>& operator =(CONST SIMPLE_CONTAINER<TYPE>& CopyObject);

		TYPE& operator [](INT Index) CONST;

		TYPE* Get(INT Index) CONST;

		INT GetSize() CONST;
		INT GetTotalSize() CONST;
		INT GetLastAddedIndex() CONST;

		BOOL ValidIndex(INT Index) CONST;

	protected:
		VOID Copy(CONST SIMPLE_CONTAINER<TYPE>& CopyObject);

		INT GetFreeIndex();

		std::vector<SIMPLE_CONTAINER_NODE<TYPE>*> DataList;
		std::list<INT> FreeIndexList;

		INT Size;
		INT LastAddedIndex;
};


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
template <class TYPE>
SIMPLE_CONTAINER<TYPE>::SIMPLE_CONTAINER()
{
	Size = 0;
	LastAddedIndex = INVALID_INDEX;
}


//+-----------------------------------------------------------------------------
//| Copy constructor
//+-----------------------------------------------------------------------------
template <class TYPE>
SIMPLE_CONTAINER<TYPE>::SIMPLE_CONTAINER(CONST SIMPLE_CONTAINER<TYPE>& CopyObject)
{
	Copy(CopyObject);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
template <class TYPE>
SIMPLE_CONTAINER<TYPE>::~SIMPLE_CONTAINER()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the container
//+-----------------------------------------------------------------------------
template <class TYPE>
VOID SIMPLE_CONTAINER<TYPE>::Clear()
{
	INT i;

	for(i = 0; i < static_cast<INT>(DataList.size()); i++)
	{
		delete DataList[i];
	}

	DataList.clear();
	FreeIndexList.clear();

	Size = 0;
	LastAddedIndex = INVALID_INDEX;
}


//+-----------------------------------------------------------------------------
//| Reserves space in the container
//+-----------------------------------------------------------------------------
template <class TYPE>
VOID SIMPLE_CONTAINER<TYPE>::Reserve(INT Size)
{
	DataList.reserve(Size);
}


//+-----------------------------------------------------------------------------
//| Adds an object to the container
//+-----------------------------------------------------------------------------
template <class TYPE>
BOOL SIMPLE_CONTAINER<TYPE>::Add(CONST TYPE& NewObject)
{
	INT Index;

	Index = GetFreeIndex();
	LastAddedIndex = Index;

	DataList[Index] = new SIMPLE_CONTAINER_NODE<TYPE>();
	if(DataList[Index] == NULL) return FALSE;

	DataList[Index]->Data = NewObject;

	Size++;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an object from the container
//+-----------------------------------------------------------------------------
template <class TYPE>
BOOL SIMPLE_CONTAINER<TYPE>::Remove(INT Index)
{
	if(!ValidIndex(Index)) return FALSE;

	delete DataList[Index];
	DataList[Index] = NULL;

	FreeIndexList.push_back(Index);

	Size--;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Replaces an object in the container
//+-----------------------------------------------------------------------------
template <class TYPE>
BOOL SIMPLE_CONTAINER<TYPE>::Replace(INT Index, CONST TYPE& NewObject)
{
	if(!ValidIndex(Index)) return FALSE;

	DataList[Index]->Data = NewObject;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Copies another container
//+-----------------------------------------------------------------------------
template <class TYPE>
CONST SIMPLE_CONTAINER<TYPE>& SIMPLE_CONTAINER<TYPE>::operator =(CONST SIMPLE_CONTAINER<TYPE>& CopyObject)
{
	Copy(CopyObject);

	return (*this);
}


//+-----------------------------------------------------------------------------
//| Returns an object from the container
//+-----------------------------------------------------------------------------
template <class TYPE>
TYPE& SIMPLE_CONTAINER<TYPE>::operator [](INT Index) CONST
{
	return DataList[Index]->Data;
}


//+-----------------------------------------------------------------------------
//| Returns an object from the container
//+-----------------------------------------------------------------------------
template <class TYPE>
TYPE* SIMPLE_CONTAINER<TYPE>::Get(INT Index) CONST
{
	if(!ValidIndex(Index)) return NULL;

	return &(DataList[Index]->Data);
}


//+-----------------------------------------------------------------------------
//| Returns the nr of objects in the container
//+-----------------------------------------------------------------------------
template <class TYPE>
INT SIMPLE_CONTAINER<TYPE>::GetSize() CONST
{
	return Size;
}


//+-----------------------------------------------------------------------------
//| Returns the total size of the container
//+-----------------------------------------------------------------------------
template <class TYPE>
INT SIMPLE_CONTAINER<TYPE>::GetTotalSize() CONST
{
	return static_cast<INT>(DataList.size());
}


//+-----------------------------------------------------------------------------
//| Returns the index of the last added object
//+-----------------------------------------------------------------------------
template <class TYPE>
INT SIMPLE_CONTAINER<TYPE>::GetLastAddedIndex() CONST
{
	return LastAddedIndex;
}


//+-----------------------------------------------------------------------------
//| Checks if an index is valid
//+-----------------------------------------------------------------------------
template <class TYPE>
BOOL SIMPLE_CONTAINER<TYPE>::ValidIndex(INT Index) CONST
{
	if(Index < 0) return FALSE;
	if(Index >= static_cast<INT>(DataList.size())) return FALSE;
	if(DataList[Index] == NULL) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Copies another container
//+-----------------------------------------------------------------------------
template <class TYPE>
VOID SIMPLE_CONTAINER<TYPE>::Copy(CONST SIMPLE_CONTAINER<TYPE>& CopyObject)
{
	INT i;
	INT ListSize;

	ListSize = static_cast<INT>(CopyObject.DataList.size());
	DataList.resize(ListSize, NULL);

	for(i = 0; i < ListSize; i++)
	{
		if(CopyObject.DataList[i] != NULL)
		{
			DataList[i] = new SIMPLE_CONTAINER_NODE<TYPE>(*(CopyObject.DataList[i]));
		}
	}

	FreeIndexList = CopyObject.FreeIndexList;

	Size = CopyObject.Size;
	LastAddedIndex = CopyObject.LastAddedIndex;
}


//+-----------------------------------------------------------------------------
//| Creates a new free index for an object
//+-----------------------------------------------------------------------------
template <class TYPE>
INT SIMPLE_CONTAINER<TYPE>::GetFreeIndex()
{
	INT FreeIndex;

	if(FreeIndexList.size() > 0)
	{
		FreeIndex = FreeIndexList.back();
		FreeIndexList.pop_back();
	}
	else
	{
		FreeIndex = static_cast<INT>(DataList.size());
		DataList.push_back(NULL);
	}

	return FreeIndex;
}


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

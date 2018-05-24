//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_REFERENCE_OBJECT_H
#define MAGOS_REFERENCE_OBJECT_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE> class REFERENCE;


//+-----------------------------------------------------------------------------
//| Reference object class
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
class REFERENCE_OBJECT
{
	public:
		CONSTRUCTOR REFERENCE_OBJECT();
		CONSTRUCTOR REFERENCE_OBJECT(CONST REFERENCE_OBJECT<TYPE, OBJECT_TYPE>& CopyObject);
		DESTRUCTOR ~REFERENCE_OBJECT();

		CONST REFERENCE_OBJECT<TYPE, OBJECT_TYPE>& operator =(CONST REFERENCE_OBJECT<TYPE, OBJECT_TYPE>& CopyObject);

		VOID Clear();

		VOID SetData(OBJECT_TYPE NewData);
		OBJECT_TYPE GetData() CONST;

		REFERENCE<TYPE, OBJECT_TYPE>* GetFirstReference() CONST;
		REFERENCE<TYPE, OBJECT_TYPE>* GetLastReference() CONST;
		REFERENCE<TYPE, OBJECT_TYPE>* GetNextReference(REFERENCE<TYPE, OBJECT_TYPE>* Reference) CONST;
		REFERENCE<TYPE, OBJECT_TYPE>* GetPreviousReference(REFERENCE<TYPE, OBJECT_TYPE>* Reference) CONST;

		INT GetReferenceCount() CONST;

	protected:
		VOID Add(REFERENCE<TYPE, OBJECT_TYPE>* Reference);
		VOID Remove(REFERENCE<TYPE, OBJECT_TYPE>* Reference);

		REFERENCE<TYPE, OBJECT_TYPE>* FirstReference;
		REFERENCE<TYPE, OBJECT_TYPE>* LastReference;

		INT ReferenceCount;

		OBJECT_TYPE Data;

		friend class REFERENCE<TYPE, OBJECT_TYPE>;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Reference.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::REFERENCE_OBJECT()
{
	FirstReference = NULL;
	LastReference = NULL;

	ReferenceCount = 0;
}


//+-----------------------------------------------------------------------------
//| Copy constructor
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::REFERENCE_OBJECT(CONST REFERENCE_OBJECT<TYPE, OBJECT_TYPE>& CopyObject)
{
	FirstReference = NULL;
	LastReference = NULL;

	ReferenceCount = 0;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::~REFERENCE_OBJECT()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Assignment operator
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
CONST REFERENCE_OBJECT<TYPE, OBJECT_TYPE>& REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::operator =(CONST REFERENCE_OBJECT<TYPE, OBJECT_TYPE>& CopyObject)
{
	FirstReference = NULL;
	LastReference = NULL;

	ReferenceCount = 0;

	return (*this);
}


//+-----------------------------------------------------------------------------
//| Clears all attached references
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
VOID REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::Clear()
{
	REFERENCE<TYPE, OBJECT_TYPE>* CurrentReference;
	REFERENCE<TYPE, OBJECT_TYPE>* NextReference;

	CurrentReference = FirstReference;
	while(CurrentReference != NULL)
	{
		NextReference = CurrentReference->NextReference;

		CurrentReference->ReferenceObject = NULL;
		CurrentReference->NextReference = NULL;
		CurrentReference->PreviousReference = NULL;

		CurrentReference = NextReference;
	}

	FirstReference = NULL;
	LastReference = NULL;

	ReferenceCount = 0;
}


//+-----------------------------------------------------------------------------
//| Sets a new data
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
VOID REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::SetData(OBJECT_TYPE NewData)
{
	Data = NewData;
}


//+-----------------------------------------------------------------------------
//| Returns the data
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
OBJECT_TYPE REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::GetData() CONST
{
	return Data;
}


//+-----------------------------------------------------------------------------
//| Returns the first reference
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
REFERENCE<TYPE, OBJECT_TYPE>* REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::GetFirstReference() CONST
{
	return FirstReference;
}


//+-----------------------------------------------------------------------------
//| Returns the last reference
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
REFERENCE<TYPE, OBJECT_TYPE>* REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::GetLastReference() CONST
{
	return LastReference;
}


//+-----------------------------------------------------------------------------
//| Returns the next reference
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
REFERENCE<TYPE, OBJECT_TYPE>* REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::GetNextReference(REFERENCE<TYPE, OBJECT_TYPE>* Reference) CONST
{
	if(Reference == NULL) return NULL;

	return Reference->NextReference;
}


//+-----------------------------------------------------------------------------
//| Returns the previous reference
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
REFERENCE<TYPE, OBJECT_TYPE>* REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::GetPreviousReference(REFERENCE<TYPE, OBJECT_TYPE>* Reference) CONST
{
	if(Reference == NULL) return NULL;

	return Reference->PreviousReference;
}


//+-----------------------------------------------------------------------------
//| Returns the reference counter
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
INT REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::GetReferenceCount() CONST
{
	return ReferenceCount;
}


//+-----------------------------------------------------------------------------
//| Adds a reference to the reference object
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
VOID REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::Add(REFERENCE<TYPE, OBJECT_TYPE>* Reference)
{
	Reference->NextReference = NULL;
	Reference->PreviousReference = LastReference;

	if(FirstReference == NULL) FirstReference = Reference;
	if(LastReference != NULL) LastReference->NextReference = Reference;
	LastReference = Reference;

	ReferenceCount++;
}


//+-----------------------------------------------------------------------------
//| Removes a reference from the reference object
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
VOID REFERENCE_OBJECT<TYPE, OBJECT_TYPE>::Remove(REFERENCE<TYPE, OBJECT_TYPE>* Reference)
{
	if(Reference->NextReference == NULL)
	{
		if(Reference->PreviousReference == NULL)
		{
			FirstReference = NULL;
			LastReference = NULL;
		}
		else
		{
			LastReference = Reference->PreviousReference;
			LastReference->NextReference = NULL;
		}
	}
	else
	{
		if(Reference->PreviousReference == NULL)
		{
			FirstReference = Reference->NextReference;
			FirstReference->PreviousReference = NULL;
		}
		else
		{
			Reference->PreviousReference->NextReference = Reference->NextReference;
			Reference->NextReference->PreviousReference = Reference->PreviousReference;
		}
	}

	ReferenceCount--;
}


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

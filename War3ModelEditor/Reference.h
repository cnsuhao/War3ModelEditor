//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_REFERENCE_H
#define MAGOS_REFERENCE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ReferenceObject.h"


//+-----------------------------------------------------------------------------
//| Reference class
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
class REFERENCE
{
	public:
		CONSTRUCTOR REFERENCE();
		CONSTRUCTOR REFERENCE(CONST REFERENCE<TYPE, OBJECT_TYPE>& CopyObject);
		DESTRUCTOR ~REFERENCE();

		CONST REFERENCE<TYPE, OBJECT_TYPE>& operator =(CONST REFERENCE<TYPE, OBJECT_TYPE>& CopyObject);

		VOID SetData(TYPE NewData);
		TYPE GetData() CONST;
		OBJECT_TYPE GetObjectData() CONST;

		VOID Attach(REFERENCE_OBJECT<TYPE, OBJECT_TYPE>& NewReferenceObject);
		VOID Detach();

		BOOL IsAttached() CONST;

	protected:
		REFERENCE_OBJECT<TYPE, OBJECT_TYPE>* ReferenceObject;
		REFERENCE<TYPE, OBJECT_TYPE>* NextReference;
		REFERENCE<TYPE, OBJECT_TYPE>* PreviousReference;

		TYPE Data;

		friend class REFERENCE_OBJECT<TYPE, OBJECT_TYPE>;
};


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
REFERENCE<TYPE, OBJECT_TYPE>::REFERENCE()
{
	ReferenceObject = NULL;
	NextReference = NULL;
	PreviousReference = NULL;
}


//+-----------------------------------------------------------------------------
//| Copy constructor
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
REFERENCE<TYPE, OBJECT_TYPE>::REFERENCE(CONST REFERENCE<TYPE, OBJECT_TYPE>& CopyObject)
{
	ReferenceObject = NULL;
	NextReference = NULL;
	PreviousReference = NULL;

	if(CopyObject.IsAttached()) Attach(*CopyObject.GetObjectData());
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
REFERENCE<TYPE, OBJECT_TYPE>::~REFERENCE()
{
	Detach();
}


//+-----------------------------------------------------------------------------
//| Assignment operator
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
CONST REFERENCE<TYPE, OBJECT_TYPE>& REFERENCE<TYPE, OBJECT_TYPE>::operator =(CONST REFERENCE<TYPE, OBJECT_TYPE>& CopyObject)
{
	Detach();

	if(CopyObject.IsAttached()) Attach(*CopyObject.ReferenceObject);

	return (*this);
}


//+-----------------------------------------------------------------------------
//| Sets a new data
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
VOID REFERENCE<TYPE, OBJECT_TYPE>::SetData(TYPE NewData)
{
	Data = NewData;
}


//+-----------------------------------------------------------------------------
//| Returns the data
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
TYPE REFERENCE<TYPE, OBJECT_TYPE>::GetData() CONST
{
	return Data;
}


//+-----------------------------------------------------------------------------
//| Returns the object data
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
OBJECT_TYPE REFERENCE<TYPE, OBJECT_TYPE>::GetObjectData() CONST
{
	return ReferenceObject->GetData();
}


//+-----------------------------------------------------------------------------
//| Attaches the reference to a reference object
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
VOID REFERENCE<TYPE, OBJECT_TYPE>::Attach(REFERENCE_OBJECT<TYPE, OBJECT_TYPE>& NewReferenceObject)
{
	Detach();

	ReferenceObject = &NewReferenceObject;
	ReferenceObject->Add(this);
}


//+-----------------------------------------------------------------------------
//| Detaches the reference from its reference object
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
VOID REFERENCE<TYPE, OBJECT_TYPE>::Detach()
{
	if(ReferenceObject != NULL)
	{
		ReferenceObject->Remove(this);
		ReferenceObject = NULL;
	}

	NextReference = NULL;
	PreviousReference = NULL;
}


//+-----------------------------------------------------------------------------
//| Checks if the reference is attached
//+-----------------------------------------------------------------------------
template <class TYPE, class OBJECT_TYPE>
BOOL REFERENCE<TYPE, OBJECT_TYPE>::IsAttached() CONST
{
	return (ReferenceObject != NULL);
}


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

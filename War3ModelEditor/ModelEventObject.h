//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_EVENT_OBJECT_H
#define MAGOS_MODEL_EVENT_OBJECT_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model event object data structure
//+-----------------------------------------------------------------------------
struct MODEL_EVENT_OBJECT_DATA : public MODEL_BASE_DATA
{
	MODEL_EVENT_OBJECT_DATA()
	{
		GlobalSequenceId = INVALID_INDEX;
	}

	INT GlobalSequenceId;

	std::list<INT> EventTrack;
};


//+-----------------------------------------------------------------------------
//| Model event object class
//+-----------------------------------------------------------------------------
class MODEL_EVENT_OBJECT : public MODEL_BASE
{
	public:
		CONSTRUCTOR MODEL_EVENT_OBJECT();
		DESTRUCTOR ~MODEL_EVENT_OBJECT();

		VOID Clear();
		INT GetSize();

		MODEL_EVENT_OBJECT_DATA& Data();

	protected:
		MODEL_EVENT_OBJECT_DATA EventObjectData;

	public:
		REFERENCE<MODEL_EVENT_OBJECT*, MODEL_GLOBAL_SEQUENCE*> GlobalSequenceNode;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

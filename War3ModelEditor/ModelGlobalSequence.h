//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_GLOBAL_SEQUENCE_H
#define MAGOS_MODEL_GLOBAL_SEQUENCE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model global sequence data structure
//+-----------------------------------------------------------------------------
struct MODEL_GLOBAL_SEQUENCE_DATA
{
	MODEL_GLOBAL_SEQUENCE_DATA()
	{
		Duration = 0;
		Time = 0;

		InternalGlobalSequenceId = INVALID_INDEX;
	}

	INT Duration;
	INT Time;

	INT InternalGlobalSequenceId;
};


//+-----------------------------------------------------------------------------
//| Model global sequence class
//+-----------------------------------------------------------------------------
class MODEL_GLOBAL_SEQUENCE
{
	public:
		CONSTRUCTOR MODEL_GLOBAL_SEQUENCE();
		DESTRUCTOR ~MODEL_GLOBAL_SEQUENCE();

		VOID Clear();
		INT GetSize();

		MODEL_GLOBAL_SEQUENCE_DATA& Data();

	protected:
		MODEL_GLOBAL_SEQUENCE_DATA GlobalSequenceData;

	public:
		REFERENCE_OBJECT<MODEL_EVENT_OBJECT*, MODEL_GLOBAL_SEQUENCE*> EventObjectNodes;
		REFERENCE_OBJECT<INTERPOLATOR*, MODEL_GLOBAL_SEQUENCE*> InterpolatorNodes;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

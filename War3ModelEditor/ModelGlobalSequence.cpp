//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelGlobalSequence.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_GLOBAL_SEQUENCE::MODEL_GLOBAL_SEQUENCE()
{
	EventObjectNodes.SetData(this);
	InterpolatorNodes.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_GLOBAL_SEQUENCE::~MODEL_GLOBAL_SEQUENCE()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the global sequence
//+-----------------------------------------------------------------------------
VOID MODEL_GLOBAL_SEQUENCE::Clear()
{
	GlobalSequenceData = MODEL_GLOBAL_SEQUENCE_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the global sequence
//+-----------------------------------------------------------------------------
INT MODEL_GLOBAL_SEQUENCE::GetSize()
{
	return 4;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_GLOBAL_SEQUENCE_DATA& MODEL_GLOBAL_SEQUENCE::Data()
{
	return GlobalSequenceData;
}

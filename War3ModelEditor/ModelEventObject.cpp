//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelEventObject.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_EVENT_OBJECT::MODEL_EVENT_OBJECT()
{
	ModelBaseData = &EventObjectData;
	ModelBaseData->Type = NODE_TYPE_EVENT_OBJECT;

	GlobalSequenceNode.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_EVENT_OBJECT::~MODEL_EVENT_OBJECT()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the event object
//+-----------------------------------------------------------------------------
VOID MODEL_EVENT_OBJECT::Clear()
{
	EventObjectData = MODEL_EVENT_OBJECT_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the event object
//+-----------------------------------------------------------------------------
INT MODEL_EVENT_OBJECT::GetSize()
{
	return GetBaseSize() + (4 * static_cast<INT>(EventObjectData.EventTrack.size())) + 12;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_EVENT_OBJECT_DATA& MODEL_EVENT_OBJECT::Data()
{
	return EventObjectData;
}

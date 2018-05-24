//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ObjectIdManager.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
OBJECT_ID_MANAGER ObjectIdManager;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
OBJECT_ID_MANAGER::OBJECT_ID_MANAGER()
{
	NextObjectId = 0;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
OBJECT_ID_MANAGER::~OBJECT_ID_MANAGER()
{
	ClearAllObjectIds();
}


//+-----------------------------------------------------------------------------
//| Clears all object IDs
//+-----------------------------------------------------------------------------
VOID OBJECT_ID_MANAGER::ClearAllObjectIds()
{
	NextObjectId = 0;
	FreeObjectIdList.clear();
}


//+-----------------------------------------------------------------------------
//| Generates a new object ID
//+-----------------------------------------------------------------------------
INT OBJECT_ID_MANAGER::GetNewObjectId()
{
	INT NewObjectId;
	std::map<INT, INT>::iterator i;

	if(FreeObjectIdList.size() > 0)
	{
		i = FreeObjectIdList.begin();
		NewObjectId = i->first;
		FreeObjectIdList.erase(i);
	}
	else
	{
		NewObjectId = NextObjectId;
		NextObjectId++;
	}

	return NewObjectId;
}


//+-----------------------------------------------------------------------------
//| Adds an existing object ID
//+-----------------------------------------------------------------------------
VOID OBJECT_ID_MANAGER::AddObjectId(INT ObjectId)
{
	INT i;
	std::map<INT, INT>::iterator j;

	j = FreeObjectIdList.find(ObjectId);
	if(j != FreeObjectIdList.end())
	{
		FreeObjectIdList.erase(j);
		return;
	}

	if(ObjectId < NextObjectId) return;

	for(i = NextObjectId; i < ObjectId; i++)
	{
		FreeObjectIdList.insert(std::make_pair(i, i));
	}

	NextObjectId = ObjectId + 1;
}


//+-----------------------------------------------------------------------------
//| Removes an existing object ID
//+-----------------------------------------------------------------------------
VOID OBJECT_ID_MANAGER::RemoveObjectId(INT ObjectId)
{
	INT i;

	if(ObjectId == INVALID_INDEX) return;

	FreeObjectIdList.insert(std::make_pair(ObjectId, ObjectId));

	if(ObjectId < NextObjectId) return;

	for(i = NextObjectId; i < ObjectId; i++)
	{
		FreeObjectIdList.insert(std::make_pair(i, i));
	}
}


//+-----------------------------------------------------------------------------
//| Checks if an object ID exists
//+-----------------------------------------------------------------------------
BOOL OBJECT_ID_MANAGER::ValidObjectId(INT ObjectId) CONST
{
	std::map<INT, INT>::const_iterator i;

	if(ObjectId >= NextObjectId) return FALSE;

	i = FreeObjectIdList.find(ObjectId);
	if(i != FreeObjectIdList.end()) return FALSE;

	return TRUE;
}


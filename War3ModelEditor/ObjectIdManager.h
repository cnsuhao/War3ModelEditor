//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_OBJECT_ID_MANAGER_H
#define MAGOS_OBJECT_ID_MANAGER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Object ID manager class
//+-----------------------------------------------------------------------------
class OBJECT_ID_MANAGER
{
	public:
		CONSTRUCTOR OBJECT_ID_MANAGER();
		DESTRUCTOR ~OBJECT_ID_MANAGER();

		VOID ClearAllObjectIds();
		INT GetNewObjectId();
		VOID AddObjectId(INT ObjectId);
		VOID RemoveObjectId(INT ObjectId);
		BOOL ValidObjectId(INT ObjectId) CONST;

	protected:
		INT NextObjectId;

		std::map<INT, INT> FreeObjectIdList;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern OBJECT_ID_MANAGER ObjectIdManager;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelCollisionShape.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_COLLISION_SHAPE::MODEL_COLLISION_SHAPE()
{
	ModelBaseData = &CollisionShapeData;
	ModelBaseData->Type = NODE_TYPE_COLLISION_SHAPE;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_COLLISION_SHAPE::~MODEL_COLLISION_SHAPE()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the collision shape
//+-----------------------------------------------------------------------------
VOID MODEL_COLLISION_SHAPE::Clear()
{
	CollisionShapeData = MODEL_COLLISION_SHAPE_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the collision shape
//+-----------------------------------------------------------------------------
INT MODEL_COLLISION_SHAPE::GetSize()
{
	INT VertexListSize;
	INT RadiusSize;

	VertexListSize = static_cast<INT>(CollisionShapeData.VertexList.size()) * 12;
	RadiusSize = (CollisionShapeData.Type == COLLISION_SHAPE_TYPE_SPHERE) ? 4 : 0;

	return GetBaseSize() + VertexListSize + RadiusSize + 4;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_COLLISION_SHAPE_DATA& MODEL_COLLISION_SHAPE::Data()
{
	return CollisionShapeData;
}

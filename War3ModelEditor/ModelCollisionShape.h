//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_COLLISION_SHAPE_H
#define MAGOS_MODEL_COLLISION_SHAPE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Collision shape type enumeration
//+-----------------------------------------------------------------------------
enum COLLISION_SHAPE_TYPE
{
	COLLISION_SHAPE_TYPE_NONE,
	COLLISION_SHAPE_TYPE_BOX,
	COLLISION_SHAPE_TYPE_SPHERE,
};


//+-----------------------------------------------------------------------------
//| Model collision shape data structure
//+-----------------------------------------------------------------------------
struct MODEL_COLLISION_SHAPE_DATA : public MODEL_BASE_DATA
{
	MODEL_COLLISION_SHAPE_DATA()
	{
		Type = COLLISION_SHAPE_TYPE_BOX;
		BoundsRadius = 0.0f;

		VertexList.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		VertexList.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	COLLISION_SHAPE_TYPE Type;
	FLOAT BoundsRadius;
	std::list<D3DXVECTOR3> VertexList;
};


//+-----------------------------------------------------------------------------
//| Model collision shape class
//+-----------------------------------------------------------------------------
class MODEL_COLLISION_SHAPE : public MODEL_BASE
{
	public:
		CONSTRUCTOR MODEL_COLLISION_SHAPE();
		DESTRUCTOR ~MODEL_COLLISION_SHAPE();

		VOID Clear();
		INT GetSize();

		MODEL_COLLISION_SHAPE_DATA& Data();

	protected:
		MODEL_COLLISION_SHAPE_DATA CollisionShapeData;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

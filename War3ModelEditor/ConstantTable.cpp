//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ConstantTable.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
CONSTANT_TABLE::CONSTANT_TABLE()
{
	ConstantTable = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
CONSTANT_TABLE::~CONSTANT_TABLE()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Clears the constant table
//+-----------------------------------------------------------------------------
VOID CONSTANT_TABLE::Clear()
{
	SAFE_RELEASE(ConstantTable);
}


//+-----------------------------------------------------------------------------
//| Returns the constant table
//+-----------------------------------------------------------------------------
LPD3DXCONSTANTTABLE CONSTANT_TABLE::GetConstantTable() CONST
{
	return ConstantTable;
}


//+-----------------------------------------------------------------------------
//| Returns the constant table
//+-----------------------------------------------------------------------------
LPD3DXCONSTANTTABLE& CONSTANT_TABLE::operator ()()
{
	return ConstantTable;
}


//+-----------------------------------------------------------------------------
//| Sets an int constant
//+-----------------------------------------------------------------------------
VOID CONSTANT_TABLE::SetInt(CONST std::string& Name, INT Int)
{
	ConstantTable->SetInt(Graphics.GetDevice(), Name.c_str(), Int);
}


//+-----------------------------------------------------------------------------
//| Sets a float constant
//+-----------------------------------------------------------------------------
VOID CONSTANT_TABLE::SetFloat(CONST std::string& Name, FLOAT Float)
{
	ConstantTable->SetFloat(Graphics.GetDevice(), Name.c_str(), Float);
}


//+-----------------------------------------------------------------------------
//| Sets a 2d vector constant
//+-----------------------------------------------------------------------------
VOID CONSTANT_TABLE::SetVector2(CONST std::string& Name, CONST D3DXVECTOR2& Vector)
{
	D3DXVECTOR4 TempVector;

	TempVector.x = Vector.x;
	TempVector.y = Vector.y;
	TempVector.z = 0.0f;
	TempVector.w = 0.0f;

	ConstantTable->SetVector(Graphics.GetDevice(), Name.c_str(), &TempVector);
}


//+-----------------------------------------------------------------------------
//| Sets a 3d vector constant
//+-----------------------------------------------------------------------------
VOID CONSTANT_TABLE::SetVector3(CONST std::string& Name, CONST D3DXVECTOR3& Vector)
{
	D3DXVECTOR4 TempVector;

	TempVector.x = Vector.x;
	TempVector.y = Vector.y;
	TempVector.z = Vector.z;
	TempVector.w = 0.0f;

	ConstantTable->SetVector(Graphics.GetDevice(), Name.c_str(), &TempVector);
}


//+-----------------------------------------------------------------------------
//| Sets a 4d vector constant
//+-----------------------------------------------------------------------------
VOID CONSTANT_TABLE::SetVector4(CONST std::string& Name, CONST D3DXVECTOR4& Vector)
{
	ConstantTable->SetVector(Graphics.GetDevice(), Name.c_str(), &Vector);
}


//+-----------------------------------------------------------------------------
//| Sets a matrix constant
//+-----------------------------------------------------------------------------
VOID CONSTANT_TABLE::SetMatrix(CONST std::string& Name, CONST D3DXMATRIX& Matrix)
{
	ConstantTable->SetMatrix(Graphics.GetDevice(), Name.c_str(), &Matrix);
}

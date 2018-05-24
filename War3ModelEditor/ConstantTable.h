//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_CONSTANT_TABLE_H
#define MAGOS_CONSTANT_TABLE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Constant table class
//+-----------------------------------------------------------------------------
class CONSTANT_TABLE
{
	public:
		CONSTRUCTOR CONSTANT_TABLE();
		DESTRUCTOR ~CONSTANT_TABLE();

		VOID Clear();

		LPD3DXCONSTANTTABLE GetConstantTable() CONST;
		LPD3DXCONSTANTTABLE& operator ()();

		VOID SetInt(CONST std::string& Name, INT Int);
		VOID SetFloat(CONST std::string& Name, FLOAT Float);
		VOID SetVector2(CONST std::string& Name, CONST D3DXVECTOR2& Vector);
		VOID SetVector3(CONST std::string& Name, CONST D3DXVECTOR3& Vector);
		VOID SetVector4(CONST std::string& Name, CONST D3DXVECTOR4& Vector);
		VOID SetMatrix(CONST std::string& Name, CONST D3DXMATRIX& Matrix);

	protected:
		LPD3DXCONSTANTTABLE ConstantTable;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Graphics.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

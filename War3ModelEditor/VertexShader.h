//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_VERTEX_SHADER_H
#define MAGOS_VERTEX_SHADER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ConstantTable.h"


//+-----------------------------------------------------------------------------
//| Vertex shader class
//+-----------------------------------------------------------------------------
class VERTEX_SHADER
{
	public:
		CONSTRUCTOR VERTEX_SHADER();
		DESTRUCTOR ~VERTEX_SHADER();

		CONSTANT_TABLE& GetConstantTable();

		BOOL Setup(CONST std::string& ShaderFileName);
		VOID Shutdown();

		VOID Use();

	protected:
		LPDIRECT3DVERTEXSHADER9 VertexShader;
		CONSTANT_TABLE ConstantTable;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern VERTEX_SHADER VertexShader;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

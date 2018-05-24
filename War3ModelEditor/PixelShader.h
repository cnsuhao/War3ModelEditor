//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_PIXEL_SHADER_H
#define MAGOS_PIXEL_SHADER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ConstantTable.h"


//+-----------------------------------------------------------------------------
//| Pixel shader class
//+-----------------------------------------------------------------------------
class PIXEL_SHADER
{
	public:
		CONSTRUCTOR PIXEL_SHADER();
		DESTRUCTOR ~PIXEL_SHADER();

		CONSTANT_TABLE& GetConstantTable();

		BOOL Setup(CONST std::string& ShaderFileName);
		VOID Shutdown();

		VOID Use();

	protected:
		LPDIRECT3DPIXELSHADER9 PixelShader;
		CONSTANT_TABLE ConstantTable;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern PIXEL_SHADER PixelShaderShaded;
extern PIXEL_SHADER PixelShaderUnshaded;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

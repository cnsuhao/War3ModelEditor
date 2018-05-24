//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "PixelShader.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
PIXEL_SHADER PixelShaderShaded;
PIXEL_SHADER PixelShaderUnshaded;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
PIXEL_SHADER::PIXEL_SHADER()
{
	PixelShader = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
PIXEL_SHADER::~PIXEL_SHADER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Returns the constant table
//+-----------------------------------------------------------------------------
CONSTANT_TABLE& PIXEL_SHADER::GetConstantTable()
{
	return ConstantTable;
}


//+-----------------------------------------------------------------------------
//| Sets up the pixel shader
//+-----------------------------------------------------------------------------
BOOL PIXEL_SHADER::Setup(CONST std::string& ShaderFileName)
{
	std::string FileName;
	std::stringstream Stream;
	BUFFER Buffer;
	LPD3DXBUFFER Direct3DBuffer;
	LPD3DXBUFFER ErrorBuffer;
	LPDIRECT3DDEVICE9 Direct3DDevice;

	FileName = Common.GetProgramDirectory() + "\\" + ShaderFileName;

	Shutdown();

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL)
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", unable to retrieve the Direct3D device!");
		return FALSE;
	}

	if(!FileLoader.LoadFromFile(FileName, Buffer))
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", file not found!");
		return FALSE;
	}

	if(FAILED(D3DXCompileShader(reinterpret_cast<CHAR*>(Buffer.GetData()), Buffer.GetSize(), NULL, NULL, "main", PIXEL_SHADER_PROFILE.c_str(), 0, &Direct3DBuffer, &ErrorBuffer, &ConstantTable())))
	{
		Stream << "Unable to load \"" << FileName << "\", pixel shader compiling failed!\n";
		Stream << reinterpret_cast<CHAR*>(ErrorBuffer->GetBufferPointer());
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	if(FAILED(Direct3DDevice->CreatePixelShader(reinterpret_cast<DWORD*>(Direct3DBuffer->GetBufferPointer()), &PixelShader)))
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", pixel shader creation failed!");
		return FALSE;
	}

	SAFE_RELEASE(Direct3DBuffer);
	SAFE_RELEASE(ErrorBuffer);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Shuts down the pixel shader
//+-----------------------------------------------------------------------------
VOID PIXEL_SHADER::Shutdown()
{
	ConstantTable.Clear();
	SAFE_RELEASE(PixelShader);
}


//+-----------------------------------------------------------------------------
//| Uses this pixel shader
//+-----------------------------------------------------------------------------
VOID PIXEL_SHADER::Use()
{
	LPDIRECT3DDEVICE9 Direct3DDevice;

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL) return;

	Direct3DDevice->SetPixelShader(PixelShader);
}

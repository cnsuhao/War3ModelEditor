//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "VertexShader.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
VERTEX_SHADER VertexShader;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
VERTEX_SHADER::VERTEX_SHADER()
{
	VertexShader = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
VERTEX_SHADER::~VERTEX_SHADER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Returns the constant table
//+-----------------------------------------------------------------------------
CONSTANT_TABLE& VERTEX_SHADER::GetConstantTable()
{
	return ConstantTable;
}


//+-----------------------------------------------------------------------------
//| Sets up the vertex shader
//+-----------------------------------------------------------------------------
BOOL VERTEX_SHADER::Setup(CONST std::string& ShaderFileName)
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

	if(FAILED(D3DXCompileShader(reinterpret_cast<CHAR*>(Buffer.GetData()), Buffer.GetSize(), NULL, NULL, "main", VERTEX_SHADER_PROFILE.c_str(), 0, &Direct3DBuffer, &ErrorBuffer, &ConstantTable())))
	{
		Stream << "Unable to load \"" << FileName << "\", vertex shader compiling failed!\n";
		Stream << reinterpret_cast<CHAR*>(ErrorBuffer->GetBufferPointer());
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	if(FAILED(Direct3DDevice->CreateVertexShader(reinterpret_cast<DWORD*>(Direct3DBuffer->GetBufferPointer()), &VertexShader)))
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", vertex shader creation failed!");
		return FALSE;
	}

	SAFE_RELEASE(Direct3DBuffer);
	SAFE_RELEASE(ErrorBuffer);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Shuts down the vertex shader
//+-----------------------------------------------------------------------------
VOID VERTEX_SHADER::Shutdown()
{
	ConstantTable.Clear();
	SAFE_RELEASE(VertexShader);
}


//+-----------------------------------------------------------------------------
//| Uses this vertex shader
//+-----------------------------------------------------------------------------
VOID VERTEX_SHADER::Use()
{
	LPDIRECT3DDEVICE9 Direct3DDevice;

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL) return;

	Direct3DDevice->SetVertexShader(VertexShader);
}

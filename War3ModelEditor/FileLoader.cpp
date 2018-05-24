//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "FileLoader.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
FILE_LOADER FileLoader;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
FILE_LOADER::FILE_LOADER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
FILE_LOADER::~FILE_LOADER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Loads a file
//+-----------------------------------------------------------------------------
BOOL FILE_LOADER::Load(CONST std::string& FileName, BUFFER& Buffer)
{
	if(LoadFromFile(FileName, Buffer)) return TRUE;
	if(LoadFromMpq(Mpq, FileName, Buffer)) return TRUE;
	if(LoadFromMpq(MpqWar3Patch, FileName, Buffer)) return TRUE;
	if(LoadFromMpq(MpqWar3xLocal, FileName, Buffer)) return TRUE;
	if(LoadFromMpq(MpqWar3x, FileName, Buffer)) return TRUE;
	if(LoadFromMpq(MpqWar3, FileName, Buffer)) return TRUE;

	Error.SetMessage("Unable to load \"" + FileName + "\", file does not exist!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads a file from a physical file
//+-----------------------------------------------------------------------------
BOOL FILE_LOADER::LoadFromFile(CONST std::string& FileName, BUFFER& Buffer)
{
	INT Size;
	std::ifstream File;

	File.open(FileName.c_str(), std::ios::in | std::ios::binary);
	if(File.fail()) return FALSE;

	File.seekg(0, std::ios::end);
	Size = File.tellg();
	File.seekg(0, std::ios::beg);

	if(!Buffer.Resize(Size)) return FALSE;

	File.read(Buffer.GetData(), Buffer.GetSize());

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a file from an mpq archive
//+-----------------------------------------------------------------------------
BOOL FILE_LOADER::LoadFromMpq(MPQ& Mpq, CONST std::string& FileName, BUFFER& Buffer)
{
	if(!Mpq.LoadFile(FileName, Buffer))
	{
		Error.ClearMessage();
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a file to a physical file
//+-----------------------------------------------------------------------------
BOOL FILE_LOADER::SaveToFile(CONST std::string& FileName, BUFFER& Buffer)
{
	std::ofstream File;

	File.open(FileName.c_str(), std::ios::out | std::ios::binary);
	if(File.fail())
	{
		Error.SetMessage("Unable to save \"" + FileName + "\"!");
		return FALSE;
	}

	File.write(Buffer.GetData(), Buffer.GetSize());

	return TRUE;
}

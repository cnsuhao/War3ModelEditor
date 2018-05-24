//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Mpq.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MPQ Mpq;
MPQ MpqWar3;
MPQ MpqWar3x;
MPQ MpqWar3xLocal;
MPQ MpqWar3Patch;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MPQ::MPQ()
{
	MpqHandle = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MPQ::~MPQ()
{
	Close();
}


//+-----------------------------------------------------------------------------
//| Opens an mpq file
//+-----------------------------------------------------------------------------
BOOL MPQ::Open(CONST std::string& FileName)
{
	Close();

	if(!SFileOpenArchive(FileName.c_str(), 0, 0, &MpqHandle))
	{
		Close();
		Error.SetMessage("Unable to open \"" + FileName + "\"!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Closes the mpq file
//+-----------------------------------------------------------------------------
VOID MPQ::Close()
{
	if(MpqHandle != NULL)
	{
		SFileCloseArchive(MpqHandle);
		MpqHandle = NULL;
	}
}


//+-----------------------------------------------------------------------------
//| Returns the nr of files in the mpq
//+-----------------------------------------------------------------------------
INT MPQ::GetNrOfFiles()
{
	INT NrOfFiles;

	if(MpqHandle == NULL) return 0;

	NrOfFiles = SFileGetFileInfo(MpqHandle, SFILE_INFO_NUM_FILES);
	if(NrOfFiles <= 0) return 0;

	return NrOfFiles;
}


//+-----------------------------------------------------------------------------
//| Generates a list of all files in the mpq
//+-----------------------------------------------------------------------------
VOID MPQ::GetFileList(std::list<std::string>& FileList)
{
	INT i;
	INT HashTableSize;
	std::vector<FILELISTENTRY> FileListEntry;

	if(MpqHandle == NULL) return;

	HashTableSize = SFileGetFileInfo(MpqHandle, SFILE_INFO_HASH_TABLE_SIZE);
	if(HashTableSize <= 0) return;

	FileListEntry.resize(HashTableSize);
	if(!SFileListFiles(MpqHandle, NULL, &FileListEntry[0], 0)) return;

	for(i = 0; i < HashTableSize; i++)
	{
		if(FileListEntry[i].dwFileExists)
		{
			FileList.push_back(FileListEntry[i].szFileName);
		}
	}
}


//+-----------------------------------------------------------------------------
//| Checks if a file exists in the mpq
//+-----------------------------------------------------------------------------
BOOL MPQ::FileExists(CONST std::string& FileName)
{
	HANDLE FileHandle;

	if(MpqHandle == NULL) return FALSE;

	if(!SFileOpenFileEx(MpqHandle, FileName.c_str(), 0, &FileHandle)) return FALSE;
	SFileCloseFile(FileHandle);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the size of a file in the mpq
//+-----------------------------------------------------------------------------
INT MPQ::GetFileSize(CONST std::string& FileName)
{
	INT Size;
	HANDLE FileHandle;

	if(MpqHandle == NULL) return 0;

	if(!SFileOpenFileEx(MpqHandle, FileName.c_str(), 0, &FileHandle)) return 0;
	Size = SFileGetFileSize(FileHandle, NULL);
	SFileCloseFile(FileHandle);

	return Size;
}


//+-----------------------------------------------------------------------------
//| Loads a file from the mpq to a buffer
//+-----------------------------------------------------------------------------
BOOL MPQ::LoadFile(CONST std::string& FileName, BUFFER& Buffer)
{
	DWORD Size;
	HANDLE FileHandle;
	DWORD BytesRead = 0;

	if(MpqHandle == NULL)
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", no MPQ file is open!");
		return FALSE;
	}

	if(!SFileOpenFileEx(MpqHandle, FileName.c_str(), 0, &FileHandle)) return 0;

	Size = SFileGetFileSize(FileHandle, NULL);
	if(!Buffer.Resize(Size))
	{
		SFileCloseFile(FileHandle);
		Error.SetMessage("Unable to resize buffer!");
		return FALSE;
	}

	if(!SFileReadFile(FileHandle, Buffer.GetData(), Buffer.GetSize(), &BytesRead, NULL))
	{
		SFileCloseFile(FileHandle);
		Error.SetMessage("Unable to read data from \"" + FileName + "\"!");
		return FALSE;
	}

	SFileCloseFile(FileHandle);

	if(BytesRead != Size)
	{
		Error.SetMessage("Not all bytes were read from \"" + FileName + "\"!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a file from the mpq to a real file
//+-----------------------------------------------------------------------------
BOOL MPQ::ExportFile(CONST std::string& FileName, CONST std::string& RealFileName)
{
	BUFFER Buffer;
	std::ofstream File;

	if(MpqHandle == NULL)
	{
		Error.SetMessage("Unable to export \"" + FileName + "\" as \"" + RealFileName + "\", no MPQ file is open!");
		return FALSE;
	}

	if(!LoadFile(FileName, Buffer)) return FALSE;

	File.open(RealFileName.c_str(), std::ios::out | std::ios::binary);
	if(File.fail())
	{
		Error.SetMessage("Unable to open \"" + RealFileName + "\" for writing!");
		return FALSE;
	}

	File.write(Buffer.GetData(), Buffer.GetSize());
	File.close();

	return TRUE;
}

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TEMPORARY_FILE_H
#define MAGOS_TEMPORARY_FILE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "FileLoader.h"


//+-----------------------------------------------------------------------------
//| Structure of a file process
//+-----------------------------------------------------------------------------
struct FILE_PROCESS
{
	FILE_PROCESS(HANDLE NewHandle, CONST std::string& NewFileName)
	{
		Handle = NewHandle;
		FileName = NewFileName;
	}
	
	HANDLE Handle;
	std::string FileName;
};


//+-----------------------------------------------------------------------------
//| Temporary file class
//+-----------------------------------------------------------------------------
class TEMPORARY_FILE
{
	public:
		CONSTRUCTOR TEMPORARY_FILE();
		DESTRUCTOR ~TEMPORARY_FILE();

		BOOL CreateTemporaryFile(CONST std::string& FileName, BUFFER& Buffer);
		VOID DestroyTemporaryFile(CONST std::string& FileName);
		VOID DestroyAllTemporaryFiles();

		BOOL LaunchThread(FILE_PROCESS* FileProcess);

		std::string GetTemporaryDirectory();

	protected:
		VOID AssertTemporaryDirectory();
		BOOL DeleteFile(CONST std::string& FileName);

		HANDLE MutexLock;

		std::string TemporaryDirectory;
		std::set<std::string> FileNameList;

		static DWORD CALLBACK DestroyFileWhenNeccessary(DWORD Parameter);
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern TEMPORARY_FILE TemporaryFile;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TemporaryFile.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEMPORARY_FILE TemporaryFile;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEMPORARY_FILE::TEMPORARY_FILE()
{
	MutexLock = CreateMutex(NULL, FALSE, NULL);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEMPORARY_FILE::~TEMPORARY_FILE()
{
	DestroyAllTemporaryFiles();

	if(MutexLock != NULL)
	{
		CloseHandle(MutexLock);
		MutexLock = NULL;
	}
}


//+-----------------------------------------------------------------------------
//| Creates a temporary file
//+-----------------------------------------------------------------------------
BOOL TEMPORARY_FILE::CreateTemporaryFile(CONST std::string& FileName, BUFFER& Buffer)
{
	AssertTemporaryDirectory();

	if(!FileLoader.SaveToFile(FileName, Buffer)) return FALSE;

	if(MutexLock == NULL) return FALSE;
	WaitForSingleObject(MutexLock, INFINITE);

	FileNameList.insert(FileName);

	ReleaseMutex(MutexLock);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys a temporary file
//+-----------------------------------------------------------------------------
VOID TEMPORARY_FILE::DestroyTemporaryFile(CONST std::string& FileName)
{
	std::set<std::string>::iterator i;

	if(MutexLock == NULL) return;
	WaitForSingleObject(MutexLock, INFINITE);

	i = FileNameList.find(FileName);
	if(i != FileNameList.end())
	{
		if(DeleteFile(FileName)) FileNameList.erase(i);
	}

	ReleaseMutex(MutexLock);
}


//+-----------------------------------------------------------------------------
//| Clears all filters
//+-----------------------------------------------------------------------------
VOID TEMPORARY_FILE::DestroyAllTemporaryFiles()
{
	std::set<std::string>::iterator i;

	if(MutexLock == NULL) return;
	WaitForSingleObject(MutexLock, INFINITE);

	i = FileNameList.begin();
	while(i != FileNameList.end())
	{
		DeleteFile(*i);
		i++;
	}

	FileNameList.clear();

	ReleaseMutex(MutexLock);
}


//+-----------------------------------------------------------------------------
//| Launches a new thread monitoring the temporary file
//+-----------------------------------------------------------------------------
BOOL TEMPORARY_FILE::LaunchThread(FILE_PROCESS* FileProcess)
{
	HANDLE Handle;
	DWORD ThreadId;
	SECURITY_ATTRIBUTES Attributes;

	ZeroMemory(&Attributes, sizeof(SECURITY_ATTRIBUTES));
	Attributes.nLength = sizeof(SECURITY_ATTRIBUTES);

	Handle = CreateThread(&Attributes, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(DestroyFileWhenNeccessary),
						  FileProcess, 0, &ThreadId);
	if(Handle == NULL)
	{
		Error.SetMessage("Unable to create a new thread!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the temporary directory
//+-----------------------------------------------------------------------------
std::string TEMPORARY_FILE::GetTemporaryDirectory()
{
	if(TemporaryDirectory == "") TemporaryDirectory = Common.GetProgramDirectory() + "\\" + PATH_TEMP;

	return TemporaryDirectory;
}


//+-----------------------------------------------------------------------------
//| Asserts that there is a temporary directory
//+-----------------------------------------------------------------------------
VOID TEMPORARY_FILE::AssertTemporaryDirectory()
{
	std::string TempDirectory;
	SECURITY_ATTRIBUTES Attributes;

	ZeroMemory(&Attributes, sizeof(SECURITY_ATTRIBUTES));
	Attributes.nLength = sizeof(SECURITY_ATTRIBUTES);

	TempDirectory = Common.GetCurrentDirectory();
	Common.SetCurrentDirectory(Common.GetProgramDirectory());
	CreateDirectory(PATH_TEMP.c_str(), &Attributes);
	Common.SetCurrentDirectory(TempDirectory);
}


//+-----------------------------------------------------------------------------
//| Deletes a file
//+-----------------------------------------------------------------------------
BOOL TEMPORARY_FILE::DeleteFile(CONST std::string& FileName)
{
	/*
	HANDLE FileHandle;
	SECURITY_ATTRIBUTES Attributes;

	ZeroMemory(&Attributes, sizeof(SECURITY_ATTRIBUTES));
	Attributes.nLength = sizeof(SECURITY_ATTRIBUTES);

	FileHandle = CreateFile(FileName.c_str(), GENERIC_WRITE, 0, &Attributes,
							OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);

	if(FileHandle != INVALID_HANDLE_VALUE) CloseHandle(FileHandle);
	*/

	return ::DeleteFile(FileName.c_str());
}


//+-----------------------------------------------------------------------------
//| Destroys the file when it's no longer needed
//+-----------------------------------------------------------------------------
DWORD CALLBACK TEMPORARY_FILE::DestroyFileWhenNeccessary(DWORD Parameter)
{
	HANDLE Handle;
	std::string FileName;
	FILE_PROCESS* FileProcessPointer;

	FileProcessPointer = reinterpret_cast<FILE_PROCESS*>(Parameter);
	if(FileProcessPointer == NULL)
	{
		ExitThread(0);
		return 0;
	}

	Handle = FileProcessPointer->Handle;
	FileName = FileProcessPointer->FileName;

	delete FileProcessPointer;

	if(WaitForSingleObject(Handle, INFINITE) == WAIT_FAILED)
	{
		ExitThread(0);
		return 0;
	}

	TemporaryFile.DestroyTemporaryFile(FileName);

	ExitThread(0);
	return 0;
}

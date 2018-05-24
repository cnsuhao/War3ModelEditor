//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MPQ_H
#define MAGOS_MPQ_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Buffer.h"
#include "StormLibStatic.h"


//+-----------------------------------------------------------------------------
//| Mpq class
//+-----------------------------------------------------------------------------
class MPQ
{
	public:
		CONSTRUCTOR MPQ();
		DESTRUCTOR ~MPQ();

		BOOL Open(CONST std::string& FileName);
		VOID Close();

		INT GetNrOfFiles();
		VOID GetFileList(std::list<std::string>& FileList);

		BOOL FileExists(CONST std::string& FileName);
		INT GetFileSize(CONST std::string& FileName);
		BOOL LoadFile(CONST std::string& FileName, BUFFER& Buffer);
		BOOL ExportFile(CONST std::string& FileName, CONST std::string& RealFileName);

	protected:
		HANDLE MpqHandle;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MPQ Mpq;
extern MPQ MpqWar3;
extern MPQ MpqWar3x;
extern MPQ MpqWar3xLocal;
extern MPQ MpqWar3Patch;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_REGISTER_H
#define MAGOS_REGISTER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Buffer.h"


//+-----------------------------------------------------------------------------
//| Register class
//+-----------------------------------------------------------------------------
class REGISTER
{
	public:
		CONSTRUCTOR REGISTER();
		DESTRUCTOR ~REGISTER();

		VOID FlushKey(HKEY Key);
		VOID CloseKey(HKEY Key);

		BOOL CreateKey(HKEY& Key, HKEY Parent, CONST std::string& Name);
		BOOL OpenKey(HKEY& Key, HKEY Parent, CONST std::string& Name);

		BOOL SetDwordValue(HKEY Key, CONST std::string& Name, DWORD Dword);
		BOOL SetBinaryValue(HKEY Key, CONST std::string& Name, BUFFER& Buffer);
		BOOL SetStringValue(HKEY Key, CONST std::string& Name, CONST std::string& String);

		BOOL GetDwordValue(HKEY Key, CONST std::string& Name, DWORD& Dword);
		BOOL GetBinaryValue(HKEY Key, CONST std::string& Name, BUFFER& Buffer);
		BOOL GetStringValue(HKEY Key, CONST std::string& Name, std::string& String);

		BOOL RegisterExtention(CONST std::string& Extention, CONST std::string& Description, CONST std::string& Icon);
		BOOL RegisterOpenWith(CONST std::string& Description);

		BOOL FindWarcraftDirectory();
		std::string GetWarcraftDirectory() CONST;

	protected:
		std::string WarcraftDirectory;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern REGISTER Register;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

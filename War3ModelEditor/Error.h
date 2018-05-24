//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_ERROR_H
#define MAGOS_ERROR_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Constants.h"


//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST std::string DEFAULT_ERROR_MESSAGE = "An unknown error has occured!";
CONST std::string ERROR_MESSAGE_TITLE   = "Error";


//+-----------------------------------------------------------------------------
//| Error class
//+-----------------------------------------------------------------------------
class ERROR_HANDLER
{
	public:
		CONSTRUCTOR ERROR_HANDLER();
		DESTRUCTOR ~ERROR_HANDLER();

		VOID ClearMessage();
		VOID SetMessage(CONST std::string& NewMessage);

		std::string GetMessage() CONST;

		VOID DisplayMessage(HWND Window = NULL) CONST;
		VOID DisplayMessageIfSet(HWND Window = NULL) CONST;

	protected:
		BOOL MessageSet;
		std::string Message;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern ERROR_HANDLER Error;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

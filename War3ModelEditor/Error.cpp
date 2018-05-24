//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Error.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
ERROR_HANDLER Error;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
ERROR_HANDLER::ERROR_HANDLER()
{
	ClearMessage();
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
ERROR_HANDLER::~ERROR_HANDLER()
{
	ClearMessage();
}


//+-----------------------------------------------------------------------------
//| Clears the message
//+-----------------------------------------------------------------------------
VOID ERROR_HANDLER::ClearMessage()
{
	MessageSet = FALSE;
	Message = "";
}


//+-----------------------------------------------------------------------------
//| Sets a new message
//+-----------------------------------------------------------------------------
VOID ERROR_HANDLER::SetMessage(CONST std::string& NewMessage)
{
	MessageSet = TRUE;
	Message = NewMessage;
}


//+-----------------------------------------------------------------------------
//| Returns the message
//+-----------------------------------------------------------------------------
std::string ERROR_HANDLER::GetMessage() CONST
{
	return Message;
}


//+-----------------------------------------------------------------------------
//| Displays the message (always)
//+-----------------------------------------------------------------------------
VOID ERROR_HANDLER::DisplayMessage(HWND Window) CONST
{
	MessageBoxEx(Window, (MessageSet ? Message : DEFAULT_ERROR_MESSAGE).c_str(), ERROR_MESSAGE_TITLE.c_str(), MB_ICONERROR, CURRENT_LANGUAGE);
}


//+-----------------------------------------------------------------------------
//| Displays the message (if one has been set)
//+-----------------------------------------------------------------------------
VOID ERROR_HANDLER::DisplayMessageIfSet(HWND Window) CONST
{
	if(MessageSet) DisplayMessage(Window);
}

//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_FILE_DIALOG_H
#define MAGOS_WINDOW_FILE_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| File dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_FILE_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_FILE_DIALOG();
		DESTRUCTOR ~WINDOW_FILE_DIALOG();

		BOOL OpenFile(HWND Window, std::string& Path);
		BOOL SaveFile(HWND Window, std::string& Path);

		VOID SetTitle(CONST std::string& NewTitle);
		VOID SetFilter(CONST CHAR* NewFilter = "All files (*.*)\0*.*\0\0");
		VOID SetDefaultExtention(CONST std::string& NewExtention = "");

	protected:
		OPENFILENAME OpenFileName;

		std::string Title;
		std::string Filter;
		std::string Extention;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

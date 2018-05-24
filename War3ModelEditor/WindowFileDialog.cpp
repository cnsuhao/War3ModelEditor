//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowFileDialog.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_FILE_DIALOG::WINDOW_FILE_DIALOG()
{
	OpenFileName.lStructSize = sizeof(OPENFILENAME);
	OpenFileName.hwndOwner = NULL;
	OpenFileName.hInstance = NULL;
	OpenFileName.lpstrFilter = NULL;
	OpenFileName.lpstrCustomFilter = NULL;
	OpenFileName.nMaxCustFilter = 0;
	OpenFileName.nFilterIndex = 0;
	OpenFileName.lpstrFile = NULL;
	OpenFileName.nMaxFile = 0;
	OpenFileName.lpstrFileTitle = NULL;
	OpenFileName.nMaxFileTitle = 0;
	OpenFileName.lpstrInitialDir = NULL;
	OpenFileName.lpstrTitle = NULL;
	OpenFileName.Flags = 0;
	OpenFileName.nFileOffset = 0;
	OpenFileName.nFileExtension = 0;
	OpenFileName.lpstrDefExt = NULL;
	OpenFileName.lCustData = 0;
	OpenFileName.lpfnHook = NULL;
	OpenFileName.lpTemplateName = NULL;

	Title = "Open File";
	Extention = "";

	SetFilter();
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_FILE_DIALOG::~WINDOW_FILE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Brings up an Open File dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_FILE_DIALOG::OpenFile(HWND Window, std::string& Path)
{
	BOOL ReturnValue = FALSE;
	CHAR Buffer[BUFFER_SIZE];

	//strcpy(Buffer, Path.c_str());
	strcpy_s(Buffer, sizeof(Buffer), Path.c_str());

	OpenFileName.hwndOwner = Window;
	OpenFileName.nMaxFile = BUFFER_SIZE - 1;
	OpenFileName.lpstrFile = Buffer;
	OpenFileName.lpstrInitialDir = Buffer;
	OpenFileName.lpstrFilter = (Filter != "") ? Filter.c_str() : NULL;
	OpenFileName.lpstrDefExt = (Extention != "") ? Extention.c_str() : NULL;
	OpenFileName.lpstrTitle = (Title != "") ? Title.c_str() : NULL;
	OpenFileName.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	ReturnValue = GetOpenFileName(&OpenFileName);
	Path = Buffer;

	return ReturnValue;
}


//+-----------------------------------------------------------------------------
//| Brings up a Save File dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_FILE_DIALOG::SaveFile(HWND Window, std::string& Path)
{
	BOOL ReturnValue = FALSE;
	CHAR Buffer[BUFFER_SIZE];

//	strcpy(Buffer, Path.c_str());
	strcpy_s(Buffer, sizeof(Buffer), Path.c_str());
	OpenFileName.hwndOwner = Window;
	OpenFileName.nMaxFile = BUFFER_SIZE - 1;
	OpenFileName.lpstrFile = Buffer;
	OpenFileName.lpstrInitialDir = Buffer;
	OpenFileName.lpstrFilter = (Filter != "") ? Filter.c_str() : NULL;
	OpenFileName.lpstrDefExt = (Extention != "") ? Extention.c_str() : NULL;
	OpenFileName.lpstrTitle = (Title != "") ? Title.c_str() : NULL;
	OpenFileName.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

	ReturnValue = GetSaveFileName(&OpenFileName);
	Path = Buffer;

	return ReturnValue;
}


//+-----------------------------------------------------------------------------
//| Changes the title of the dialogs
//+-----------------------------------------------------------------------------
VOID WINDOW_FILE_DIALOG::SetTitle(CONST std::string& NewTitle)
{
	Title = NewTitle;
}


//+-----------------------------------------------------------------------------
//| Changes the filter used
//+-----------------------------------------------------------------------------
VOID WINDOW_FILE_DIALOG::SetFilter(CONST CHAR* NewFilter)
{
	INT i;
	INT Size = 0;
	CHAR LastChar = 0;
	CHAR CurrentChar = 0;

	if(NewFilter == NULL)
	{
		Filter = "";
		return;
	}

	if(NewFilter[0] == '\0')
	{
		Filter = "";
		return;
	}

	while((NewFilter[Size + 1] != '\0') || (NewFilter[Size + 1] != NewFilter[Size]))
	{
		Size++;
	}

	Size += 2;
	Filter.resize(Size);

	for(i = 0; i < Size; i++)
	{
		Filter[i] = NewFilter[i];
	}
}



//+-----------------------------------------------------------------------------
//| Changes the default extention
//+-----------------------------------------------------------------------------
VOID WINDOW_FILE_DIALOG::SetDefaultExtention(CONST std::string& NewExtention)
{
	Extention = NewExtention;
}

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowDialog.h"


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
std::set<HWND> WINDOW_DIALOG::OpenDialogSet;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_DIALOG::WINDOW_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_DIALOG::~WINDOW_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Closes all open dialogs
//+-----------------------------------------------------------------------------
VOID WINDOW_DIALOG::CloseAllOpenDialogs()
{
	std::set<HWND>::iterator i;

	i = OpenDialogSet.begin();
	while(i != OpenDialogSet.end())
	{
		EndDialog((*i), 0);
		i++;
	}

	OpenDialogSet.clear();
}


//+-----------------------------------------------------------------------------
//| Centers a window on the screen
//+-----------------------------------------------------------------------------
VOID WINDOW_DIALOG::CenterWindow(HWND Window)
{
	INT X;
	INT Y;
	RECT TempRect;

	GetWindowRect(Window, &TempRect);

	X = (GetSystemMetrics(SM_CXSCREEN) - (TempRect.right - TempRect.left)) / 2;
	if(X < 0) X = 0;

	Y = (GetSystemMetrics(SM_CYSCREEN) - (TempRect.bottom - TempRect.top)) / 2;
	if(Y < 0) Y = 0;

	SetWindowPos(Window, NULL, X, Y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
}


//+-----------------------------------------------------------------------------
//| Sets a new int for a control
//+-----------------------------------------------------------------------------
VOID WINDOW_DIALOG::SetInt(HWND Control, INT NewInt)
{
	std::stringstream Stream;

	Stream << NewInt;
	SetString(Control, Stream.str());
}


//+-----------------------------------------------------------------------------
//| Sets a new float for a control
//+-----------------------------------------------------------------------------
VOID WINDOW_DIALOG::SetFloat(HWND Control, FLOAT NewFloat)
{
	std::stringstream Stream;

	Stream << NewFloat;
	SetString(Control, Stream.str());
}


//+-----------------------------------------------------------------------------
//| Sets a new string for a control
//+-----------------------------------------------------------------------------
VOID WINDOW_DIALOG::SetString(HWND Control, CONST std::string& NewString)
{
	::SetWindowText(Control, NewString.c_str());
}


//+-----------------------------------------------------------------------------
//| Retrieves an int form a control
//+-----------------------------------------------------------------------------
INT WINDOW_DIALOG::GetInt(HWND Control, INT DefaultInt)
{
	std::stringstream Stream;

	Stream.str(GetString(Control));
	Stream >> DefaultInt;

	return DefaultInt;
}


//+-----------------------------------------------------------------------------
//| Retrieves a float form a control
//+-----------------------------------------------------------------------------
FLOAT WINDOW_DIALOG::GetFloat(HWND Control, FLOAT DefaultFloat)
{
	std::stringstream Stream;

	Stream.str(GetString(Control));
	Stream >> DefaultFloat;

	return DefaultFloat;
}


//+-----------------------------------------------------------------------------
//| Retrieves a string form a control
//+-----------------------------------------------------------------------------
std::string WINDOW_DIALOG::GetString(HWND Control)
{
	INT Size;
	std::vector<CHAR> Buffer;

	Size = ::GetWindowTextLength(Control) + 1;
	Buffer.resize(Size + 1);

	::GetWindowText(Control, &Buffer[0], Size);
	Buffer[Size] = '\0';

	return &Buffer[0];
}


//+-----------------------------------------------------------------------------
//| Converts a boolean to a check state
//+-----------------------------------------------------------------------------
UINT WINDOW_DIALOG::BoolToCheckState(BOOL Bool)
{
	return Bool ? BST_CHECKED : BST_UNCHECKED;
}


//+-----------------------------------------------------------------------------
//| Converts a check state to a boolean
//+-----------------------------------------------------------------------------
BOOL WINDOW_DIALOG::CheckStateToBool(UINT CheckState)
{
	return (CheckState == BST_CHECKED);
}


//+-----------------------------------------------------------------------------
//| Selects a color from a color dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_DIALOG::SelectColor(HWND Window, D3DCOLOR& Color)
{
	BOOL Status;
	CHAR* Rgb;
	DWORD TempColor;
	CHOOSECOLOR ColorStruct;
	CONST INT NR_OF_COLORS = 16;

	static DWORD ColorArray[NR_OF_COLORS];
	static BOOL Initialized = FALSE;

	if(!Initialized)
	{
		Initialized = TRUE;

		ColorArray[0] = 0x000303FF;
		ColorArray[1] = 0x00FF4200;
		ColorArray[2] = 0x00B9E61C;
		ColorArray[3] = 0x00810054;
		ColorArray[4] = 0x0001FCFF;
		ColorArray[5] = 0x000EBAFE;
		ColorArray[6] = 0x0000C020;
		ColorArray[7] = 0x00B05BE5;
		ColorArray[8] = 0x00979695;
		ColorArray[9] = 0x00F1BF7E;
		ColorArray[10] = 0x00466210;
		ColorArray[11] = 0x00042A4E;
		ColorArray[12] = 0x00000000;
		ColorArray[13] = 0x00000000;
		ColorArray[14] = 0x00000000;
		ColorArray[15] = 0x00000000;

		Initialized = TRUE;
	}

	TempColor = Color;
	TempColor &= 0x00FFFFFF;

	Rgb = reinterpret_cast<CHAR*>(&TempColor);
	std::swap(Rgb[0], Rgb[2]);

	ZeroMemory(&ColorStruct, sizeof(CHOOSECOLOR));
	ColorStruct.lStructSize = sizeof(CHOOSECOLOR);
	ColorStruct.hwndOwner = Window;
	ColorStruct.hInstance = NULL;
	ColorStruct.rgbResult = TempColor;
	ColorStruct.lpCustColors = ColorArray;
	ColorStruct.Flags = CC_RGBINIT | CC_FULLOPEN;
	ColorStruct.lCustData = NULL;
	ColorStruct.lpfnHook = NULL;
	ColorStruct.lpTemplateName = NULL;

	Status = ChooseColor(&ColorStruct);
	if(!Status) return FALSE;

	TempColor = ColorStruct.rgbResult;

	Rgb = reinterpret_cast<CHAR*>(&TempColor);
	std::swap(Rgb[0], Rgb[2]);

	TempColor |= 0xFF000000;
	Color = TempColor;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Renders a colored box
//+-----------------------------------------------------------------------------
VOID WINDOW_DIALOG::RenderColorBox(HWND Window, HWND Control, D3DCOLOR Color)
{
	PAINTSTRUCT PaintStruct;
	HDC HandleDC;
	DWORD TempColor;
	RECT TempRect;
	BYTE* Rgb;

	TempColor = Color;
	TempColor &= 0x00FFFFFF;

	GetWindowRect(Control, &TempRect);
	GetClientRect(Control, &TempRect);

	ValidateRect(Control, &TempRect);

	TempRect.left += 2;
	TempRect.right += 2;
	TempRect.top += 2;
	TempRect.bottom += 2;

	HandleDC = BeginPaint(Window, &PaintStruct);
	if(HandleDC == NULL) return;

	Rgb = reinterpret_cast<BYTE*>(&TempColor);
	std::swap(Rgb[0], Rgb[2]);

	FillRect(GetWindowDC(Control), &TempRect, CreateSolidBrush(TempColor));

	EndPaint(Window, &PaintStruct);
}


//+-----------------------------------------------------------------------------
//| Adds a window to the set of opened dialogs
//+-----------------------------------------------------------------------------
VOID WINDOW_DIALOG::AddWindow(HWND Window)
{
	OpenDialogSet.insert(Window);
}


//+-----------------------------------------------------------------------------
//| Removes a window from the set of opened dialogs
//+-----------------------------------------------------------------------------
VOID WINDOW_DIALOG::RemoveWindow(HWND Window)
{
	OpenDialogSet.erase(Window);
}

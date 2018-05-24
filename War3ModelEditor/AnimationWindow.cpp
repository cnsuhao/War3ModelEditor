//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "AnimationWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
ANIMATION_WINDOW AnimationWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
ANIMATION_WINDOW::ANIMATION_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
ANIMATION_WINDOW::~ANIMATION_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL ANIMATION_WINDOW::Create()
{
	CHAR* Rgb;
	INT Index;
	DWORD BackgroundColor;
	LOGBRUSH BackgroundBrush;
	WINDOW_COMBOBOX_INFO ComboboxInfo;
	WINDOW_BUTTON_INFO ButtonInfo;

	BackgroundColor = PROPERTIES_NO_BACKGROUND;
	BackgroundColor &= 0x00FFFFFF;

	Rgb = reinterpret_cast<CHAR*>(&BackgroundColor);
	std::swap(Rgb[0], Rgb[2]);

	BackgroundBrush.lbStyle = BS_SOLID;
	BackgroundBrush.lbColor = DIB_RGB_COLORS | BackgroundColor;
	BackgroundBrush.lbHatch = 0;

	FrameInfo.Title = "Animation Controller";
	FrameInfo.Background = CreateBrushIndirect(&BackgroundBrush);
	FrameInfo.Width = DEFAULT_ANIMATION_WINDOW_WIDTH;
	FrameInfo.Height = DEFAULT_ANIMATION_WINDOW_HEIGHT;
	FrameInfo.Style &= (~WS_VISIBLE);

	if(!WINDOW_FRAME::Create()) return FALSE;

	ComboboxInfo.Parent = Window;
	ComboboxInfo.Width = DEFAULT_ANIMATION_WINDOW_WIDTH;
	ComboboxInfo.Height = DEFAULT_ANIMATION_WINDOW_HEIGHT;
	ComboboxInfo.AutoSort = TRUE;

	if(!ComboBox.Create(ComboboxInfo)) return FALSE;

	Index = ComboBox.AddString(NAME_NO_ANIMATION);
	ComboBox.SetData(Index, NULL);
	ComboBox.SetCurrentSelection(0);

	ButtonInfo.Parent = Window;
	ButtonInfo.Text = "Play Animation";
	ButtonInfo.X = 10;
	ButtonInfo.Y = 50;
	ButtonInfo.Width = 150;
	ButtonInfo.Height = 26;
	ButtonInfo.ButtonType = BUTTON_TYPE_PUSHBUTTON;

	if(!PlayButton.Create(ButtonInfo)) return FALSE;

	ButtonInfo.Text = "Default Loop";
	ButtonInfo.X = 10;
	ButtonInfo.Y = 90;
	ButtonInfo.Width = DEFAULT_ANIMATION_WINDOW_WIDTH - 10;
	ButtonInfo.Height = 22;
	ButtonInfo.ButtonType = BUTTON_TYPE_RADIOBUTTON;

	if(!DefaultLoopButton.Create(ButtonInfo)) return FALSE;

	ButtonInfo.Text = "Always Loop";
	ButtonInfo.X = 10;
	ButtonInfo.Y = 112;

	if(!AlwaysLoopButton.Create(ButtonInfo)) return FALSE;

	ButtonInfo.Text = "Never Loop";
	ButtonInfo.X = 10;
	ButtonInfo.Y = 134;

	if(!NeverLoopButton.Create(ButtonInfo)) return FALSE;

	DefaultLoopButton.SendMessage(BM_SETCHECK, BST_CHECKED, 0);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID ANIMATION_WINDOW::Destroy()
{
	NeverLoopButton.Destroy();
	AlwaysLoopButton.Destroy();
	DefaultLoopButton.Destroy();
	PlayButton.Destroy();
	ComboBox.Destroy();

	WINDOW_FRAME::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT ANIMATION_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_CLOSE:
		{
			Hide();
			return 0;
		}

		case WM_DESTROY:
		{
			return 0;
		}
	}

	return DefWindowProc(Window, Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT ANIMATION_WINDOW::MenuHandler(WORD MenuItem)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window control messages
//+-----------------------------------------------------------------------------
LRESULT ANIMATION_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	PROPERTIES_INFO PropertiesInfo;

	if(Control == ComboBox.GetWindow())
	{
		if(Code == CBN_SELCHANGE)
		{
			Model.PlayAnimation(reinterpret_cast<MODEL_SEQUENCE*>(ComboBox.GetData(ComboBox.GetCurrentSelection())));
		}
	}
	else if(Control == PlayButton.GetWindow())
	{
		Model.PlayAnimation(reinterpret_cast<MODEL_SEQUENCE*>(ComboBox.GetData(ComboBox.GetCurrentSelection())));
	}
	else if(Control == DefaultLoopButton.GetWindow())
	{
		PropertiesInfo = Properties.GetPropertiesInfo();
		PropertiesInfo.Minor.AlwaysLoop = FALSE;
		PropertiesInfo.Minor.NeverLoop = FALSE;
		Properties.SetPropertiesInfo(PropertiesInfo);

		DefaultLoopButton.SendMessage(BM_SETCHECK, BST_CHECKED, 0);
	}
	else if(Control == AlwaysLoopButton.GetWindow())
	{
		PropertiesInfo = Properties.GetPropertiesInfo();
		PropertiesInfo.Minor.AlwaysLoop = TRUE;
		PropertiesInfo.Minor.NeverLoop = FALSE;
		Properties.SetPropertiesInfo(PropertiesInfo);

		AlwaysLoopButton.SendMessage(BM_SETCHECK, BST_CHECKED, 0);
	}
	else if(Control == NeverLoopButton.GetWindow())
	{
		PropertiesInfo = Properties.GetPropertiesInfo();
		PropertiesInfo.Minor.AlwaysLoop = FALSE;
		PropertiesInfo.Minor.NeverLoop = TRUE;
		Properties.SetPropertiesInfo(PropertiesInfo);

		NeverLoopButton.SendMessage(BM_SETCHECK, BST_CHECKED, 0);
	}

	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT ANIMATION_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Adds an animation
//+-----------------------------------------------------------------------------
VOID ANIMATION_WINDOW::AddAnimation(INT Id, CONST std::string& Name)
{
	INT Index;

	Index = ComboBox.AddString(Name);
	ComboBox.SetData(Index, Id);
}


//+-----------------------------------------------------------------------------
//| Removes an animation
//+-----------------------------------------------------------------------------
VOID ANIMATION_WINDOW::RemoveAnimation(INT Id)
{
	INT i;
	INT Size;

	Size = ComboBox.GetNrOfItems();

	for(i = 0; i < Size; i++)
	{
		if(ComboBox.GetData(i) == Id)
		{
			ComboBox.RemoveString(i);
			break;
		}
	}
}


//+-----------------------------------------------------------------------------
//| Clears all animations
//+-----------------------------------------------------------------------------
VOID ANIMATION_WINDOW::ClearAllAnimations()
{
	INT Index;

	ComboBox.Clear();
	Index = ComboBox.AddString(NAME_NO_ANIMATION);
	ComboBox.SetData(Index, NULL);
	ComboBox.SetCurrentSelection(0);
}


//+-----------------------------------------------------------------------------
//| Changes the name of a sequence
//+-----------------------------------------------------------------------------
VOID ANIMATION_WINDOW::ChangeName(MODEL_SEQUENCE* Sequence, CONST std::string& NewName)
{
	INT i;
	INT Size;
	INT Index;

	Size = ComboBox.GetNrOfItems();
	for(i = 0; i < Size; i++)
	{
		if(reinterpret_cast<MODEL_SEQUENCE*>(ComboBox.GetData(i)) == Sequence)
		{
			ComboBox.RemoveString(i);
			Index = ComboBox.AddString(NewName);
			ComboBox.SetData(Index, reinterpret_cast<INT>(Sequence));
			break;
		}
	}
}

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowInterpolatorDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_INTERPOLATOR_DIALOG InterpolatorDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
INTERPOLATOR WINDOW_INTERPOLATOR_DIALOG::StaticData;
MODEL_GLOBAL_SEQUENCE* WINDOW_INTERPOLATOR_DIALOG::StaticGlobalSequence;
std::string WINDOW_INTERPOLATOR_DIALOG::StaticTitle;
HFONT WINDOW_INTERPOLATOR_DIALOG::StaticFont;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_INTERPOLATOR_DIALOG::WINDOW_INTERPOLATOR_DIALOG()
{
	StaticFont = CreateFont(0, 8, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, "Fixedsys");
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_INTERPOLATOR_DIALOG::~WINDOW_INTERPOLATOR_DIALOG()
{
	DeleteObject(StaticFont);
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_INTERPOLATOR_DIALOG::Display(HWND ParentWindow, INTERPOLATOR& Data, CONST std::string& Title) CONST
{
	StaticData = Data;
	StaticTitle = Title;

	StaticGlobalSequence = Data.GlobalSequenceNode.IsAttached() ? Data.GlobalSequenceNode.GetObjectData() : NULL;

	if(!DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogInterpolator), ParentWindow, DialogMessageHandler, 0))
	{
		StaticData = INTERPOLATOR();
		return FALSE;
	}

	Data = StaticData;
	StaticData = INTERPOLATOR();

	if(StaticGlobalSequence != NULL)
	{
		Data.GlobalSequenceNode.Attach(StaticGlobalSequence->InterpolatorNodes);
	}
	else
	{
		Data.GlobalSequenceNode.Detach();
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_INTERPOLATOR_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT Index;
			HWND ComboBox;

			AddWindow(Window);
			CenterWindow(Window);

			ComboBox = GetDlgItem(Window, DialogInterpolatorComboInterpolationType);

			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("None")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, INTERPOLATION_TYPE_NONE);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Linear")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, INTERPOLATION_TYPE_LINEAR);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Hermite")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, INTERPOLATION_TYPE_HERMITE);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("Bezier")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, INTERPOLATION_TYPE_BEZIER);

			::SendMessage(ComboBox, CB_SETCURSEL, StaticData.InterpolationType, 0);

			if(StaticFont != NULL) ::SendMessage(GetDlgItem(Window, DialogInterpolatorEdit), WM_SETFONT, reinterpret_cast<WPARAM>(StaticFont), 0);

			BuildList(GetDlgItem(Window, DialogInterpolatorEdit));
			BuildGlobalSequenceIdList(Window);

			::SetWindowText(Window, StaticTitle.c_str());

			return TRUE;
		}

		case WM_COMMAND:
		{
			HWND Control;

			Control = reinterpret_cast<HWND>(L);
			if(Control == GetDlgItem(Window, DialogInterpolatorComboInterpolationType))
			{
				switch(HIWORD(W))
				{
					case CBN_SELCHANGE:
					{
						INT Index;
						BOOL TypeUsesTangents;
						BOOL LastTypeUsesTangents;
						INTERPOLATION_TYPE Type;

						Index = static_cast<INT>(::SendMessage(Control, CB_GETCURSEL, 0, 0));
						Type = static_cast<INTERPOLATION_TYPE>(::SendMessage(Control, CB_GETITEMDATA, Index, 0));

						TypeUsesTangents = (Type == INTERPOLATION_TYPE_HERMITE) || (Type == INTERPOLATION_TYPE_BEZIER);
						LastTypeUsesTangents = (StaticData.InterpolationType == INTERPOLATION_TYPE_HERMITE) || (StaticData.InterpolationType == INTERPOLATION_TYPE_BEZIER);

						if(TypeUsesTangents != LastTypeUsesTangents)
						{
							if(!RetrieveList(GetDlgItem(Window, DialogInterpolatorEdit)))
							{
								Error.DisplayMessage(Window);
								Error.ClearMessage();
								::SendMessage(GetDlgItem(Window, DialogInterpolatorComboInterpolationType), CB_SETCURSEL, StaticData.InterpolationType, 0);
								return TRUE;
							}

							StaticData.InterpolationType = Type;
							BuildList(GetDlgItem(Window, DialogInterpolatorEdit));
						}
						else
						{
							StaticData.InterpolationType = Type;
						}

						break;
					}
				}

				return TRUE;
			}

			switch(LOWORD(W))
			{
				case DialogInterpolatorButtonOk:
				{
					if(!RetrieveList(GetDlgItem(Window, DialogInterpolatorEdit)))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					StaticGlobalSequence = reinterpret_cast<MODEL_GLOBAL_SEQUENCE*>(::SendMessage(GetDlgItem(Window, DialogInterpolatorComboGlobalSequenceId), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogInterpolatorComboGlobalSequenceId), CB_GETCURSEL, 0, 0), 0));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogInterpolatorButtonCancel:
				{
					RemoveWindow(Window);
					EndDialog(Window, 0);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogInterpolatorButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Builds the global sequence ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_INTERPOLATOR_DIALOG::BuildGlobalSequenceIdList(HWND Window)
{
	INT i;
	INT Index;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogInterpolatorComboGlobalSequenceId);
	if(ComboBox == NULL) return;

	Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(NAME_NO_ID.c_str())));
	::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
	::SendMessage(ComboBox, CB_SETCURSEL, 0, 0);

	for(i = 0; i < Model.Data().GlobalSequenceContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GlobalSequenceContainer.ValidIndex(i))
		{
			std::stringstream Stream;

			Stream << "GlobalSequence " << Model.Data().GlobalSequenceContainer[i];
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Stream.str().c_str())));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Model.Data().GlobalSequenceContainer[i]));

			if(Model.Data().GlobalSequenceContainer[i] == StaticGlobalSequence)
			{
				::SendMessage(ComboBox, CB_SETCURSEL, Index, 0);
			}
		}
	}
}


//+-----------------------------------------------------------------------------
//| Builds an interpolator list
//+-----------------------------------------------------------------------------
VOID WINDOW_INTERPOLATOR_DIALOG::BuildList(HWND ListBox)
{
	std::stringstream Stream;
	std::list<INTERPOLATOR_NODE>::iterator i;

	if(StaticData.IsStatic())
	{
		Stream << "0: ";
		BuildVector(Stream, StaticData.StaticVector);
		Stream << "\r\n";
		::SetWindowText(ListBox, Stream.str().c_str());
		return;
	}

	i = StaticData.NodeList.begin();
	while(i != StaticData.NodeList.end())
	{
		Stream << i->Time << ": ";
		BuildVector(Stream, i->Vector);
		Stream << "\r\n";

		switch(StaticData.InterpolationType)
		{
			case INTERPOLATION_TYPE_HERMITE:
			case INTERPOLATION_TYPE_BEZIER:
			{
				Stream << "  InTan: ";
				BuildVector(Stream, i->InTan);
				Stream << "\r\n";

				Stream << "  OutTan: ";
				BuildVector(Stream, i->OutTan);
				Stream << "\r\n";
			}
		}

		i++;
	}

	::SetWindowText(ListBox, Stream.str().c_str());
}


//+-----------------------------------------------------------------------------
//| Retrieves the interpolator list
//+-----------------------------------------------------------------------------
BOOL WINDOW_INTERPOLATOR_DIALOG::RetrieveList(HWND ListBox)
{
	INT Size;
	std::string Word;
	std::vector<CHAR> Buffer;
	std::stringstream Stream;
	INTERPOLATOR_NODE Node;

	Size = ::GetWindowTextLength(ListBox) + 1;
	Buffer.resize(Size + 1);

	::GetWindowText(ListBox, &Buffer[0], Size);
	Buffer[Size] = '\0';

	Stream.str(&Buffer[0]);

	StaticData.NodeList.clear();

	while(!Stream.eof())
	{
		Node = INTERPOLATOR_NODE();

		Node.Time = -1;
		Stream >> Node.Time;
		if(Node.Time == -1) break;

		if(!ExpectChar(Stream, ':')) return FALSE;
		if(!RetrieveVector(Stream, Node.Vector)) return FALSE;

		switch(StaticData.InterpolationType)
		{
			case INTERPOLATION_TYPE_HERMITE:
			case INTERPOLATION_TYPE_BEZIER:
			{
				Word = "";
				Stream >> Word;
				if(Word != "InTan:")
				{
					Error.SetMessage("Expected \"InTan:\", got \"" + Word + "\"!");
					return FALSE;
				}

				if(!RetrieveVector(Stream, Node.InTan)) return FALSE;

				Word = "";
				Stream >> Word;
				if(Word != "OutTan:")
				{
					Error.SetMessage("Expected \"OutTan:\", got \"" + Word + "\"!");
					return FALSE;
				}

				if(!RetrieveVector(Stream, Node.OutTan)) return FALSE;
			}
		}

		StaticData.NodeList.push_back(Node);
	}

	StaticData.Static = FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Builds a vector
//+-----------------------------------------------------------------------------
VOID WINDOW_INTERPOLATOR_DIALOG::BuildVector(std::stringstream& Stream, CONST D3DXVECTOR4& Vector)
{
	switch(StaticData.Type)
	{
		case INTERPOLATOR_TYPE_SCALAR:
		case INTERPOLATOR_TYPE_SCALAR_INT:
		{
			Stream << Vector.x;
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR2:
		{
			Stream << "{ " << Vector.x << ", " << Vector.y << " }";
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR3:
		{
			Stream << "{ " << Vector.x << ", " << Vector.y << ", " << Vector.z << " }";
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR4:
		{
			Stream << "{ " << Vector.x << ", " << Vector.y << ", " << Vector.z << ", " << Vector.w << " }";
			break;
		}
	}
}


//+-----------------------------------------------------------------------------
//| Retrieves a vector
//+-----------------------------------------------------------------------------
BOOL WINDOW_INTERPOLATOR_DIALOG::RetrieveVector(std::stringstream& Stream, D3DXVECTOR4& Vector)
{
	Vector = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

	switch(StaticData.Type)
	{
		case INTERPOLATOR_TYPE_SCALAR:
		case INTERPOLATOR_TYPE_SCALAR_INT:
		{
			Stream >> Vector.x;
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR2:
		{
			if(!ExpectChar(Stream, '{')) return FALSE;
			Stream >> Vector.x;
			if(!ExpectChar(Stream, ',')) return FALSE;
			Stream >> Vector.y;
			if(!ExpectChar(Stream, '}')) return FALSE;
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR3:
		{
			if(!ExpectChar(Stream, '{')) return FALSE;
			Stream >> Vector.x;
			if(!ExpectChar(Stream, ',')) return FALSE;
			Stream >> Vector.y;
			if(!ExpectChar(Stream, ',')) return FALSE;
			Stream >> Vector.z;
			if(!ExpectChar(Stream, '}')) return FALSE;
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR4:
		{
			if(!ExpectChar(Stream, '{')) return FALSE;
			Stream >> Vector.x;
			if(!ExpectChar(Stream, ',')) return FALSE;
			Stream >> Vector.y;
			if(!ExpectChar(Stream, ',')) return FALSE;
			Stream >> Vector.z;
			if(!ExpectChar(Stream, ',')) return FALSE;
			Stream >> Vector.z;
			if(!ExpectChar(Stream, '}')) return FALSE;
			break;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Expects a specific character
//+-----------------------------------------------------------------------------
BOOL WINDOW_INTERPOLATOR_DIALOG::ExpectChar(std::stringstream& Stream, CHAR Char)
{
	CHAR TempChar = ' ';

	Stream >> TempChar;
	if(TempChar != Char)
	{
		Error.SetMessage(std::string("Expected \"") + Char + "\", got \"" + TempChar + "\"!");
		return FALSE;
	}

	return TRUE;
}

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowEventObjectDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_EVENT_OBJECT_DIALOG EventObjectDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_EVENT_OBJECT_DATA WINDOW_EVENT_OBJECT_DIALOG::StaticData;
MODEL_GLOBAL_SEQUENCE* WINDOW_EVENT_OBJECT_DIALOG::StaticGlobalSequence;
std::string WINDOW_EVENT_OBJECT_DIALOG::StaticNameType;
std::string WINDOW_EVENT_OBJECT_DIALOG::StaticNameIdentifier;
std::string WINDOW_EVENT_OBJECT_DIALOG::StaticNameData;
std::map<std::string, std::string> WINDOW_EVENT_OBJECT_DIALOG::SplatMap;
std::map<std::string, std::string> WINDOW_EVENT_OBJECT_DIALOG::UberSplatMap;
std::map<std::string, std::string> WINDOW_EVENT_OBJECT_DIALOG::SoundMap;
std::map<std::string, std::string> WINDOW_EVENT_OBJECT_DIALOG::SpawnObjectMap;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_EVENT_OBJECT_DIALOG::WINDOW_EVENT_OBJECT_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_EVENT_OBJECT_DIALOG::~WINDOW_EVENT_OBJECT_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Loads all data
//+-----------------------------------------------------------------------------
BOOL WINDOW_EVENT_OBJECT_DIALOG::LoadData()
{
	BUFFER Buffer;
	TOKEN_IN_STREAM TokenStream;
	std::string FileName;
	std::string ProgramDirectory;
	std::string Name;
	std::string Data;

	ProgramDirectory = Common.GetProgramDirectory() + "\\";

	FileName = ProgramDirectory + PATH_DATA_SPLAT;
	TokenStream.SetFileName(FileName);

	if(!FileLoader.LoadFromFile(FileName, Buffer))
	{
		Error.SetMessage("Unable to open \"" + FileName + "\"!");
		return FALSE;
	}

	if(!TokenStream.Load(Buffer)) return FALSE;

	while(!TokenStream.Eof())
	{
		Data = TokenStream.ReadWord();
		if(Data == "") break;

		Name = TokenStream.ReadLine();
		if(Name == "") break;

		RegisterSplat(Data, Data + " - " + Name);
	}

	FileName = ProgramDirectory + PATH_DATA_UBERSPLAT;
	TokenStream.SetFileName(FileName);

	if(!FileLoader.LoadFromFile(FileName, Buffer))
	{
		Error.SetMessage("Unable to open \"" + FileName + "\"!");
		return FALSE;
	}

	if(!TokenStream.Load(Buffer)) return FALSE;

	while(!TokenStream.Eof())
	{
		Data = TokenStream.ReadWord();
		if(Data == "") break;

		Name = TokenStream.ReadLine();
		if(Name == "") break;

		RegisterUberSplat(Data, Data + " - " + Name);
	}

	FileName = ProgramDirectory + PATH_DATA_SOUND;
	TokenStream.SetFileName(FileName);

	if(!FileLoader.LoadFromFile(FileName, Buffer))
	{
		Error.SetMessage("Unable to open \"" + FileName + "\"!");
		return FALSE;
	}

	if(!TokenStream.Load(Buffer)) return FALSE;

	while(!TokenStream.Eof())
	{
		Data = TokenStream.ReadWord();
		if(Data == "") break;

		Name = TokenStream.ReadLine();
		if(Name == "") break;

		RegisterSound(Data, Data + " - " + Name);
	}

	FileName = ProgramDirectory + PATH_DATA_SPAWNOBJECT;
	TokenStream.SetFileName(FileName);

	if(!FileLoader.LoadFromFile(FileName, Buffer))
	{
		Error.SetMessage("Unable to open \"" + FileName + "\"!");
		return FALSE;
	}

	if(!TokenStream.Load(Buffer)) return FALSE;

	while(!TokenStream.Eof())
	{
		Data = TokenStream.ReadWord();
		if(Data == "") break;

		Name = TokenStream.ReadLine();
		if(Name == "") break;

		RegisterSpawnObject(Data, Data + " - " + Name);
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_EVENT_OBJECT_DIALOG::Display(HWND ParentWindow, MODEL_EVENT_OBJECT_DATA& Data, MODEL_GLOBAL_SEQUENCE*& GlobalSequence) CONST
{
	StaticData = Data;
	StaticGlobalSequence = GlobalSequence;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogEventObject), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		GlobalSequence = StaticGlobalSequence;
		StaticData = MODEL_EVENT_OBJECT_DATA();
		return TRUE;
	}

	StaticData = MODEL_EVENT_OBJECT_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_EVENT_OBJECT_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT Index;
			HWND ListBox;
			HWND ComboBox;
			std::list<INT>::iterator i;

			AddWindow(Window);
			CenterWindow(Window);

			ListBox = GetDlgItem(Window, DialogEventObjectList);
			ComboBox = GetDlgItem(Window, DialogEventObjectCombo1);

			i = StaticData.EventTrack.begin();
			while(i != StaticData.EventTrack.end())
			{
				SortedInsert(ListBox, (*i));
				i++;
			}

			::SendMessage(ListBox, LB_SETCURSEL, 0, 0);

			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("FPT (FootPrint)")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, 0);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("SPL (BloodSplat)")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, 1);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("UBR (UberSplat)")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, 2);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("SND (Sound)")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, 3);
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_INSERTSTRING, -1, reinterpret_cast<LPARAM>("SPN (SpawnObject)")));
			::SendMessage(ComboBox, CB_SETITEMDATA, Index, 4);

			if(StaticData.Name.size() != 8) StaticData.Name = STRING_DEFAULT_EVENT;

			StaticNameType = StaticData.Name.substr(0, 3);
			StaticNameIdentifier = StaticData.Name.substr(3, 1);
			StaticNameData = StaticData.Name.substr(4, 4);

			if(StaticNameType == "SPL")
			{
				BuildSplatList(Window);
				::SendMessage(ComboBox, CB_SETCURSEL, 1, 0);
				::SendMessage(GetDlgItem(Window, DialogEventObjectCombo2), CB_SETCURSEL, GetComboIndex(Window, StaticNameData, SplatMap), 0);
			}
			else if(StaticNameType == "UBR")
			{
				BuildUberSplatList(Window);
				::SendMessage(ComboBox, CB_SETCURSEL, 2, 0);
				::SendMessage(GetDlgItem(Window, DialogEventObjectCombo2), CB_SETCURSEL, GetComboIndex(Window, StaticNameData, UberSplatMap), 0);
			}
			else if(StaticNameType == "SND")
			{
				BuildSoundList(Window);
				::SendMessage(ComboBox, CB_SETCURSEL, 3, 0);
				::SendMessage(GetDlgItem(Window, DialogEventObjectCombo2), CB_SETCURSEL, GetComboIndex(Window, StaticNameData, SoundMap), 0);
			}
			else if(StaticNameType == "SPN")
			{
				BuildSpawnObjectList(Window);
				::SendMessage(ComboBox, CB_SETCURSEL, 4, 0);
				::SendMessage(GetDlgItem(Window, DialogEventObjectCombo2), CB_SETCURSEL, GetComboIndex(Window, StaticNameData, SpawnObjectMap), 0);
			}
			else
			{
				BuildSplatList(Window);
				::SendMessage(ComboBox, CB_SETCURSEL, 0, 0);
				::SendMessage(GetDlgItem(Window, DialogEventObjectCombo2), CB_SETCURSEL, GetComboIndex(Window, StaticNameData, SplatMap), 0);
			}

			SetString(GetDlgItem(Window, DialogEventObjectEditIdentifier), StaticNameIdentifier);
			SetString(GetDlgItem(Window, DialogEventObjectEditData), StaticNameData);

			BuildGlobalSequenceIdList(Window);

			return TRUE;
		}

		case WM_CONTEXTMENU:
		{
			INT Index;
			INT MenuId;
			INT NrOfItems;
			RECT TempRect;
			RECT ListRect;
			POINT MousePosition;
			POINT LocalMousePosition;
			HWND ListBox;

			ListBox = GetDlgItem(Window, DialogEventObjectList);

			GetWindowRect(ListBox, &ListRect);
			GetCursorPos(&MousePosition);
			LocalMousePosition = MousePosition;
			ScreenToClient(ListBox, &LocalMousePosition);

			if(!Common.PointInsideRect(MousePosition, ListRect)) return TRUE;

			MenuId = TrackMenu2;
			NrOfItems = static_cast<INT>(::SendMessage(ListBox, LB_GETCOUNT, 0, 0));

			Index = 0;
			while(Index < NrOfItems)
			{
				::SendMessage(ListBox, LB_GETITEMRECT, Index, reinterpret_cast<LPARAM>(&TempRect));
				if(Common.PointInsideRect(LocalMousePosition, TempRect))
				{
					::SendMessage(ListBox, LB_SETCURSEL, Index, 0);
					MenuId = TrackMenu;
					break;
				}

				Index++;
			}

			TrackPopupMenuEx(GetSubMenu(LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(MenuId)), 0), TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePosition.x, MousePosition.y, Window, NULL);

			return TRUE;
		}

		case WM_COMMAND:
		{
			HWND Control;

			Control = reinterpret_cast<HWND>(L);
			if(Control == GetDlgItem(Window, DialogEventObjectCombo1))
			{
				switch(HIWORD(W))
				{
					case CBN_SELCHANGE:
					{
						INT Index;
						INT Type;

						Index = static_cast<INT>(::SendMessage(Control, CB_GETCURSEL, 0, 0));
						Type = static_cast<INT>(::SendMessage(Control, CB_GETITEMDATA, Index, 0));

						switch(Type)
						{
							case 0:
							{
								BuildSplatList(Window);
								break;
							}

							case 1:
							{
								BuildSplatList(Window);
								break;
							}

							case 2:
							{
								BuildUberSplatList(Window);
								break;
							}

							case 3:
							{
								BuildSoundList(Window);
								break;
							}

							case 4:
							{
								BuildSpawnObjectList(Window);
								break;
							}
						}

						::SendMessage(GetDlgItem(Window, DialogEventObjectCombo2), CB_SETCURSEL, 0, 0);

						break;
					}
				}

				return TRUE;
			}
			else if(Control == GetDlgItem(Window, DialogEventObjectCombo2))
			{
				switch(HIWORD(W))
				{
					case CBN_SELCHANGE:
					{
						INT Index;
						INT Type;
						INT Size;
						std::vector<CHAR> Buffer;
						std::string String;
						std::map<std::string, std::string>::iterator i;

						Index = static_cast<INT>(::SendMessage(GetDlgItem(Window, DialogEventObjectCombo1), CB_GETCURSEL, 0, 0));
						Type = static_cast<INT>(::SendMessage(GetDlgItem(Window, DialogEventObjectCombo1), CB_GETITEMDATA, Index, 0));

						Index = static_cast<INT>(::SendMessage(Control, CB_GETCURSEL, 0, 0));
						Size = static_cast<INT>(::SendMessage(Control, CB_GETLBTEXTLEN, Index, 0));
						if(Size ==  LB_ERR) return TRUE;

						Buffer.resize(Size + 1);
						::SendMessage(Control, CB_GETLBTEXT, Index, reinterpret_cast<LPARAM>(&Buffer[0]));
						Buffer[Size] = '\0';
						String = &Buffer[0];

						switch(Type)
						{
							case 0:
							{
								i = SplatMap.find(String);
								if(i == SplatMap.end()) break;

								SetString(GetDlgItem(Window, DialogEventObjectEditData), i->second);

								break;
							}

							case 1:
							{
								i = SplatMap.find(String);
								if(i == SplatMap.end()) break;

								SetString(GetDlgItem(Window, DialogEventObjectEditData), i->second);

								break;
							}

							case 2:
							{
								i = UberSplatMap.find(String);
								if(i == UberSplatMap.end()) break;

								SetString(GetDlgItem(Window, DialogEventObjectEditData), i->second);

								break;
							}

							case 3:
							{
								i = SoundMap.find(String);
								if(i == SoundMap.end()) break;

								SetString(GetDlgItem(Window, DialogEventObjectEditData), i->second);

								break;
							}

							case 4:
							{
								i = SpawnObjectMap.find(String);
								if(i == SpawnObjectMap.end()) break;

								SetString(GetDlgItem(Window, DialogEventObjectEditData), i->second);

								break;
							}

							default:
							{
								SetString(GetDlgItem(Window, DialogEventObjectEditData), "Data not found!");
								break;
							}
						}

						break;
					}
				}

				return TRUE;
			}

			switch(LOWORD(W))
			{
				case TrackFileCreateNew:
				case TrackFileCreateNew2:
				{
					INT Time;

					Time = static_cast<INT>(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogTrack), Window, TrackDialogMessageHandler, 0));
					if(Time < 0) return TRUE;

					SortedInsert(GetDlgItem(Window, DialogEventObjectList), Time);

					return TRUE;
				}

				case TrackFileRemove:
				case TrackFileRemove2:
				{
					INT Index;
					HWND ListBox;

					ListBox = GetDlgItem(Window, DialogEventObjectList);

					Index = static_cast<INT>(::SendMessage(ListBox, LB_GETCURSEL, 0, 0));
					if(Index == LB_ERR) return TRUE;

					::SendMessage(ListBox, LB_DELETESTRING, Index, 0);

					return TRUE;
				}

				case DialogEventObjectButtonOk:
				{
					INT i;
					INT Size;
					INT Index;
					HWND ListBox;

					Index = static_cast<INT>(::SendMessage(GetDlgItem(Window, DialogEventObjectCombo1), CB_GETCURSEL, 0, 0));
					switch(::SendMessage(GetDlgItem(Window, DialogEventObjectCombo1), CB_GETITEMDATA, Index, 0))
					{
						case 0:
						{
							StaticNameType = "FPT";
							break;
						}

						case 1:
						{
							StaticNameType = "SPL";
							break;
						}

						case 2:
						{
							StaticNameType = "UBR";
							break;
						}

						case 3:
						{
							StaticNameType = "SND";
							break;
						}

						case 4:
						{
							StaticNameType = "SPN";
							break;
						}

						default:
						{
							Error.SetMessage("The event type is invalid!");
							Error.DisplayMessage(Window);
							Error.ClearMessage();

							return TRUE;
						}
					}

					StaticNameIdentifier = GetString(GetDlgItem(Window, DialogEventObjectEditIdentifier));
					if(StaticNameIdentifier.size() != 1)
					{
						Error.SetMessage("The identifier has to be exactly one character!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogEventObjectEditIdentifier));
						::SendMessage(GetDlgItem(Window, DialogEventObjectEditIdentifier), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticNameData = GetString(GetDlgItem(Window, DialogEventObjectEditData));
					if(StaticNameData.size() != 4)
					{
						Error.SetMessage("The data has to be exactly four characters!");
						Error.DisplayMessage(Window);
						Error.ClearMessage();

						SetFocus(GetDlgItem(Window, DialogEventObjectEditData));
						::SendMessage(GetDlgItem(Window, DialogEventObjectEditData), EM_SETSEL, 0, -1);

						return TRUE;
					}

					StaticData.Name = StaticNameType + StaticNameIdentifier + StaticNameData;

					ListBox = GetDlgItem(Window, DialogEventObjectList);
					Size = static_cast<INT>(::SendMessage(ListBox, LB_GETCOUNT, 0, 0));
					StaticData.EventTrack.clear();

					for(i = 0; i < Size; i++)
					{
						StaticData.EventTrack.push_back(static_cast<INT>(::SendMessage(ListBox, LB_GETITEMDATA, i, 0)));
					}

					StaticGlobalSequence = reinterpret_cast<MODEL_GLOBAL_SEQUENCE*>(::SendMessage(GetDlgItem(Window, DialogEventObjectComboGlobalSequenceId), CB_GETITEMDATA, ::SendMessage(GetDlgItem(Window, DialogEventObjectComboGlobalSequenceId), CB_GETCURSEL, 0, 0), 0));

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogEventObjectButtonCancel:
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
			::SendMessage(Window, WM_COMMAND, DialogEventObjectButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the track dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_EVENT_OBJECT_DIALOG::TrackDialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			AddWindow(Window);
			CenterWindow(Window);

			SetDlgItemInt(Window, DialogTrackEditTime, 0, FALSE);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogTrackButtonOk:
				{
					INT Time;

					Time = GetDlgItemInt(Window, DialogTrackEditTime, NULL, FALSE);

					RemoveWindow(Window);
					EndDialog(Window, Time);
					return TRUE;
				}

				case DialogTrackButtonCancel:
				{
					RemoveWindow(Window);
					EndDialog(Window, -1);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogTrackButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Inserts the data into a sorted position in the list
//+-----------------------------------------------------------------------------
VOID WINDOW_EVENT_OBJECT_DIALOG::SortedInsert(HWND ListBox, INT Data)
{
	INT i;
	INT Size;
	INT Index;
	std::stringstream Stream;

	Size = static_cast<INT>(::SendMessage(ListBox, LB_GETCOUNT, 0, 0));
	Index = -1;

	for(i = 0; i < Size; i++)
	{
		if(::SendMessage(ListBox, LB_GETITEMDATA, i, 0) > Data)
		{
			Index = i;
			break;
		}
	}

	Stream << Data;

	Index = static_cast<INT>(::SendMessage(ListBox, LB_INSERTSTRING, Index, reinterpret_cast<LPARAM>(Stream.str().c_str())));
	::SendMessage(ListBox, LB_SETITEMDATA, Index, Data);
}


//+-----------------------------------------------------------------------------
//| Builds the global sequence ID list
//+-----------------------------------------------------------------------------
VOID WINDOW_EVENT_OBJECT_DIALOG::BuildGlobalSequenceIdList(HWND Window)
{
	INT i;
	INT Index;
	HWND ComboBox;

	ComboBox = GetDlgItem(Window, DialogEventObjectComboGlobalSequenceId);
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
//| Builds the splat list
//+-----------------------------------------------------------------------------
VOID WINDOW_EVENT_OBJECT_DIALOG::BuildSplatList(HWND Window)
{
	HWND ComboBox;
	std::map<std::string, std::string>::iterator i;

	ComboBox = GetDlgItem(Window, DialogEventObjectCombo2);
	if(ComboBox == NULL) return;

	::SendMessage(ComboBox, CB_RESETCONTENT, 0, 0);

	i = SplatMap.begin();
	while(i != SplatMap.end())
	{
		::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(i->first.c_str()));
		i++;
	}

	::SendMessage(Window, CB_SETCURSEL, 0, 0);
}


//+-----------------------------------------------------------------------------
//| Builds the uber splat list
//+-----------------------------------------------------------------------------
VOID WINDOW_EVENT_OBJECT_DIALOG::BuildUberSplatList(HWND Window)
{
	HWND ComboBox;
	std::map<std::string, std::string>::iterator i;

	ComboBox = GetDlgItem(Window, DialogEventObjectCombo2);
	if(ComboBox == NULL) return;

	::SendMessage(ComboBox, CB_RESETCONTENT, 0, 0);

	i = UberSplatMap.begin();
	while(i != UberSplatMap.end())
	{
		::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(i->first.c_str()));
		i++;
	}

	::SendMessage(Window, CB_SETCURSEL, 0, 0);
}


//+-----------------------------------------------------------------------------
//| Builds the sound list
//+-----------------------------------------------------------------------------
VOID WINDOW_EVENT_OBJECT_DIALOG::BuildSoundList(HWND Window)
{
	HWND ComboBox;
	std::map<std::string, std::string>::iterator i;

	ComboBox = GetDlgItem(Window, DialogEventObjectCombo2);
	if(ComboBox == NULL) return;

	::SendMessage(ComboBox, CB_RESETCONTENT, 0, 0);

	i = SoundMap.begin();
	while(i != SoundMap.end())
	{
		::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(i->first.c_str()));
		i++;
	}

	::SendMessage(Window, CB_SETCURSEL, 0, 0);
}


//+-----------------------------------------------------------------------------
//| Builds the spawn object list
//+-----------------------------------------------------------------------------
VOID WINDOW_EVENT_OBJECT_DIALOG::BuildSpawnObjectList(HWND Window)
{
	HWND ComboBox;
	std::map<std::string, std::string>::iterator i;

	ComboBox = GetDlgItem(Window, DialogEventObjectCombo2);
	if(ComboBox == NULL) return;

	::SendMessage(ComboBox, CB_RESETCONTENT, 0, 0);

	i = SpawnObjectMap.begin();
	while(i != SpawnObjectMap.end())
	{
		::SendMessage(ComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(i->first.c_str()));
		i++;
	}

	::SendMessage(Window, CB_SETCURSEL, 0, 0);
}


//+-----------------------------------------------------------------------------
//| Registers a splat
//+-----------------------------------------------------------------------------
VOID WINDOW_EVENT_OBJECT_DIALOG::RegisterSplat(CONST std::string& Data, CONST std::string& Name)
{
	SplatMap.insert(std::make_pair(Name, Data));
}


//+-----------------------------------------------------------------------------
//| Registers an uber splat
//+-----------------------------------------------------------------------------
VOID WINDOW_EVENT_OBJECT_DIALOG::RegisterUberSplat(CONST std::string& Data, CONST std::string& Name)
{
	UberSplatMap.insert(std::make_pair(Name, Data));
}


//+-----------------------------------------------------------------------------
//| Registers a sound
//+-----------------------------------------------------------------------------
VOID WINDOW_EVENT_OBJECT_DIALOG::RegisterSound(CONST std::string& Data, CONST std::string& Name)
{
	SoundMap.insert(std::make_pair(Name, Data));
}


//+-----------------------------------------------------------------------------
//| Registers a spawn object
//+-----------------------------------------------------------------------------
VOID WINDOW_EVENT_OBJECT_DIALOG::RegisterSpawnObject(CONST std::string& Data, CONST std::string& Name)
{
	SpawnObjectMap.insert(std::make_pair(Name, Data));
}


//+-----------------------------------------------------------------------------
//| Returns the combobox index for a certain data
//+-----------------------------------------------------------------------------
INT WINDOW_EVENT_OBJECT_DIALOG::GetComboIndex(HWND Window, CONST std::string& Data, std::map<std::string, std::string>& Map)
{
	INT Index;
	HWND ComboBox;
	std::map<std::string, std::string>::iterator i;

	ComboBox = GetDlgItem(Window, DialogEventObjectCombo2);

	i = Map.begin();
	while(i != Map.end())
	{
		if(i->second == Data)
		{
			Index = static_cast<INT>(::SendMessage(ComboBox, CB_FINDSTRINGEXACT, -1, reinterpret_cast<LPARAM>(i->first.c_str())));
			if(Index == LB_ERR) return 0;
			
			return Index;
		}

		i++;
	}

	return 0;
}

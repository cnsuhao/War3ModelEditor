//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "MpqWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MPQ_WINDOW MpqWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MPQ_WINDOW::MPQ_WINDOW()
{
	CurrentlySelectedFile = NULL;
	CurrentlySelectedPopupFile = NULL;

	MpqLoaded = FALSE;
	MpqFileName = "";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MPQ_WINDOW::~MPQ_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::Create()
{
	WINDOW_TREEVIEW_INFO TreeviewInfo;

	FrameInfo.Title = "MPQ Browser";
	FrameInfo.Width = DEFAULT_MPQ_WINDOW_WIDTH;
	FrameInfo.Height = DEFAULT_MPQ_WINDOW_HEIGHT;
	FrameInfo.Menu = ::LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(MpqMenu));
	FrameInfo.Style &= (~WS_VISIBLE);
	FrameInfo.Style |= (WS_MAXIMIZEBOX | WS_SIZEBOX);

	if(!WINDOW_FRAME::Create()) return FALSE;

	TreeviewInfo.Parent = Window;

	if(!TreeView.Create(TreeviewInfo)) return FALSE;
	
	Icon.AssignIconListToTreeView(TreeView.GetWindow());
	if(!Filter.AddFilterItemsToMenu(GetMenu())) return FALSE;
	Filter.UpdateFilterMenu(GetMenu());

	RootNode.FilterData.FilterList.resize(Filter.GetNrOfFilters(), FALSE);

	if(!OpenAllMpqArchives()) return FALSE;

	ResizeTreeView();
	UpdateWindowStatus();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID MPQ_WINDOW::Destroy()
{
	Mpq.Close();
	CloseAllMpqArchives();

	TreeView.Destroy();

	WINDOW_FRAME::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT MPQ_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_SIZE:
		case WM_SIZING:
		{
			ResizeTreeView();
			return 0;
		}

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
LRESULT MPQ_WINDOW::MenuHandler(WORD MenuItem)
{
	switch(MenuItem)
	{
		case MpqFileOpen:
		{
			if(!OpenMpq())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MpqFileClose:
		{
			if(!CloseMpq())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MpqFileWar3Mpq:
		{
			if(!OpenMpqByName(Register.GetWarcraftDirectory() + "\\" + PATH_MPQ_WAR3))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MpqFileWar3xMpq:
		{
			if(!OpenMpqByName(Register.GetWarcraftDirectory() + "\\" + PATH_MPQ_WAR3X))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MpqFileWar3xLocalMpq:
		{
			if(!OpenMpqByName(Register.GetWarcraftDirectory() + "\\" + PATH_MPQ_WAR3X_LOCAL))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MpqFileWar3PatchMpq:
		{
			if(!OpenMpqByName(Register.GetWarcraftDirectory() + "\\" + PATH_MPQ_WAR3_PATCH))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MpqFiltersOther:
		{
			Filter.SelectOther();
			Filter.UpdateFilterMenu(GetMenu());
			UpdateTreeView();
			return 0;
		}

		case MpqFiltersAll:
		{
			Filter.SelectAll();
			Filter.UpdateFilterMenu(GetMenu());
			UpdateTreeView();
			return 0;
		}

		case MpqFiltersNone:
		{
			Filter.SelectNone();
			Filter.UpdateFilterMenu(GetMenu());
			UpdateTreeView();
			return 0;
		}

		case MpqPopupOpen:
		case MpqPopupOpen2:
		{
			if(CurrentlySelectedPopupFile == NULL) return 0;
			if(!OpenMpqFile(CurrentlySelectedPopupFile->FileNameAndPath))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MpqPopupExport:
		case MpqPopupExport2:
		{
			std::string FileName;
			WINDOW_FILE_DIALOG FileDialog;

			if(CurrentlySelectedPopupFile == NULL) return 0;

			FileDialog.SetTitle("Export File");
			FileDialog.SetFilter(FILTER_ALL);
			
			FileName = CurrentlySelectedPopupFile->FileName;
			if(!FileDialog.SaveFile(Window, FileName)) return 0;

			if(!ExportMpqFile(CurrentlySelectedPopupFile->FileNameAndPath, FileName))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MpqPopupCopyPath:
		case MpqPopupCopyPath2:
		{
			if(CurrentlySelectedPopupFile == NULL) return 0;

			if(!Common.SetClipboardData(CurrentlySelectedPopupFile->FileNameAndPath))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MpqPopupUseAsTexture:
		case MpqPopupUseAsTexture2:
		{
			std::string FileName;

			if(CurrentlySelectedPopupFile == NULL) return 0;

			FileName = CurrentlySelectedPopupFile->FileNameAndPath;
			if(!TextureManagerWindow.UseTextureFromMpq(FileName))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}
	}

	if(MenuItem < Filter.GetMinMenuItemId()) return 0;
	if(MenuItem > Filter.GetMaxMenuItemId()) return 0;

	Filter.Select(MenuItem);
	Filter.UpdateFilterMenu(GetMenu());
	UpdateTreeView();

	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window control messages
//+-----------------------------------------------------------------------------
LRESULT MPQ_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT MPQ_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	if(Control == TreeView.GetWindow())
	{
		NM_TREEVIEW* TreeViewHeader;

		TreeViewHeader = reinterpret_cast<NM_TREEVIEW*>(Header);
		if(TreeViewHeader == NULL) return 0;

		return TreeViewHandler(Code, TreeViewHeader);
	}

	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the treeview messages
//+-----------------------------------------------------------------------------
LRESULT MPQ_WINDOW::TreeViewHandler(UINT Code, NM_TREEVIEW* TreeViewHeader)
{
	TVITEM ItemData;
	POINT MousePosition;
	POINT LocalMousePosition;
	TVHITTESTINFO HitTestInfo;

	GetCursorPos(&MousePosition);
	LocalMousePosition = MousePosition;
	ScreenToClient(TreeView.GetWindow(), &LocalMousePosition);

	switch(Code)
	{
		case NM_DBLCLK:
		{
			if(CurrentlySelectedFile == NULL) return 0;
			if(CurrentlySelectedFile->IsFolder) return 0;

			ZeroMemory(&HitTestInfo, sizeof(TVHITTESTINFO));
			HitTestInfo.pt = LocalMousePosition;
			if(TreeView_HitTest(TreeView.GetWindow(), &HitTestInfo) == NULL) return 0;
			if(!(HitTestInfo.flags & TVHT_ONITEM)) return 0;

			if(!OpenMpqFile(CurrentlySelectedFile->FileNameAndPath))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NM_RCLICK:
		{
			INT MenuId;
			std::string FileName;
			HTREEITEM TreeViewItem;

			ZeroMemory(&HitTestInfo, sizeof(TVHITTESTINFO));
			HitTestInfo.pt = LocalMousePosition;
			if(TreeView_HitTest(TreeView.GetWindow(), &HitTestInfo) == NULL) return 0;
			if(!(HitTestInfo.flags & TVHT_ONITEM)) return 0;

			TreeViewItem = HitTestInfo.hItem;
			if(TreeViewItem == NULL) return 0;

			ZeroMemory(&ItemData, sizeof(TVITEM));
			ItemData.mask = TVIF_PARAM;
			ItemData.hItem = TreeViewItem;
			if(!TreeView_GetItem(TreeView.GetWindow(), &ItemData)) return 0;

			CurrentlySelectedPopupFile = reinterpret_cast<FILE_NODE*>(ItemData.lParam);
			if(CurrentlySelectedPopupFile == NULL) return 0;

			if(CurrentlySelectedPopupFile->IsFolder) return 0;

			FileName = CurrentlySelectedPopupFile->FileName;
			MenuId = (MainWindow.ModelIsLoaded() && ResourceLoader.IsTextureExtention(Common.GetExtention(FileName))) ? MpqPopupMenu : MpqPopupMenu2;
			if(!TrackPopupMenuEx(GetSubMenu(LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(MenuId)), 0),
								 TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePosition.x, MousePosition.y,
								 Window, NULL))
			{
				Error.SetMessage("Unable to create popup menu!");
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case TVN_SELCHANGED:
		{
			CurrentlySelectedFile = reinterpret_cast<FILE_NODE*>(TreeViewHeader->itemNew.lParam);
			if(CurrentlySelectedFile == NULL) return 0;

			return 0;
		}
	}

	return 0;
}


//+-----------------------------------------------------------------------------
//| Opens all MPQ archives
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::OpenAllMpqArchives()
{
	BOOL Result = FALSE;
	std::string FileName;
	std::string ErrorMessage;
	std::string WarcraftDirectory;

	WarcraftDirectory = Register.GetWarcraftDirectory();

	while(TRUE)
	{
		FileName = WarcraftDirectory + "\\" + PATH_MPQ_WAR3;
		if(!MpqWar3.Open(FileName)) break;

		FileName = WarcraftDirectory + "\\" + PATH_MPQ_WAR3X;
		if(!MpqWar3x.Open(FileName)) break;

		FileName = WarcraftDirectory + "\\" + PATH_MPQ_WAR3X_LOCAL;
		if(!MpqWar3xLocal.Open(FileName)) break;

		FileName = WarcraftDirectory + "\\" + PATH_MPQ_WAR3_PATCH;
		if(!MpqWar3Patch.Open(FileName)) break;

		Result = TRUE;
		break;
	}

	if(!Result)
	{
		ErrorMessage = "Unable to open \"" + FileName + "\"!\n\n";
		ErrorMessage += "Make sure that Warcraft 3 is installed and that the registry key\n";
		ErrorMessage += "\"HKEY_CURRENT_USER\\Blizzard Entertainment\\Warcraft III\\InstallPath\" exists!";
		Error.SetMessage(ErrorMessage);
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Closes all MPQ archives
//+-----------------------------------------------------------------------------
VOID MPQ_WINDOW::CloseAllMpqArchives()
{
	MpqWar3.Close();
	MpqWar3x.Close();
	MpqWar3xLocal.Close();
	MpqWar3Patch.Close();
}


//+-----------------------------------------------------------------------------
//| Checks if an extention is an mpq archive extention
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::IsMpqExtention(CONST std::string& Extention)
{
	if(Extention == "mpq") return TRUE;
	if(Extention == "w3m") return TRUE;
	if(Extention == "w3x") return TRUE;
	if(Extention == "w3n") return TRUE;

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Opens an mpq archive by name
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::OpenMpqByName(CONST std::string& FileName)
{
	CloseMpq();

	if(!Mpq.Open(FileName)) return FALSE;
	if(!LoadListFile(FileName)) return FALSE;

	MpqLoaded = TRUE;
	MpqFileName = FileName;

	UpdateTreeView();
	UpdateWindowStatus();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Opens an mpq archive
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::OpenMpq()
{
	std::string FileName;
	WINDOW_FILE_DIALOG FileDialog;

	FileDialog.SetTitle("Open MPQ Archive");
	FileDialog.SetFilter(FILTER_MPQ);
	if(!FileDialog.OpenFile(Window, FileName)) return TRUE;

	if(!OpenMpqByName(FileName)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Closes the mpq archive
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::CloseMpq()
{
	RemoveFileTree(&RootNode);
	Mpq.Close();

	MpqLoaded = FALSE;
	MpqFileName = "";

	UpdateTreeView();
	UpdateWindowStatus();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Updates the window status
//+-----------------------------------------------------------------------------
VOID MPQ_WINDOW::UpdateWindowStatus()
{
	std::string Title;

	GrayMenuItem(MpqFileClose, !MpqLoaded);

	Title = EDITOR_MPQ_NAME;
	if(MpqLoaded) Title += (" (" + Common.GetFileName(MpqFileName) + ")");

	SetText(Title);
}


//+-----------------------------------------------------------------------------
//| Opens a file from the mpq archive
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::OpenMpqFile(CONST std::string& FileName)
{
	BUFFER Buffer;
	SHELLEXECUTEINFO Info;
	std::string Extention;
	std::string TempFileName;

	Extention = Common.LowerCase(Common.GetExtention(FileName));

	if(InvalidExtention(Extention))
	{
		Error.SetMessage("This file cannot be opened from here!\nExport it first then open it!");
		return FALSE;
	}
	else if(ModelExtention(Extention))
	{
		BUFFER Buffer;

		if(!Mpq.LoadFile(FileName, Buffer)) return FALSE;
		if(!MainWindow.LoadFileFromBuffer(FileName, Buffer)) return FALSE;

		MainWindow.MakeModelNeverSaved();

		return TRUE;
	}
	else if(TextureExtention(Extention))
	{
		BUFFER Buffer;

		if(!Mpq.LoadFile(FileName, Buffer)) return FALSE;
		if(!MainWindow.LoadFileFromBuffer(FileName, Buffer)) return FALSE;

		MainWindow.MakeModelNeverSaved();

		return TRUE;
	}

	TempFileName = TemporaryFile.GetTemporaryDirectory() + "\\" + Common.GetFileName(FileName);

	if(!Mpq.LoadFile(FileName, Buffer)) return FALSE;
	if(!TemporaryFile.CreateTemporaryFile(TempFileName, Buffer)) return FALSE;

	ZeroMemory(&Info, sizeof(SHELLEXECUTEINFO));
	Info.cbSize = sizeof(SHELLEXECUTEINFO);
	Info.fMask = SEE_MASK_NOCLOSEPROCESS;
	Info.hwnd = Window;
	Info.lpVerb = "open";
	Info.lpFile = TempFileName.c_str();
	Info.hInstApp = GetModuleHandle(NULL);
	Info.nShow = SW_SHOW;

	if(ScriptExtention(Extention) || (FileName == PATH_LISTFILE))
	{
		Info.lpFile = "Notepad";
		Info.lpParameters = TempFileName.c_str();
	}

	if(!ShellExecuteEx(&Info))
	{
		Error.SetMessage("Unable to open \"" + TempFileName + "\"!\nProbably an unassociated extention!");
		TemporaryFile.DestroyTemporaryFile(TempFileName);
		return FALSE;
	}

	if(Info.hProcess == NULL) return TRUE;

	if(!TemporaryFile.LaunchThread(new FILE_PROCESS(Info.hProcess, TempFileName)))
	{
		TemporaryFile.DestroyTemporaryFile(TempFileName);
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Exports a file from the mpq archive
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::ExportMpqFile(CONST std::string& FileName, CONST std::string& RealFileName)
{
	BUFFER Buffer;

	if(!Mpq.LoadFile(FileName, Buffer)) return FALSE;
	if(!FileLoader.SaveToFile(RealFileName, Buffer)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Updates the treeview
//+-----------------------------------------------------------------------------
VOID MPQ_WINDOW::UpdateTreeView()
{
	FILE_NODE* CurrentNode;
	FILE_NODE* CurrentChildNode;
	TVINSERTSTRUCT InsertItem;
	HTREEITEM TreeViewItem;
	std::list<FILE_NODE*> Stack;
	INT FolderIconIndex;
	INT UnknownIconIndex;

	TreeView.Clear();

	InsertItem.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	InsertItem.item.hItem = NULL;
	InsertItem.item.state = 0;
	InsertItem.item.stateMask = 0;
	InsertItem.item.cChildren = 0;

	FolderIconIndex = Icon.GetFolderIconIndex();
	UnknownIconIndex = Icon.GetUnknownIconIndex();

	RootNode.TreeViewItem = TVI_ROOT;
	Stack.push_back(&RootNode);

	while(Stack.size() > 0)
	{
		CurrentNode = Stack.back();
		Stack.pop_back();

		if(!Filter.ValidFilter(CurrentNode->FilterData)) continue;

		CurrentChildNode = CurrentNode->FileConnection.FirstChildNode;
		while(CurrentChildNode != NULL)
		{
			Stack.push_back(CurrentChildNode);
			CurrentChildNode = CurrentChildNode->NextNode;
		}

		CurrentChildNode = CurrentNode->FolderConnection.FirstChildNode;
		while(CurrentChildNode != NULL)
		{
			Stack.push_back(CurrentChildNode);
			CurrentChildNode = CurrentChildNode->NextNode;
		}

		if(CurrentNode == &RootNode) continue;

		InsertItem.item.pszText = const_cast<CHAR*>(CurrentNode->FileName.c_str());
		InsertItem.item.cchTextMax = static_cast<INT>(CurrentNode->FileName.size());
		InsertItem.item.iImage = (CurrentNode->IsFolder ? FolderIconIndex : Icon.GetIconIndex(CurrentNode->Extention));
		InsertItem.item.iSelectedImage = InsertItem.item.iImage;
		InsertItem.item.lParam = reinterpret_cast<LPARAM>(CurrentNode);
		InsertItem.hParent = CurrentNode->ParentNode->TreeViewItem;
		InsertItem.hInsertAfter = TVI_LAST;

		TreeViewItem = TreeView_InsertItem(TreeView.GetWindow(), &InsertItem);
		if(TreeViewItem == NULL) continue;

		CurrentNode->TreeViewItem = TreeViewItem;
	}

	CurrentlySelectedFile = NULL;
	CurrentlySelectedPopupFile = NULL;
}


//+-----------------------------------------------------------------------------
//| Resizes the treeview
//+-----------------------------------------------------------------------------
VOID MPQ_WINDOW::ResizeTreeView()
{
	TreeView.SetDimension(GetWidth(), GetHeight());
}


//+-----------------------------------------------------------------------------
//| Loads the listfile from the mpq (or from a custom listfile)
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::LoadListFile(CONST std::string& FileName)
{
	std::string Name;
	std::ifstream File;
	std::string ListFileName;
	BOOL UseCustomListFile = TRUE;
	std::list<std::string> FileList;
	std::list<std::string>::iterator i;

	Filter.BuildFilter(RootNode.FilterData, "");
	ListFileName = Common.GetProgramDirectory() + "\\" + PATH_LIST_FILES + Common.GetName(FileName) + ".txt";

	File.open(ListFileName.c_str(), std::ios::in);
	if(File.fail()) UseCustomListFile = FALSE;

	if(UseCustomListFile)
	{
		while(!File.eof())
		{
			Name = "";
			std::getline(File, Name);

			Common.RemoveWhiteSpace(Name);
			if(Name != "")
			{
				if(!AddFileToFileTree(Name)) return FALSE;
			}
		}
	}
	else
	{
		Mpq.GetFileList(FileList);

		i = FileList.begin();
		while(i != FileList.end())
		{
			Name = (*i);

			Common.RemoveWhiteSpace(Name);
			if(Name != "")
			{
				if(!AddFileToFileTree(Name)) return FALSE;
			}

			i++;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a file to the file tree
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::AddFileToFileTree(CONST std::string& FileName)
{
	std::string Name;
	std::list<std::string> PathList;
	std::list<std::string>::iterator i;
	FILE_NODE* NewNode;
	FILE_NODE* CurrentNode;

	SplitPath(FileName, Name, PathList);

	CurrentNode = &RootNode;

	i = PathList.begin();
	while(i != PathList.end())
	{
		CurrentNode = CreateFolder(CurrentNode, (*i));
		if(CurrentNode == NULL)
		{
			Error.SetMessage("Unable to add \"" + FileName + "\" to the treeview!");
			return FALSE;
		}

		i++;
	}

	NewNode = new FILE_NODE();
	if(NewNode == NULL)
	{
		Error.SetMessage("Unable to add \"" + FileName + "\" to the treeview, memory allocation failed!");
		return FALSE;
	}

	NewNode->FileNameAndPath = FileName;
	NewNode->FileName = Name;
	NewNode->Extention = Common.LowerCase(Common.GetExtention(Name));

	if(NewNode->FileName == PATH_LISTFILE) NewNode->Extention = "txt";

	AddChild(CurrentNode, &CurrentNode->FileConnection, NewNode);

	Filter.BuildFilter(NewNode->FilterData, NewNode->Extention);
	UpdateFilterData(NewNode, NewNode->FilterData);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes all childs of a file node
//+-----------------------------------------------------------------------------
VOID MPQ_WINDOW::RemoveFileTree(FILE_NODE* FileNode)
{
	FILE_NODE* CurrentNode;
	FILE_NODE* NextNode;

	CurrentNode = FileNode->FileConnection.FirstChildNode;
	while(CurrentNode != NULL)
	{
		NextNode = CurrentNode->NextNode;

		RemoveFileTree(CurrentNode);
		delete CurrentNode;

		CurrentNode = NextNode;
	}

	CurrentNode = FileNode->FolderConnection.FirstChildNode;
	while(CurrentNode != NULL)
	{
		NextNode = CurrentNode->NextNode;

		RemoveFileTree(CurrentNode);
		delete CurrentNode;

		CurrentNode = NextNode;
	}

	FileNode->FileConnection.FirstChildNode = NULL;
	FileNode->FileConnection.LastChildNode = NULL;
	FileNode->FolderConnection.FirstChildNode = NULL;
	FileNode->FolderConnection.LastChildNode = NULL;
}


//+-----------------------------------------------------------------------------
//| Finds the child of a file node
//+-----------------------------------------------------------------------------
FILE_NODE* MPQ_WINDOW::FindChild(FILE_NODE* FileNode, FILE_NODE_CONNECTION* Connection, CONST std::string& Name)
{
	FILE_NODE* ChildNode;
	std::string LowerCaseName;

	LowerCaseName = Common.LowerCase(Name);

	ChildNode = Connection->FirstChildNode;
	while(ChildNode != NULL)
	{
		if(Common.LowerCase(ChildNode->FileName) == LowerCaseName) return ChildNode;
		ChildNode = ChildNode->NextNode;
	}

	return NULL;
}


//+-----------------------------------------------------------------------------
//| Finds or creates a folder node
//+-----------------------------------------------------------------------------
FILE_NODE* MPQ_WINDOW::CreateFolder(FILE_NODE* FileNode, CONST std::string& Name)
{
	FILE_NODE* ChildNode;

	ChildNode = FindChild(FileNode, &FileNode->FolderConnection, Name);
	if(ChildNode != NULL) return ChildNode;

	ChildNode = new FILE_NODE();
	if(ChildNode == NULL) return NULL;

	ChildNode->IsFolder = TRUE;
	ChildNode->IconCollectionIndex = Icon.GetFolderIconIndex();
	ChildNode->FileName = Name;

	if(ChildNode->FileName != "")
	{
		ChildNode->FileName[0] = toupper(ChildNode->FileName[0]);
	}

	Filter.BuildFilter(ChildNode->FilterData, "");

	AddChild(FileNode, &FileNode->FolderConnection, ChildNode);

	return ChildNode;
}


//+-----------------------------------------------------------------------------
//| Adds a child node to a node
//+-----------------------------------------------------------------------------
VOID MPQ_WINDOW::AddChild(FILE_NODE* FileNode, FILE_NODE_CONNECTION* Connection, FILE_NODE* ChildNode)
{
	FILE_NODE* CurrentChildNode;
	std::string ChildNodeName;
	BOOL LocationFound = FALSE;

	ChildNodeName = Common.LowerCase(ChildNode->FileName);

	CurrentChildNode = Connection->FirstChildNode;
	while(CurrentChildNode != NULL)
	{
		if(ChildNodeName > Common.LowerCase(CurrentChildNode->FileName)) break;
		CurrentChildNode = CurrentChildNode->NextNode;
	}

	if(CurrentChildNode != NULL)
	{
		ChildNode->NextNode = CurrentChildNode;
		ChildNode->PreviousNode = CurrentChildNode->PreviousNode;
		ChildNode->ParentNode = FileNode;

		CurrentChildNode->PreviousNode = ChildNode;
		if(ChildNode->PreviousNode) ChildNode->PreviousNode->NextNode = ChildNode;

		if(Connection->FirstChildNode == CurrentChildNode) Connection->FirstChildNode = ChildNode;

		return;
	}

	ChildNode->NextNode = NULL;
	ChildNode->PreviousNode = Connection->LastChildNode;
	ChildNode->ParentNode = FileNode;

	if(Connection->FirstChildNode == NULL) Connection->FirstChildNode = ChildNode;
	if(Connection->LastChildNode != NULL) Connection->LastChildNode->NextNode = ChildNode;
	Connection->LastChildNode = ChildNode;
}


//+-----------------------------------------------------------------------------
//| Updates the node and its parents with the filter data
//+-----------------------------------------------------------------------------
VOID MPQ_WINDOW::UpdateFilterData(FILE_NODE* FileNode, FILTER_DATA& FilterData)
{
	FILE_NODE* CurrentNode;

	CurrentNode = FileNode->ParentNode;
	while(CurrentNode != NULL)
	{
		Filter.ExtendFilter(CurrentNode->FilterData, FileNode->FilterData);
		CurrentNode = CurrentNode->ParentNode;
	}
}


//+-----------------------------------------------------------------------------
//| Splits a path into a list of path parts
//+-----------------------------------------------------------------------------
VOID MPQ_WINDOW::SplitPath(CONST std::string& Path, std::string& FileName, std::list<std::string>& PathList)
{
	INT i;
	std::string TempPath = Path;

	i = static_cast<INT>(TempPath.find("\\"));
	while(i != std::string::npos)
	{
		if(i > 0) PathList.push_back(TempPath.substr(0, i));
		TempPath.erase(0, i + 1);

		i = static_cast<INT>(TempPath.find("\\"));
	}

	FileName = TempPath;
}


//+-----------------------------------------------------------------------------
//| Checks if the extention is invalid
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::InvalidExtention(CONST std::string& Extention)
{
	if(Extention == "exe") return TRUE;
	if(Extention == "dll") return TRUE;
	if(Extention == "w3m") return TRUE;
	if(Extention == "w3x") return TRUE;
	if(Extention == "w3n") return TRUE;
	if(Extention == "w3g") return TRUE;
	if(Extention == "mpq") return TRUE;

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Checks if the extention is a model
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::ModelExtention(CONST std::string& Extention)
{
	return ResourceLoader.IsModelExtention(Extention);
}


//+-----------------------------------------------------------------------------
//| Checks if the extention is a texture
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::TextureExtention(CONST std::string& Extention)
{
	return ResourceLoader.IsTextureExtention(Extention);
}


//+-----------------------------------------------------------------------------
//| Checks if the extention is a script file
//+-----------------------------------------------------------------------------
BOOL MPQ_WINDOW::ScriptExtention(CONST std::string& Extention)
{
	if(Extention == "ai")  return TRUE;
	if(Extention == "wai") return TRUE;
	if(Extention == "j")   return TRUE;
	if(Extention == "js")  return TRUE;
	if(Extention == "pld") return TRUE;

	return FALSE;
}

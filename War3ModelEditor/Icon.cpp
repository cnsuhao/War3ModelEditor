//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Icon.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
ICON Icon;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
ICON::ICON()
{
	FolderIconIndex = INVALID_INDEX;
	UnknownIconIndex = INVALID_INDEX;

	IconCollection = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
ICON::~ICON()
{
	ClearAllIcons();
}


//+-----------------------------------------------------------------------------
//| Clears all icons
//+-----------------------------------------------------------------------------
VOID ICON::ClearAllIcons()
{
	IconInfoContainer.Clear();

	if(IconCollection != NULL)
	{
		ImageList_Destroy(IconCollection);
		IconCollection = NULL;
	}

	FolderIconIndex = INVALID_INDEX;
	UnknownIconIndex = INVALID_INDEX;
}


//+-----------------------------------------------------------------------------
//| Loads all icons
//+-----------------------------------------------------------------------------
BOOL ICON::LoadAllIcons()
{
	INT i;
	INT Index;
	BOOL Looping;
	HANDLE SearchHandle;
	WIN32_FIND_DATA FileData;
	std::string Path;
	std::string FileName;

	ClearAllIcons();

	Path = Common.GetProgramDirectory() + "\\" + PATH_ICON_FILES;

	if(!LoadIconFromResource("FOLDER", IconFolder)) return FALSE;
	if(!LoadIconFromResource("UNKNOWN", IconUnknown)) return FALSE;

	SearchHandle = FindFirstFile(Path.c_str(), &FileData);
	Looping = (SearchHandle != INVALID_HANDLE_VALUE);

	while(Looping)
	{
		FileName = Common.GetProgramDirectory() + "\\" + PATH_ICONS + FileData.cFileName;
		if(!LoadIconFromFile(FileName))
		{
			FindClose(SearchHandle);
			return FALSE;
		}

		if(!FindNextFile(SearchHandle, &FileData))
		{
			FindClose(SearchHandle);
			Looping = FALSE;
		}
	}

	IconCollection = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR8 | ILC_MASK, 1, 1);
	if(IconCollection == NULL)
	{
		Error.SetMessage("Unable to create an icon list!");
		return FALSE;
	}

	for(i = 0; i < IconInfoContainer.GetSize(); i++)
	{
		Index = ImageList_AddIcon(IconCollection, IconInfoContainer[i].Icon);
		if(Index == -1)
		{
			Error.SetMessage("Unable to add icons to the icon list!");
			return FALSE;
		}

		IconInfoContainer[i].IconCollectionIndex = Index;

		if(i == 0) FolderIconIndex = Index;
		if(i == 1) UnknownIconIndex = Index;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the icon index of the folder icon
//+-----------------------------------------------------------------------------
INT ICON::GetFolderIconIndex() CONST
{
	return FolderIconIndex;
}


//+-----------------------------------------------------------------------------
//| Returns the icon index of the unknown icon
//+-----------------------------------------------------------------------------
INT ICON::GetUnknownIconIndex() CONST
{
	return UnknownIconIndex;
}


//+-----------------------------------------------------------------------------
//| Returns the icon index of a specific extention icon
//+-----------------------------------------------------------------------------
INT ICON::GetIconIndex(CONST std::string& Extention) CONST
{
	INT Index;

	Index = IconInfoContainer.GetIndex(Extention);
	if(Index == INVALID_INDEX) return UnknownIconIndex;

	return IconInfoContainer[Index].IconCollectionIndex;
}


//+-----------------------------------------------------------------------------
//| Assigns the icon list to a treeview
//+-----------------------------------------------------------------------------
VOID ICON::AssignIconListToTreeView(HWND TreeView)
{
	TreeView_SetImageList(TreeView, IconCollection, TVSIL_NORMAL);
}


//+-----------------------------------------------------------------------------
//| Loads an icon from a file
//+-----------------------------------------------------------------------------
BOOL ICON::LoadIconFromFile(CONST std::string& FileName)
{
	ICON_INFO IconInfo;
	std::string Extention;

	IconInfo.Icon = static_cast<HICON>(LoadImage(GetModuleHandle(NULL), FileName.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	if(IconInfo.Icon == NULL)
	{
		Error.SetMessage("Unable to load the icon \"" + FileName + "\"!");
		return FALSE;
	}

	Extention = Common.LowerCase(Common.GetName(FileName));

	if(!IconInfoContainer.Add(Extention, IconInfo))
	{
		Error.SetMessage("Unable to add the icon \"" + FileName + "\"!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads an icon from a resource
//+-----------------------------------------------------------------------------
BOOL ICON::LoadIconFromResource(CONST std::string& Name, UINT ResourceId)
{
	ICON_INFO IconInfo;

	IconInfo.Icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ResourceId));
	if(IconInfo.Icon == NULL)
	{
		Error.SetMessage("Unable to load the icon \"" + Name + "\"!");
		return FALSE;
	}

	if(!IconInfoContainer.Add(Name, IconInfo))
	{
		Error.SetMessage("Unable to add the icon \"" + Name + "\"!");
		return FALSE;
	}

	return TRUE;
}
